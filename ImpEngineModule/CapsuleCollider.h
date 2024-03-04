#pragma once
#include "Collider.h"

namespace ImpEngineModule
{
class Transform;

/// <summary>
/// Capsule 모양 콜라이더
/// </summary>
class CapsuleCollider: public Collider
{
public:
    CapsuleCollider();
    ~CapsuleCollider();

    Component* Clone(Component* clone = nullptr) const override;

    void Start() override;

    void OnCollsiionEnter(const Physics::Collision& collision) override;
    void OnCollsiionExit(const Physics::Collision& collision) override;

    void OnTriggerEnter(const Physics::Collision& collision) override;
    void OnTriggerExit(const Physics::Collision& collision) override;

	bool IsRotatable() const
    {
        return _isRotatable;
    }
    void SetRotatable(bool isRotatable)
    {
        _isRotatable = isRotatable;
    }

	float GetHeight() const
    {
        return _height;
    }
    void SetHeight(float height)
    {
        _height = height;
    }

	float GetRadius() const
    {
        return _radius;
    }
    void SetRadius(float radius)
    {
        _radius = radius;
    }

	ImpMath::Vector3 GetCenterPosition() const
    {
        return _center;
    }

	int GetCurrentCollisionCollider() const
    {
        return _currentCollisionCollider;
    }

    
    void GetTopBottomPoint(ImpMath::Vector3& bottom, ImpMath::Vector3& top)const;

public:
    void GetMinMaxPoint(ImpMath::Vector3& min, ImpMath::Vector3& max) override;

    Physics::Collision IsCollide(Collider* other) override;

    ColliderType GetColliderType() override;

    void SyncWithTransform() override;

    bool IsTrigger() const override
    {
        return _isTrigger;
    }
    void SetTrigger(bool isTrigger)
    {
        _isTrigger = isTrigger;
    }

    ImpMath::Quaternion GetRotation() const;

    ImpMath::Vector3 GetOffset() const
    {
        return _offset;
    }
    void SetOffset(ImpMath::Vector3 offset)
    {
        _offset = offset;
    }

    ImpEngineModule::Physics::Tag GetTag() const override
    {
        return _tag;
    }
    void SetTag(ImpEngineModule::Physics::Tag val) override
    {
        _tag = val;
    }

private:
    void SetNode(Physics::Node* node) override
    {
        _node = node;
    }
    Physics::Node* GetNode() override
    {
        return _node;
    }

    void SetColliderID(unsigned int id) override
    {
        _colliderID = id;
    }
    unsigned int GetColliderID() const override
    {
        return _colliderID;
    }

private:
    Physics::Tag _tag;
    bool _isTrigger;
    bool _isRotatable;

    ImpMath::Vector3 _offset;
    ImpMath::Vector3 _center;
    float _radius;
    float _height;

    unsigned int _colliderID;
    int _currentCollisionCollider;

    Physics::Node* _node;
    Transform* _trasform;

    REFLECTION_FRIEND(CapsuleCollider)
};

} // namespace ImpEngineModule