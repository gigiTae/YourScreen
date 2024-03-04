#include "EngineModulePCH.h"
#include "SphereCollider.h"
#include "Entity.h"
#include "Collide.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "Transform.h"
#include "CapsuleCollider.h"

ImpEngineModule::SphereCollider::SphereCollider()
	:_radius(1.f), _offset{}, _node(nullptr), _isTrigger(false), _transform(nullptr)
	, _tag(Physics::Tag::Untagged),_colliderID(0),_currentCollisionCollider(0)
{
}

ImpEngineModule::SphereCollider::~SphereCollider()
{

}

ImpEngineModule::Component* ImpEngineModule::SphereCollider::Clone(Component* clone /*= nullptr*/) const
{
	SphereCollider* sphere = static_cast<SphereCollider*>(clone);

	if (sphere == nullptr)
	{
		sphere = new SphereCollider(*this);
	}
	else
	{
		*sphere = *this;
	}
	sphere->_node = nullptr;

	return sphere;
}

void ImpEngineModule::SphereCollider::Start()
{
	_transform = GetComponent<Transform>();
}

void ImpEngineModule::SphereCollider::OnCollsiionEnter(const Physics::Collision& collision)
{
    if (collision.lfm == this || collision.rfm == this)
    {
        ++_currentCollisionCollider;
    }

	assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::SphereCollider::OnCollsiionExit(const Physics::Collision& collision)
{
    if (collision.lfm == this || collision.rfm == this)
    {
        --_currentCollisionCollider;
    }

	assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::SphereCollider::OnTriggerEnter(const Physics::Collision& collision)
{
    if (collision.lfm == this || collision.rfm == this)
    {
        ++_currentCollisionCollider;
    }
    assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::SphereCollider::OnTriggerExit(const Physics::Collision& collision)
{
    if (collision.lfm == this || collision.rfm == this)
    {
        --_currentCollisionCollider;
    }
    assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::SphereCollider::GetMinMaxPoint(ImpMath::Vector3& min, ImpMath::Vector3& max)
{
	min = _center - ImpMath::Vector3(_radius, _radius, _radius) + _offset;
	max = _center + ImpMath::Vector3(_radius, _radius, _radius) + _offset;
}

ImpEngineModule::Physics::Collision ImpEngineModule::SphereCollider::IsCollide(Collider* other)
{
	ColliderType type = other->GetColliderType();

	Physics::Collision collision;

	if (type == ColliderType::Sphere)
	{
		collision = Physics::Collide(this, static_cast<SphereCollider*>(other));
	}
	else if (type == ColliderType::Box)
	{
		collision = Physics::Collide(static_cast<BoxCollider*>(other), this);
	}
	else if (type == ColliderType::Capsule)
	{
        collision = Physics::Collide(this, static_cast<CapsuleCollider*>(other));
	}

	return collision;
}

ImpEngineModule::ColliderType ImpEngineModule::SphereCollider::GetColliderType()
{
	return ColliderType::Sphere;
}

void ImpEngineModule::SphereCollider::SyncWithTransform()
{
	// 중심좌표 이동
	_center = _transform->GetWorldPostiion() + _offset;
}
