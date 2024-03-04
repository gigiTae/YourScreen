#include "EngineModulePCH.h"
#include "CollisionInfomation.h"
#include "Collider.h"
#include "Collision.h"


ImpEngineModule::Physics::CollisionInfomation::CollisionInfomation(const Collision& collision)
	:prevCollision(false), currentcCollision(collision)
{
	Collider* leftCollider = collision.lfm;
	Collider* rightCollider = collision.rfm;

	unsigned int leftId = leftCollider->GetColliderID();
	unsigned int rightId = rightCollider->GetColliderID();

	colliderKey.colliderID1 = std::min(leftId, rightId);
	colliderKey.colliderID2 = std::max(leftId, rightId);
}
