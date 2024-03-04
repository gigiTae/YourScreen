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
/// Box형태의 충돌체
///
/// </summary>
class BoxCollider: public Collider
{
public:
    BoxCollider();
    ~BoxCollider();

    Component* Clone(Component* clone = nullptr) const override;

    void Start() override;

    void OnCollsiionEnter(const Physics::Collision& collision) override;
    void OnCollsiionExit(const Physics::Collision& collision) override;

    void OnTriggerEnter(const Physics::Collision& collision) override;
    void OnTriggerExit(const Physics::Collision& collision) override;

public:
    void GetMinMaxPoint(ImpMath::Vector3& min, ImpMath::Vector3& max) override;

    Physics::Collision IsCollide(Collider* other) override;
    ColliderType GetColliderType() override;

    // 현재 충돌중인 콜라이더
    int GetCurrentCollisionCollider() const
    {
        return _currentCollisionCollider;
    }

    // Box의 중심 좌표 반환
    ImpMath::Vector3 GetCenterPosition() const
    {
        return _center;
    }
    // Collider의 위치를 Trasfrom과 동기화한다.
    void SyncWithTransform() override;

    bool IsTrigger() const override
    {
        return _isTrigger;
    }
    void SetTrigger(bool isTrigger)
    {
        _isTrigger = isTrigger;
    }

    ImpMath::Vector3 GetSize() const
    {
        return _size;
    }
    void SetSize(ImpMath::Vector3 size)
    {
        _size = size;
    }

    ImpMath::Vector3 GetOffset() const
    {
        return _offset;
    }
    void SetOffset(ImpMath::Vector3 offset)
    {
        _offset = offset;
    }

    ImpEngineModule::Physics::Tag GetTag() const
    {
        return _tag;
    }
    void SetTag(ImpEngineModule::Physics::Tag val)
    {
        _tag = val;
    }


#pragma region rotation
    bool IsRotatable() const
    {
        return _isRotatable;
    }
    void SetRotatable(bool isRotatable)
    {
        _isRotatable = isRotatable;
    }

    ImpMath::Quaternion GetRotation() const;
#pragma endregion rotation

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
    bool _isRotatable; // true : OBB , false : AABBB

    ImpMath::Vector3 _center;
    ImpMath::Vector3 _size;
    ImpMath::Vector3 _offset;
    bool _isTrigger;

    unsigned int _colliderID;
    int _currentCollisionCollider; // 현재 충돌중인 콜라이더 갯수

    Physics::Node* _node;
    Transform* _trasform;

    REFLECTION_FRIEND(BoxCollider)
};


} // namespace ImpEngineModule