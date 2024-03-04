#include "EngineModulePCH.h"
#include "BoxCollider.h"
#include "Entity.h"
#include "Transform.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Collide.h"

ImpEngineModule::BoxCollider::BoxCollider()
    : _offset(ImpMath::Vector3::Zero)
    , _node(nullptr)
    , _size{ 1.f, 1.f, 1.f }
    , _isTrigger(false)
    , _center{ 0.f, 0.f, 0.f }
    , _trasform(nullptr)
    , _isRotatable(false)
    , _colliderID(0)
    , _tag(Physics::Tag::Untagged)
    , _currentCollisionCollider(0)
{
}

ImpEngineModule::BoxCollider::~BoxCollider()
{
}

ImpEngineModule::Component* ImpEngineModule::BoxCollider::Clone(
    Component* clone /*= nullptr*/) const
{
    BoxCollider* box = static_cast<BoxCollider*>(clone);

    if (box == nullptr)
    {
        box = new BoxCollider(*this);
    }
    else
    {
        *box = *this;
    }
    box->_node = nullptr;

    return box;
}

void ImpEngineModule::BoxCollider::Start()
{
    _trasform = GetComponent<Transform>();
}

void ImpEngineModule::BoxCollider::OnCollsiionEnter(const Physics::Collision& collision)
{
    if (collision.lfm == this || collision.rfm == this)
    {
        ++_currentCollisionCollider;
    }

    assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::BoxCollider::OnCollsiionExit(const Physics::Collision& collision)
{
    if (collision.lfm == this || collision.rfm == this)
    {
        --_currentCollisionCollider;
    }
    assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::BoxCollider::OnTriggerEnter(const Physics::Collision& collision)
{
    if (collision.lfm == this || collision.rfm == this)
    {
        ++_currentCollisionCollider;
    }
    assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::BoxCollider::OnTriggerExit(const Physics::Collision& collision)
{
    if (collision.lfm == this || collision.rfm == this)
    {
        --_currentCollisionCollider;
    }
    assert(_currentCollisionCollider >= 0);
}

void ImpEngineModule::BoxCollider::GetMinMaxPoint(ImpMath::Vector3& min,
                                                  ImpMath::Vector3& max)
{
    using namespace ImpMath;

    // OBB
    if (_isRotatable)
    {
        Vector3 minPoint = Vector3(-_size * 0.5f);
        Vector3 maxPoint = Vector3(_size * 0.5f);

        Matrix rotM = Matrix::MakeFromQuaternion(_trasform->GetRotation());

        Vector3 v[8];
        v[0] = minPoint.Transform(rotM);
        v[1] = Vector3(minPoint.x, minPoint.y, maxPoint.z).Transform(rotM);
        v[2] = Vector3(minPoint.x, maxPoint.y, minPoint.z).Transform(rotM);
        v[3] = Vector3(minPoint.x, maxPoint.y, maxPoint.z).Transform(rotM);
        v[4] = Vector3(maxPoint.x, minPoint.y, minPoint.z).Transform(rotM);
        v[5] = Vector3(maxPoint.x, minPoint.y, maxPoint.z).Transform(rotM);
        v[6] = Vector3(maxPoint.x, maxPoint.y, minPoint.z).Transform(rotM);
        v[7] = maxPoint.Transform(rotM);

        min = v[0];
        max = v[0];

        for (int i = 1; i < 8; ++i)
        {
            min.x = std::min(min.x, v[i].x);
            min.y = std::min(min.y, v[i].y);
            min.z = std::min(min.z, v[i].z);

            max.x = std::max(max.x, v[i].x);
            max.y = std::max(max.y, v[i].y);
            max.z = std::max(max.z, v[i].z);
        }

        min += _center;
        max += _center;
    }
    else // AABB
    {
        min = ImpMath::Vector3(GetCenterPosition() - _size * 0.5f);
        max = ImpMath::Vector3(GetCenterPosition() + _size * 0.5f);
    }
}


ImpEngineModule::Physics::Collision ImpEngineModule::BoxCollider::IsCollide(
    Collider* other)
{
    ColliderType type = other->GetColliderType();
    if (type == ColliderType::Box)
    {
        return Physics::Collide(this, static_cast<BoxCollider*>(other));
    }
    else if (type == ColliderType::Sphere)
    {
        return Physics::Collide(this, static_cast<SphereCollider*>(other));
    }
    else if (type == ColliderType::Capsule)
    {
        return Physics::Collide(this, static_cast<CapsuleCollider*>(other));
    }

    return Physics::Collision{};
}

ImpEngineModule::ColliderType ImpEngineModule::BoxCollider::GetColliderType()
{
    return ColliderType::Box;
}

void ImpEngineModule::BoxCollider::SyncWithTransform()
{
    _center = _trasform->GetWorldPostiion() + _offset;
}

ImpMath::Quaternion ImpEngineModule::BoxCollider::GetRotation() const
{
    return _trasform->GetRotation();
}
