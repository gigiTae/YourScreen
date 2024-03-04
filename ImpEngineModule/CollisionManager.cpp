#include "EngineModulePCH.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "Entity.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "AABBTree.h"
#include "World.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Solver.h"

ImpEngineModule::CollisionManager::CollisionManager()
    : _eventManager(nullptr)
    , _world(nullptr)
    , _lastColliderID(0)
    , _collisionInfomations{}
{
}

ImpEngineModule::CollisionManager::~CollisionManager()
{
}


void ImpEngineModule::CollisionManager::Initialize(EventManager* eventManager,
    World* world)
{
    _world      = world;
    _broadphase = std::make_unique<Physics::AABBTree>(AABBTreeMargin);
    _solver     = std::make_unique<Physics::Solver>();

    _eventManager = eventManager;

    eventManager->SubScribe<Event::OnEntityStarted>(this);
    eventManager->SubScribe<Event::OnEntityDestroyed>(this);
}

void ImpEngineModule::CollisionManager::Reset()
{
    _broadphase->Clear();
    _lastColliderID = 0;
    _collisionInfomations.clear();
}

void ImpEngineModule::CollisionManager::Update(float dt)
{
    // Rigdbody
    _world->Each<Rigidbody>(
        [dt](Entity* ent, ComponentHandle<Rigidbody> rigid)
        {
            if (rigid->IsKinematic())
            {
                return;
            }

            // 힘 적용
            rigid->UpdateVelocity(dt);

            if (rigid->HasGravity() && !rigid->OnGround())
            {
                // 중력 적용
                ImpMath::Vector3 gravity{ 0.f, WorldGravity * dt, 0.f };
                rigid->AddVelocity(gravity);
            }

            rigid->SetOnGround(false);

            // 위치 계산
            rigid->UpdatePosition(dt);

            // LinearDamping (마찰력 개념)
            ImpMath::Vector3 v = rigid->GetVelocity();
            v *= std::exp(-rigid->GetLinearDamping() * dt);
            rigid->SetVelocity(v);
        });

    _world->Each<BoxCollider>([](Entity* ent, ComponentHandle<BoxCollider> box)
        { box->SyncWithTransform(); },
        false);

    _world->Each<SphereCollider>(
        [](Entity* ent, ComponentHandle<SphereCollider> sphere)
        { sphere->SyncWithTransform(); },
        false);

    // AABB Tree Update
    _broadphase->Update();

    // 이번 프레임 충돌 정보 계산
    auto& collisions = _broadphase->ComputeCollisions();

    // 충돌 해결
    _solver->SolveCollisions(collisions);

    // 평균으로 밀어내기 
    _world->Each<Rigidbody>(
        [dt](Entity* ent, ComponentHandle<Rigidbody> rigid)
        {
            if (rigid->IsKinematic())
            {
                return;
            }
            rigid->SolveAcmulateDistance();
        });

    // CallBack Event
    CallCollisionEvent(collisions, dt);
}

void ImpEngineModule::CollisionManager::AddCollider(Collider* collider)
{
    if (collider->GetNode() == nullptr)
    {
        _broadphase->Add(collider);

        ++_lastColliderID;
        collider->SetColliderID(_lastColliderID);
    }
}

void ImpEngineModule::CollisionManager::RemoveCollider(Collider* collider)
{
    if (collider->GetNode())
    {
        _broadphase->Remove(collider);
    }
}

void ImpEngineModule::CollisionManager::Finalize()
{
    _eventManager->UnSubScribe<Event::OnEntityStarted>(this);
    _eventManager->UnSubScribe<Event::OnEntityDestroyed>(this);
}

void ImpEngineModule::CollisionManager::SetTagMap(const Physics::TagMap& tagMap)
{
    _broadphase->SetTagMap(tagMap);
}

