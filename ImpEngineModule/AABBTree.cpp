#include "EngineModulePCH.h"
#include "AABBTree.h"
#include "Node.h"
#include "Collider.h"
#include "Entity.h"
#include "Collision.h"

ImpEngineModule::Physics::AABBTree::AABBTree(float margin)
	:_margin(margin), _root(nullptr), _invalidNodes{}, _collisions{}
{

}

ImpEngineModule::Physics::AABBTree::~AABBTree()
{
	Clear();
}

void ImpEngineModule::Physics::AABBTree::Add(Collider* collider)
{
	Node* node = new Node();

	collider->SetNode(node);

	if (_root)
	{
		// ��Ʈ��尡 �ƴϴ�
		node->SetLeaf(collider);
		node->UpdateAABB(_margin);
		InsertNode(node, &_root);
	}
	else
	{
		// ��Ʈ ���
		_root = node;
		_root->SetLeaf(collider);
		_root->UpdateAABB(_margin);
	}

}

void ImpEngineModule::Physics::AABBTree::Remove(Collider* collider)
{
	Node* node = collider->GetNode();

	node->collider = nullptr;
	collider->SetNode(nullptr);

	RemoveNode(node);
}

void ImpEngineModule::Physics::AABBTree::Update()
{
	if (_root)
	{
		if (_root->IsLeaf())
		{
			// ��Ʈ ��尡 ���������� ���
			if (_root->collider->GetEntity()->GetState() == Entity::EntityState::Destroyed)
			{
				Remove(_root->collider);
			}
			else
			{
				_root->UpdateAABB(_margin);
			}
		}
		else
		{
			// grab all invalid nodes
			// ��� ��ȿ���� �ʴ� ��带 �����ɴϴ�
			// ������ ������Ʈ�� �߰����� 
			_invalidNodes.clear();
			_removeNodes.clear();
			UpdateNodeHelper(_root);

			// ���������� ������Ʈ�� Ʈ������ ����
			for (Node* node : _removeNodes)
			{
				Remove(node->collider);
			}

			// re-insert all invalid nodes
			// ��ȿ���� �ʴ� ��� ��带 �ٽ� ����
			for (Node* node : _invalidNodes)
			{
				// grab parent link
				// (pointer to the pointer that points to parent)
				// �θ� ��ũ ��� (�θ� ����Ű�� �����͸� ����Ű�� ������)
				Node* parent = node->parent;
				Node* siblibg = node->GetSibling();
				Node** parentLink = parent->parent
					? (parent == parent->parent->children[0]
						? &parent->parent->children[0]
						: &parent->parent->children[1])
					: &_root;

				// replace parent with sibling
				siblibg->parent =
					parent->parent
					? parent->parent
					: nullptr; // root has null parent

				*parentLink = siblibg;
				delete parent;

				// re-insert node 
				// Ʈ���� �ٽ�Ȯ�� 
				node->UpdateAABB(_margin);
				InsertNode(node, &_root);
			}
			_invalidNodes.clear();
			_removeNodes.clear();
		}
	}
}

const ImpEngineModule::Physics::IBroadphase::CollisionVector& ImpEngineModule::Physics::AABBTree::ComputeCollisions()
{
	_collisions.clear();

	if (_root == nullptr || _root->IsLeaf())
	{
		return _collisions;
	}

	ClearChildrenCrossFlagHelper(_root);

	ComputePairsHelper(_root->children[0], _root->children[1]);

	return _collisions;
}


void ImpEngineModule::Physics::AABBTree::Clear()
{
	std::queue<Node*> q;

	if (_root)
	{
		q.push(_root);
	}

	while (!q.empty())
	{
		Node* node = q.front();
		q.pop();

		if (node->children[0])
		{
			q.push(node->children[0]);
			q.push(node->children[1]);
		}
		delete node;
	}

	_root = nullptr;
}

void ImpEngineModule::Physics::AABBTree::UpdateNodeHelper(Node* node)
{
	if (node->IsLeaf())
	{
		// ���� ������ ������Ʈ
 		if (node->collider->GetEntity()->GetState() == Entity::EntityState::Destroyed)
		{
			_removeNodes.push_back(node);
		}
		else if (!node->aabb.Contains(node->collider))
		{
			// check if fat AABB dosen't contain the collider's AABB anymore
			// Ȯ���� AABB�ڽ����� �ݶ��̴��� ����� invalidNodes�� �����͸� �߰��Ѵ�
			_invalidNodes.push_back(node);
		}
	}
	else
	{
		UpdateNodeHelper(node->children[0]);
		UpdateNodeHelper(node->children[1]);
	}
}

