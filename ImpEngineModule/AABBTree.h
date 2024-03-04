#pragma once

#include "Broadphase.h"
#include "TagMap.h"

namespace ImpEngineModule::Physics
{
	struct Node;

	/// <summary>
	/// �浹üũ �˰��� AABBTree
	/// ������ �ݶ��̴��� AABB�簢������ Ȯ���ϰ�
	/// Ȯ���� �簢���� margin��ŭ Ȯ���� �Ŀ� 
	/// ���ǿ����� Node�� �з��Ѵ�.
	/// </summary>
	class AABBTree :public IBroadphase
	{
	public:
		AABBTree(float margin);
		~AABBTree();

		// ���ο� �ݶ��̴� �߰�
		void Add(Collider* collider)override;
		
		// �ݶ��̴� ����
		void Remove(Collider* collider) override;

		// AABBƮ���� ������ �����Ѵ�.
		void Update() override;

		// �̹� �������� �浹������ ����Ѵ�.
		const CollisionVector& ComputeCollisions()override;

		// TagMap�� �����Ѵ�.
		void SetTagMap(const TagMap& tagMap)override;

		// AABBƮ�� �ʱ�ȭ
		void Clear() override;

	private:
		typedef std::vector<Node*> NodeVector;
		void UpdateNodeHelper(Node* node);
		void InsertNode(Node* node, Node** _parent);
		void RemoveNode(Node* node);
		void ComputePairsHelper(Node* n0, Node* n1);
		void ClearChildrenCrossFlagHelper(Node* node);
		void CrossChildren(Node* node);
		// ���� �浹�ϴ� Tag���� 
		bool IsCollideTag(Tag lfm, Tag rfm);

	private:
		TagMap _tagMap;
		Node* _root;
		CollisionVector _collisions;
		float _margin;

		NodeVector _invalidNodes;
		NodeVector _removeNodes;
	};


}