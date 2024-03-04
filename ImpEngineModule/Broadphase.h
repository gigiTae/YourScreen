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
	/// ���� �浹üũ �������̽�
	/// </summary>
	class IBroadphase abstract
	{
	public:
		typedef std::pair<Collider*, Collider*> ColliderPair;
		typedef std::vector<Collision> CollisionVector;
		typedef std::list<ColliderPair> ColliderPairList;
		typedef std::vector<Collider*> ColliderVector;

		virtual ~IBroadphase() {}

		// Collider�� �߰��Ѵ�
		virtual void Add(Collider* collider)abstract;

		// Collider ����
		virtual void Remove(Collider* collider)abstract;

		// Collider���� ��ġ������ �����Ѵ�.
		virtual void Update() abstract;

		// Collider���� �浹������ �����´�.
		virtual const CollisionVector& ComputeCollisions()abstract;

		// TagMap�� �����Ѵ�.
		virtual void SetTagMap(const TagMap& tagMap)abstract;

		// �ʱ�ȭ
		virtual void Clear()abstract;
	};



}