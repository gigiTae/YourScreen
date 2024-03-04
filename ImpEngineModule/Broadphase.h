#pragma once

#include "Collision.h"

namespace ImpEngineModule
{
	class Collider;
}

namespace ImpEngineModule::Physics
{
	struct TagMap;

	/// <summary>
	/// 광역 충돌체크 인터페이스
	/// </summary>
	class IBroadphase abstract
	{
	public:
		typedef std::pair<Collider*, Collider*> ColliderPair;
		typedef std::vector<Collision> CollisionVector;
		typedef std::list<ColliderPair> ColliderPairList;
		typedef std::vector<Collider*> ColliderVector;

		virtual ~IBroadphase() {}

		// Collider를 추가한다
		virtual void Add(Collider* collider)abstract;

		// Collider 삭제
		virtual void Remove(Collider* collider)abstract;

		// Collider들의 위치정보를 갱신한다.
		virtual void Update() abstract;

		// Collider들의 충돌정보를 가져온다.
		virtual const CollisionVector& ComputeCollisions()abstract;

		// TagMap을 설정한다.
		virtual void SetTagMap(const TagMap& tagMap)abstract;

		// 초기화
		virtual void Clear()abstract;
	};



}