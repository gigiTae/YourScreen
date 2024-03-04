#pragma once
#include "Collider.h"

namespace ImpEngineModule::Physics
{
 	struct Node;
}

namespace ImpEngineModule
{
	class Transform;

	/// <summary>
	/// 구형 콜라이더
	/// </summary>
	class SphereCollider :	public Collider
	{
	public:
		SphereCollider();
		~SphereCollider();

		Component* Clone(Component* clone = nullptr)const override;

		void Start() override;

		void OnCollsiionEnter(const Physics::Collision& collision) override;
		void OnCollsiionExit(const Physics::Collision& collision) override;

		void OnTriggerEnter(const Physics::Collision& collision) override;
		void OnTriggerExit(const Physics::Collision& collision) override;

		// 구의 중심좌표 반환 
		ImpMath::Vector3 GetCenterPosition()const { return _center; }
	public:
		// 최소,최대점 반환
		void GetMinMaxPoint(ImpMath::Vector3& min, ImpMath::Vector3& max) override;
	
		// 현재 충돌중인 콜라이더 갯수 반환
		int GetCurrentCollisionCollider() const { return _currentCollisionCollider; }
		
		// 충돌 판단 함수
		Physics::Collision IsCollide(Collider* other) override;
		// 충돌체 타입
		ColliderType GetColliderType() override;

		// Collider의 위치를 Trasfrom과 동기화한다.
		void SyncWithTransform() override;

		bool IsTrigger() const override{ return _isTrigger; }
		void SetTrigger(bool isTrigger) { _isTrigger = isTrigger; }

		float GetRadius() const { return _radius; }
		void SetoRadius(float radius) { _radius = radius; }

		ImpMath::Vector3 GetOffset() const { return _offset; }
		void SetOffset(ImpMath::Vector3 offset) { _offset = offset; }

		ImpEngineModule::Physics::Tag GetTag() const { return _tag; }
		void SetTag(ImpEngineModule::Physics::Tag val) { _tag = val; }

	private:
		void SetColliderID(unsigned int id)override { _colliderID = id; }
		unsigned int GetColliderID()const override { return _colliderID; }
		void SetNode(Physics::Node* node)override { _node = node; }
		Physics::Node* GetNode() override { return _node; }
	private:
		Physics::Tag _tag;
		Physics::Node* _node;
		unsigned int _colliderID;
		int _currentCollisionCollider; // 현재 충돌중인 충돌체 갯수

		ImpMath::Vector3 _center; // 구의 중심
		float _radius; // 구의 반지름
 		ImpMath::Vector3 _offset; 

		bool _isTrigger;

		Transform* _transform;
		REFLECTION_FRIEND(SphereCollider)
	};


}