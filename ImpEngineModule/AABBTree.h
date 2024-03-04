#pragma once

#include "Broadphase.h"
#include "TagMap.h"

namespace ImpEngineModule::Physics
{
	struct Node;

	/// <summary>
	/// 충돌체크 알고리즘 AABBTree
	/// 각각의 콜라이더를 AABB사각형으로 확장하고
	/// 확장한 사각형을 margin만큼 확장한 후에 
	/// 부피에따라서 Node를 분류한다.
	/// </summary>
	class AABBTree :public IBroadphase
	{
	public:
		AABBTree(float margin);
		~AABBTree();

		// 새로운 콜라이더 추가
		void Add(Collider* collider)override;
		
		// 콜라이더 삭제
		void Remove(Collider* collider) override;

		// AABB트리의 노드들을 갱신한다.
		void Update() override;

		// 이번 프레임의 충돌정보를 계산한다.
		const CollisionVector& ComputeCollisions()override;

		// TagMap을 설정한다.
		void SetTagMap(const TagMap& tagMap)override;

		// AABB트리 초기화
		void Clear() override;

	private:
		typedef std::vector<Node*> NodeVector;
		void UpdateNodeHelper(Node* node);
		void InsertNode(Node* node, Node** _parent);
		void RemoveNode(Node* node);
		void ComputePairsHelper(Node* n0, Node* n1);
		void ClearChildrenCrossFlagHelper(Node* node);
		void CrossChildren(Node* node);
		// 서로 충돌하는 Tag인지 
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