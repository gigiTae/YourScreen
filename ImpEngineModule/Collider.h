#pragma once
#include "Component.h"


namespace ImpEngineModule::Physics
{
	struct Collision;
	struct Node;
}

namespace ImpEngineModule
{

	/// <summary>
	/// �浹ü�� �����ϴ� �������̽� 
	/// </summary>
	class Collider :public Component
	{
	public:
		virtual void GetMinMaxPoint(ImpMath::Vector3 & min, ImpMath::Vector3& max) abstract;

		virtual Physics::Collision IsCollide(Collider* other) abstract;
		virtual ColliderType GetColliderType() abstract;
		virtual void SetNode(Physics::Node* node)abstract;
		virtual Physics::Node* GetNode() abstract;

		virtual unsigned int GetColliderID()const abstract;
		// Collider ID�� �����Ѵ�.
		virtual void SetColliderID(unsigned int id) abstract;

		virtual bool IsTrigger()const abstract;
		// Collider�� ��ġ�� Trasfrom�� ����ȭ�Ѵ�.
		virtual void SyncWithTransform() abstract;

		virtual ImpEngineModule::Physics::Tag GetTag() const abstract;
		// Tag ����
		virtual void SetTag(ImpEngineModule::Physics::Tag tag)abstract;
	};


}