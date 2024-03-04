#pragma once
#include "Component.h"


namespace ImpEngineModule
{
class Transform;

/// <summary>
/// �������� ó���� �ϴ� ������Ʈ
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

    // ���� ���� ���ؼ� �ӵ��� ����Ѵ�.
    void UpdateVelocity(float dt);

    // ���� �ӵ��� ���ؼ� ��ġ�� ����Ѵ�.
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


    /// �浹ó���������� �⵿��Ĩ�� �ذ��ϱ����� �Լ�
    void AcmulatePushDistance(ImpMath::Vector3 distance);

    void SolveAcmulateDistance();

private:
    bool _isKinematic;
    bool _hasGravity;           // �߷�
    float _mass;                // ����
    float _inverseMass;         // 1/����
    ImpMath::Vector3 _velocity; // �ӵ�
    ImpMath::Vector3 _force;    // ��

    bool _onGround = false;
    float _linearDamping = 1.f;                      // ������ ����
    float _slipAngle     = ImpMath::QuaterPI * 0.5f; // �̲������� ����

    Transform* _transform;

    ImpMath::Vector3 _acmulatePushDistance;
    unsigned int _acmulateCount;

    REFLECTION_FRIEND(Rigidbody)
};


} // namespace ImpEngineModule