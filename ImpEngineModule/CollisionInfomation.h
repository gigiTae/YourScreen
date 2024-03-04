#pragma once

#include "Collision.h"

namespace ImpEngineModule
{
	class Collider;
}


namespace ImpEngineModule::Physics
{
	struct Collision;

	// CallBack 이벤트를 위한 프레임 충돌 정보 
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
		bool prevCollision; // 이전 프레임 충돌 
		Collision currentcCollision; // 이번 프레임 충돌
	};

}