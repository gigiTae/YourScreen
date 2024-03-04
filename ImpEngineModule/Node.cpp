#include "EngineModulePCH.h"
#include "Node.h"
#include "Collider.h"

ImpEngineModule::Physics::Node::Node()
	:parent(nullptr), collider(nullptr),
	children{ nullptr,nullptr }, childrenCrossed(false), aabb{}
{

}

void ImpEngineModule::Physics::Node::UpdateAABB(float margin)
{
	if (IsLeaf())
	{
		// Ȯ���� AABB ����
		ImpMath::Vector3 marginVec3(margin, margin, margin);

		collider->GetMinMaxPoint(aabb.minPoint, aabb.maxPoint);

		aabb.minPoint -= marginVec3;
		aabb.maxPoint += marginVec3;
	}
	else
	{
		// �ڽĳ�带 �����ؼ� AABB�����
		aabb = AABB::Union(children[0]->aabb, children[1]->aabb);
	}
}
