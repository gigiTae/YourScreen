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
/// 충돌을 처리하는 매니져
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
    // 새로운 콜라이더 추가
    void AddCollider(Collider* collider);
    // 콜라이더 삭제
    void RemoveCollider(Collider* collider);

    // Collider가 있으면 콜라이더를 추가한다.
    void Receive(ImpEngineModule::World* world,
                 const ImpEngineModule::Event::OnEntityStarted& event) override;

    // Collider가 있으면 콜라이더를 제거한다.
    void Receive(ImpEngineModule::World* world,
                 const ImpEngineModule::Event::OnEntityDestroyed& event) override;

    // Entity들에게 충돌이벤트를 전달한다.
    void CallCollisionEvent(const std::vector<Physics::Collision>& collisions , float dt);

private:
    unsigned int _lastColliderID;

    EventManager* _eventManager;
    World* _world;
    std::unique_ptr<Physics::IBroadphase> _broadphase;
    std::unique_ptr<Physics::Solver> _solver;

    std::unordered_map<unsigned long long, Physics::CollisionInfomation>
        _collisionInfomations; // 프레임 충돌정보
};


} // namespace ImpEngineModule