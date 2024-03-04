#include "EngineModulePCH.h"
#include "CapsuleCollider.h"
#include "Transform.h"
#include "Collision.h"
#include "Entity.h"
#include "Collide.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

ImpEngineModule::CapsuleCollider::CapsuleCollider()
    : _center()
    , _node(nullptr)
    , _tag(Physics::Tag::Untagged)
    , _currentCollisionCollider(0)
    , _height(1.f)
    , _radius(1.f)
    , _trasform(nullptr)
    , _colliderID(0)
    , _isRotatable(false)
    , _isTrigger(false)
{
}

ImpEngineModule::CapsuleCollider::~CapsuleCollider()
{
}

ImpEngineModule::Component* ImpEngineModule::CapsuleCollider::Clone(
    Component* clone /*= nullptr*/) const
{
    CapsuleCollider* capsule = static_cast<CapsuleCollider*>(clone);

    if (capsule == nullptr)
    {
        capsule = new CapsuleCollider(*this);
    }
    else
    {
        *capsule = *this;
    }
    capsule->_node = nullptr;

    return capsule;
}

void ImpEngineModule::CapsuleCollider::Start()
{
    _trasform = GetComponent<Transform>();
}

void ImpEngineModule::CapsuleCollider::OnCollsiionEnter(const Physics::Collision& collision)
{
    ++_currentCollisionCollider;
    assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::CapsuleCollider::OnCollsiionExit(const Physics::Collision& collision)
{
    --_currentCollisionCollider;
    assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::CapsuleCollider::OnTriggerEnter(const Physics::Collision& collision)
{
    ++_currentCollisionCollider;
    assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::CapsuleCollider::OnTriggerExit(const Physics::Collision& collision)
{
    --_currentCollisionCollider;
    assert(_currentCollisionCollider >= 0);
}


void ImpEngineModule::CapsuleCollider::GetTopBottomPoint(ImpMath::Vector3& bottom,
                                                         ImpMath::Vector3& top) const
{
    using namespace ImpMath;
    Matrix rotM = Matrix::MakeFromQuaternion(GetRotation());

    if (_isRotatable)
    {
        top    = _center + Vector3(0.f, _height, 0.f).Transform(rotM);
        bottom = _center + Vector3(0.f, -_height, 0.f).Transform(rotM);
    }
    else
    {
        top    = _center + Vector3(0.f, _height, 0.f);
        bottom = _center - Vector3(0.f, _height, 0.f);
    }
}

void ImpEngineModule::CapsuleCollider::GetMinMaxPoint(ImpMath::Vector3& min,
                                                      ImpMath::Vector3& max)
{
    if (!IsRotatable())
    {
        min = _center + ImpMath::Vector3(-_radius, -_radius - _height, -_radius);
        max = _center + ImpMath::Vector3(_radius, _radius + _height, _radius);
    }
    else
    {
        min = _center + ImpMath::Vector3(-_radius - _height,
                                         -_radius - _height,
                                         -_radius - _height);

        max = _center +
              ImpMath::Vector3(_radius + _height, _radius + _height, _radius + _height);
    }
}

ImpEngineModule::Physics::Collision ImpEngineModule::CapsuleCollider::IsCollide(
    Collider* other)
{
    ColliderType type = other->GetColliderType();
    if (type == ColliderType::Box)
    {
        return Physics::Collide(static_cast<BoxCollider*>(other), this);
    }
    else if (type == ColliderType::Sphere)
    {
        return Physics::Collide(static_cast<SphereCollider*>(other), this);
    }
    else if (type == ColliderType::Capsule)
    {
        return Physics::Collide(this, static_cast<CapsuleCollider*>(other));
    }

    return Physics::Collision{};
}

ImpEngineModule::ColliderType ImpEngineModule::CapsuleCollider::GetColliderType()
{
    return ColliderType::Capsule;
}

void ImpEngineModule::CapsuleCollider::SyncWithTransform()
{
    _center = _trasform->GetWorldPostiion() + _offset;
}

ImpMath::Quaternion ImpEngineModule::CapsuleCollider::GetRotation() const
{
    return _trasform->GetRotation();
}
