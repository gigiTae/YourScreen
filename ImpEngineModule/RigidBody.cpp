#include "EngineModulePCH.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Entity.h"
#include "World.h"
#include "InputManager.h"
#include "Collision.h"
#include "Collider.h"

ImpEngineModule::Rigidbody::Rigidbody()
    : _mass(1.f)
    , _hasGravity(true)
    , _force(0.f, 0.f, 0.f)
    , _velocity(0.f, 0.f, 0.f)
    , _isKinematic(false)
    , _transform(nullptr)
    , _inverseMass(0.f)
    , _acmulatePushDistance{0.f,0.f,0.f}
    , _acmulateCount(0)
{
}

ImpEngineModule::Rigidbody::~Rigidbody()
{
}

ImpEngineModule::Component* ImpEngineModule::Rigidbody::Clone(
    Component* clone /*= nullptr*/) const
{
    Rigidbody* rigid = static_cast<Rigidbody*>(clone);

    if (rigid == nullptr)
    {
        rigid = new Rigidbody(*this);
    }
    else
    {
        *rigid = *this;
    }

    return rigid;
}

void ImpEngineModule::Rigidbody::Start()
{
    _transform = GetComponent<Transform>();

    _inverseMass = 1 / _mass;
}

void ImpEngineModule::Rigidbody::AddForce(ImpMath::Vector3 force)
{
    _force += force;
}

void ImpEngineModule::Rigidbody::AddVelocity(ImpMath::Vector3 velocity)
{
    _velocity = _velocity + velocity;
}

void ImpEngineModule::Rigidbody::UpdateVelocity(float dt)
{
    // v = F*dt/m
    _velocity += _force * dt * _inverseMass;

    _force = ImpMath::Vector3::Zero;
}

void ImpEngineModule::Rigidbody::UpdatePosition(float dt)
{
    if (!IsKinematic())
    {
        _transform->AddPosition(_velocity * dt);
    }
}

void ImpEngineModule::Rigidbody::SetMass(float mass)
{
    _mass        = mass;
    _inverseMass = 1 / _mass;
}

void ImpEngineModule::Rigidbody::AcmulatePushDistance(ImpMath::Vector3 distance)
{
    if (distance == ImpMath::Vector3::Zero)
    {
        return;
    }

    _acmulatePushDistance += distance;
    ++_acmulateCount;
}

void ImpEngineModule::Rigidbody::SolveAcmulateDistance()
{
    if (_acmulateCount == 0)
    {
        return;
    }

    _acmulatePushDistance /= _acmulateCount;
    _transform->AddPosition(_acmulatePushDistance);
    
    _acmulatePushDistance = ImpMath::Vector3::Zero;
    _acmulateCount        = 0;
}

//void ImpEngineModule::Rigidbody::OnCollsiionEnter(const Physics::Collision& collision)
//{
//    ImpMath::Vector3 normal = collision.separationNormal;
//
//    if (collision.lfm->GetEntity() == GetEntity())
//    {
//        normal *= -1.f;
//    }
//
//    ImpMath::Vector3 yDown{ 0.f, -1.f, 0.f };
//    float angle = std::acos(yDown.Dot(normal));
//
//    if (angle <= _slipAngle)
//    {
//        _onGround = true;
//    }
//}

