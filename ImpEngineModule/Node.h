#pragma once

#include "AABB.h"
namespace ImpEngineModule 
{
	class Collider;
}
namespace ImpEngineModule::Physics
{
	struct Node
	{
		Node();

		// 리프노드인지
		bool IsLeaf()const { return children[0] == nullptr; }

		void SetBranch(Node* child0, Node* child1)
		{
			child0->parent = this;
			child1->parent = this;

			children[0] = child0;
			children[1] = child1;
		}

		void UpdateAABB(float margin);

		Node* GetSibling()const
		{
			assert(parent);
			return this == parent->children[0] ? parent->children[1] : parent->children[0];
		}

		// Leaf노드로 만든다
		void SetLeaf(Collider* collider)
		{
			this->collider = collider;
			children[0] = nullptr;
			children[1] = nullptr;
		}

		Node* parent;
		Node* children[2];

		bool childrenCrossed;
		AABB aabb;
		Collider* collider;
	};


}