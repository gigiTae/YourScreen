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
	/// 충돌체를 구성하는 인터페이스 
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
		// Collider ID를 설정한다.
		virtual void SetColliderID(unsigned int id) abstract;

		virtual bool IsTrigger()const abstract;
		// Collider의 위치를 Trasfrom과 동기화한다.
		virtual void SyncWithTransform() abstract;

		virtual ImpEngineModule::Physics::Tag GetTag() const abstract;
		// Tag 설정
		virtual void SetTag(ImpEngineModule::Physics::Tag tag)abstract;
	};


}