void ImpEngineModule::Physics::AABBTree::InsertNode(Node* node, Node** _parent)
{
	Node* p = *_parent;
	if (p->IsLeaf())
	{
		// parent is leaf, simply split
		// �θ� �������, ������ �и�
		Node* newParent = new Node();
		newParent->parent = p->parent;
		newParent->SetBranch(node, p);
		*_parent = newParent;
	}
	else
	{
		// parent is branch, compute volume differences
		// between pre-insert and post-insert��
		// �θ� �귣ġ�̸�, �������� ���� �������̸� ���
		const AABB aabb0 = p->children[0]->aabb;
		const AABB aabb1 = p->children[1]->aabb;

		const float volumeDiff0 = AABB::Union(node->aabb, aabb0).GetVolume() - aabb0.GetVolume();
		const float volumeDiff1 = AABB::Union(node->aabb, aabb1).GetVolume() - aabb1.GetVolume();

		// insert to the child that gives less volume increase;
		// ���� ������ ���� �ڽĳ�忡�� �����Ѵ�.
		if (volumeDiff0 < volumeDiff1)
			InsertNode(node, &p->children[0]);
		else
			InsertNode(node, &p->children[1]);
	}

	// update parent AABB
	// (propagate back up the recursion stack)
	// �θ� AABB ������Ʈ (������� ȣ��)
	(*_parent)->UpdateAABB(_margin);
}

void ImpEngineModule::Physics::AABBTree::RemoveNode(Node* node)
{
	Node* parent = node->parent;
	if (parent)
	{
		Node* sibling = node->GetSibling();
		if (parent->parent)
		{
			sibling->parent = parent->parent;
			(parent == parent->parent->children[0] ?
				parent->parent->children[0] : parent->parent->children[1]) = sibling;
		}
		else
		{
			_root = sibling;
			sibling->parent = nullptr;
		}
		delete node;
		delete parent;
	}
	else
	{
		_root = nullptr;
		delete node;
	}

}

void ImpEngineModule::Physics::AABBTree::ComputePairsHelper(Node* n0, Node* n1)
{
	if (n0->IsLeaf())
	{
		// 2 leaves, check proxies instead of fat AABBs
		if (n1->IsLeaf())
		{
			Tag lfmTag = n0->collider->GetTag();
			Tag rfmTag = n1->collider->GetTag();

			// ���� �浹�ϴ� �±����� Ȯ���Ѵ�.
			if (IsCollideTag(lfmTag, rfmTag) && n0->collider->GetEntity() != n1->collider->GetEntity())
			{
				// �浹�ϴ� �� Ȯ���Ѵ�. �浹�ϸ� �浹������ �����Ѵ�.
				Physics::Collision collision = n0->collider->IsCollide(n1->collider);
				if (collision.isCollide)
				{
					_collisions.push_back(collision);
				}
			}
			
		
		}
		// 1 branch / 1 leaf, 2 cross checks
		else
		{
			CrossChildren(n1);
			if (!n0->aabb.IsCollision(n1->aabb))
				return;

			ComputePairsHelper(n0, n1->children[0]);
			ComputePairsHelper(n0, n1->children[1]);
		}
	}
	else
	{
		// 1branch / l leaf, 2 cross checks
		if (n1->IsLeaf())
		{
			CrossChildren(n0);
			if (!n0->aabb.IsCollision(n1->aabb))
				return;

			ComputePairsHelper(n0->children[0], n1);
			ComputePairsHelper(n0->children[1], n1);
		}
		// 2 branches, 4 cross check
		else
		{
			CrossChildren(n0);
			CrossChildren(n1);

			// �ڽĳ��� Ȯ�����Ŀ� ���ΰ� �浹�ϸ� 4���� ���Ѵ�
			if (!n0->aabb.IsCollision(n1->aabb))
				return;

			ComputePairsHelper(n0->children[0], n1->children[0]);
			ComputePairsHelper(n0->children[0], n1->children[1]);
			ComputePairsHelper(n0->children[1], n1->children[0]);
			ComputePairsHelper(n0->children[1], n1->children[1]);
		}
	} // end of if (_n0->IsLeaf())

}

void ImpEngineModule::Physics::AABBTree::ClearChildrenCrossFlagHelper(Node* node)
{
	node->childrenCrossed = false;
	if (!node->IsLeaf())
	{
		ClearChildrenCrossFlagHelper(node->children[0]);
		ClearChildrenCrossFlagHelper(node->children[1]);
	}
}

void ImpEngineModule::Physics::AABBTree::CrossChildren(Node* node)
{
	if (!node->childrenCrossed)
	{
		ComputePairsHelper(node->children[0], node->children[1]);
		node->childrenCrossed = true;
	}
}

void ImpEngineModule::Physics::AABBTree::SetTagMap(const TagMap& tagMap)
{
	_tagMap = tagMap;
}

bool ImpEngineModule::Physics::AABBTree::IsCollideTag(Tag lfm, Tag rfm)
{
	if (_tagMap.tagCheck[static_cast<int>(lfm)][static_cast<int>(rfm)])
	{
		return true;
	}

	return false;
}