void ImpEngineModule::CollisionManager::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::OnEntityStarted& event)
{
    // Broadphase에 콜라이더 추가
    Entity* ent = event.entity;

    if (ent->HasComponent<BoxCollider>())
    {
        AddCollider(ent->GetComponent<BoxCollider>());
    }

    if (ent->HasComponent<SphereCollider>())
    {
        AddCollider(ent->GetComponent<SphereCollider>());
    }

    if (ent->HasComponent<CapsuleCollider>())
    {
        AddCollider(ent->GetComponent<CapsuleCollider>());
    }
}

void ImpEngineModule::CollisionManager::CallCollisionEvent(
    const std::vector<Physics::Collision>& collisions,
    float dt)
{
    for (auto& collision : collisions)
    {
        Physics::CollisionInfomation info(collision);

        auto iter = _collisionInfomations.find(info.colliderKey.key);

        // 새로운 충돌쌍 생성
        if (iter == _collisionInfomations.end())
        {
            _collisionInfomations.insert({ info.colliderKey.key, info });
        }
        else
        {
            iter->second.currentcCollision = collision;
        }
    }

    // 충돌 콜백 전달
    for (auto iter = _collisionInfomations.begin();
         iter != _collisionInfomations.end();)
    {
        bool prevCollision    = iter->second.prevCollision;
        bool currentCollision = iter->second.currentcCollision.isCollide;

        Collider* collider1 = iter->second.currentcCollision.lfm;
        Collider* collider2 = iter->second.currentcCollision.rfm;

        Entity* ent1 = collider1->GetEntity();
        Entity* ent2 = collider2->GetEntity();

        auto state1 = ent1->GetState();
        auto state2 = ent2->GetState();

        bool isTrigger1 = collider1->IsTrigger();
        bool isTrigger2 = collider2->IsTrigger();

        Physics::Collision& collision = iter->second.currentcCollision;
        collision.dt                  = dt;

        // Enter
        if (!prevCollision && currentCollision)
        {
            // 충돌하지 않은 것으로 판단한다.
            if (state1 == Entity::EntityState::Destroyed ||
                state2 == Entity::EntityState::Destroyed)
            {
                collision.isCollide = false;
            }
            else if (!isTrigger1 && !isTrigger2) // 둘다 트리거 X
            {
                ent1->OnCollsiionEnter(collision);
                ent2->OnCollsiionEnter(collision);
            }
            else
            {
                ent1->OnTriggerEnter(collision);
                ent2->OnTriggerEnter(collision);
            }
        }
        // Stay
        else if (prevCollision && currentCollision)
        {
            if (state1 == Entity::EntityState::Destroyed ||
                state2 == Entity::EntityState::Destroyed)
            {
                collision.isCollide = false;
                ent1->OnCollsiionExit(collision);
                ent2->OnCollsiionExit(collision);
            }
            else if (!isTrigger1 && !isTrigger2)
            {
                ent1->OnCollsiionStay(collision);
                ent2->OnCollsiionStay(collision);
            }
            else
            {
                ent1->OnTriggerStay(collision);
                ent2->OnTriggerStay(collision);
            }
        }
        // Exit
        else if (prevCollision && !currentCollision)
        {
            if (!isTrigger1 && !isTrigger2)
            {
                ent1->OnCollsiionExit(collision);
                ent2->OnCollsiionExit(collision);
            }
            else
            {
                ent1->OnTriggerExit(collision);
                ent2->OnTriggerExit(collision);
            }
        }

        iter->second.prevCollision               = collision.isCollide;
        iter->second.currentcCollision.isCollide = false;

        // 삭제처리
        if (state1 == Entity::EntityState::Destroyed ||
            state2 == Entity::EntityState::Destroyed)
        {
            iter = _collisionInfomations.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void ImpEngineModule::CollisionManager::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::OnEntityDestroyed& event)
{
    Entity* ent = event.entity;

    if (ent->HasComponent<BoxCollider>())
    {
        RemoveCollider(ent->GetComponent<BoxCollider>());
    }

    if (ent->HasComponent<SphereCollider>())
    {
        RemoveCollider(ent->GetComponent<SphereCollider>());
    }

    if (ent->HasComponent<CapsuleCollider>())
    {
        RemoveCollider(ent->GetComponent<CapsuleCollider>());
    }
}
