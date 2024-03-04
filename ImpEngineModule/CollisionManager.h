#pragma once

#include "CallBackEvent.h"
#include "EventSubscriber.h"
#include "Collision.h"
#include "CollisionInfomation.h"

namespace ImpEngineModule::Physics
{
class Solver;
class IBroadphase;
struct TagMap;
} // namespace ImpEngineModule::Physics

namespace ImpEngineModule
{
class EventManager;
class Collider;
class World;

/// <summary>
/// �浹�� ó���ϴ� �Ŵ���
/// </summary>
class CollisionManager:
    public EventSubscriber<Event::OnEntityDestroyed>,
    public EventSubscriber<Event::OnEntityStarted>
{
private:
    static constexpr float AABBTreeMargin = 1.f;
    static constexpr float WorldGravity   = -50.f;

public:
    CollisionManager();
    ~CollisionManager();

    void Initialize(EventManager* eventManager, World* world);

    void Reset();

    void Update(float dt);

    void Finalize();

    void SetTagMap(const Physics::TagMap& tagMap);

private:
    // ���ο� �ݶ��̴� �߰�
    void AddCollider(Collider* collider);
    // �ݶ��̴� ����
    void RemoveCollider(Collider* collider);

    // Collider�� ������ �ݶ��̴��� �߰��Ѵ�.
    void Receive(ImpEngineModule::World* world,
                 const ImpEngineModule::Event::OnEntityStarted& event) override;

    // Collider�� ������ �ݶ��̴��� �����Ѵ�.
    void Receive(ImpEngineModule::World* world,
                 const ImpEngineModule::Event::OnEntityDestroyed& event) override;

    // Entity�鿡�� �浹�̺�Ʈ�� �����Ѵ�.
    void CallCollisionEvent(const std::vector<Physics::Collision>& collisions , float dt);

private:
    unsigned int _lastColliderID;

    EventManager* _eventManager;
    World* _world;
    std::unique_ptr<Physics::IBroadphase> _broadphase;
    std::unique_ptr<Physics::Solver> _solver;

    std::unordered_map<unsigned long long, Physics::CollisionInfomation>
        _collisionInfomations; // ������ �浹����
};


} // namespace ImpEngineModule