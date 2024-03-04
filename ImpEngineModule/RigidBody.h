#pragma once
#include "Component.h"


namespace ImpEngineModule
{
class Transform;

/// <summary>
/// 물리적인 처리를 하는 컴포넌트
/// </summary>
class Rigidbody: public Component
{
public:
    Rigidbody();
    ~Rigidbody();

    Component* Clone(Component* clone = nullptr) const override;

    void Start() override;

    void AddForce(ImpMath::Vector3 force);
    void AddVelocity(ImpMath::Vector3 velocity);

    // 현재 힘을 통해서 속도를 계산한다.
    void UpdateVelocity(float dt);

    // 현재 속도를 통해서 위치를 계산한다.
    void UpdatePosition(float dt);

    float GetMass() const
    {
        return _mass;
    }
    void SetMass(float mass);

    bool IsKinematic() const
    {
        return _isKinematic;
    }
    void SetKinematic(bool isKinematic)
    {
        _isKinematic = isKinematic;
    }

    bool HasGravity() const
    {
        return _hasGravity;
    }
    void SetGravity(bool hasGravity)
    {
        _hasGravity = hasGravity;
    }

    ImpMath::Vector3 GetVelocity() const
    {
        return _velocity;
    }
    void SetVelocity(ImpMath::Vector3 velocity)
    {
        _velocity = velocity;
    }

    float GetLinearDamping() const
    {
        return _linearDamping;
    }
    void SetLinearDamping(float val)
    {
        _linearDamping = val;
    }

    float GetSlipAngle() const
    {
        return _slipAngle;
    }
    void SetSlipAngle(float slipAngle)
    {
        _slipAngle = slipAngle;
    }

    bool OnGround() const
    {
        return _onGround;
    }

    void SetOnGround(bool onGround)
    {
        _onGround = onGround;
    }


    /// 충돌처리과정에서 출동겹칩을 해결하기위한 함수
    void AcmulatePushDistance(ImpMath::Vector3 distance);

    void SolveAcmulateDistance();

private:
    bool _isKinematic;
    bool _hasGravity;           // 중력
    float _mass;                // 질량
    float _inverseMass;         // 1/질량
    ImpMath::Vector3 _velocity; // 속도
    ImpMath::Vector3 _force;    // 힘

    bool _onGround = false;
    float _linearDamping = 1.f;                      // 마찰력 개념
    float _slipAngle     = ImpMath::QuaterPI * 0.5f; // 미끄러지는 각도

    Transform* _transform;

    ImpMath::Vector3 _acmulatePushDistance;
    unsigned int _acmulateCount;

    REFLECTION_FRIEND(Rigidbody)
};


} // namespace ImpEngineModule