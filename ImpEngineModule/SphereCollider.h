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
	/// ���� �ݶ��̴�
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

		// ���� �߽���ǥ ��ȯ 
		ImpMath::Vector3 GetCenterPosition()const { return _center; }
	public:
		// �ּ�,�ִ��� ��ȯ
		void GetMinMaxPoint(ImpMath::Vector3& min, ImpMath::Vector3& max) override;
	
		// ���� �浹���� �ݶ��̴� ���� ��ȯ
		int GetCurrentCollisionCollider() const { return _currentCollisionCollider; }
		
		// �浹 �Ǵ� �Լ�
		Physics::Collision IsCollide(Collider* other) override;
		// �浹ü Ÿ��
		ColliderType GetColliderType() override;

		// Collider�� ��ġ�� Trasfrom�� ����ȭ�Ѵ�.
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
		int _currentCollisionCollider; // ���� �浹���� �浹ü ����

		ImpMath::Vector3 _center; // ���� �߽�
		float _radius; // ���� ������
 		ImpMath::Vector3 _offset; 

		bool _isTrigger;

		Transform* _transform;
		REFLECTION_FRIEND(SphereCollider)
	};


}