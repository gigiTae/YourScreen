#include "EngineModulePCH.h"
#include "AABB.h"
#include "Collider.h"


ImpEngineModule::Physics::AABB::AABB()
	:minPoint(ImpMath::Vector3::Zero), maxPoint(ImpMath::Vector3::Zero)
{}


ImpEngineModule::Physics::AABB::AABB(const ImpMath::Vector3& min, const ImpMath::Vector3& max)
	:minPoint(min), maxPoint(max)
{}


float ImpEngineModule::Physics::AABB::GetVolume() const
{
	using namespace ImpMath;

	Vector3 length = (maxPoint - minPoint);

	return length.x * length.y * length.z;
}

bool ImpEngineModule::Physics::AABB::IsCollision(const AABB& other) const
{
	if (minPoint.x <= other.maxPoint.x && maxPoint.x >= other.minPoint.x
		&& minPoint.y <= other.maxPoint.y && maxPoint.y >= other.minPoint.y
		&& minPoint.z <= other.maxPoint.z && maxPoint.z >= other.minPoint.z)
	{
		return true;
	}

	return false;
}

ImpEngineModule::Physics::AABB ImpEngineModule::Physics::AABB::Union(const AABB& lfm, const AABB& rfm)
{
	ImpMath::Vector3 min{ std::min(lfm.minPoint.x,rfm.minPoint.x),
		std::min(lfm.minPoint.y,rfm.minPoint.y),
		std::min(lfm.minPoint.z,rfm.minPoint.z) };

	ImpMath::Vector3 max{ std::max(lfm.maxPoint.x,rfm.maxPoint.x),
		std::max(lfm.maxPoint.y,rfm.maxPoint.y) ,
		std::max(lfm.maxPoint.z,rfm.maxPoint.z) };

	return AABB(min, max);
}

bool ImpEngineModule::Physics::AABB::Contains(Collider* collider) const
{
	ImpMath::Vector3 colliderMax;
	ImpMath::Vector3 colliderMin;

	collider->GetMinMaxPoint(colliderMin, colliderMax);

	if (minPoint.x <= colliderMin.x && minPoint.y <= colliderMin.y && minPoint.z <= colliderMin.z
		&& maxPoint.x >= colliderMax.x && maxPoint.y >= colliderMax.y && maxPoint.z >= colliderMax.z)
	{
		return true;
	}

	return false;
}
