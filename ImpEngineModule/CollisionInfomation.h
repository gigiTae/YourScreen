#pragma once

#include "Collision.h"

namespace ImpEngineModule
{
	class Collider;
}


namespace ImpEngineModule::Physics
{
	struct Collision;

	// CallBack �̺�Ʈ�� ���� ������ �浹 ���� 
	struct CollisionInfomation
	{
		CollisionInfomation(const Collision& collision);

		union ColliderKey
		{
			unsigned long long key;

			struct
			{
				unsigned int colliderID1;
				unsigned int colliderID2;
			};
		};

		ColliderKey colliderKey;
		bool prevCollision; // ���� ������ �浹 
		Collision currentcCollision; // �̹� ������ �浹
	};

}