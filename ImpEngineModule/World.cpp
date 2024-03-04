#include "EngineModulePCH.h"
#include "World.h"
#include "Entity.h"
#include "Transform.h"
#include "CallBackEvent.h"
#include "EventManager.h"
#include "NameCheck.h"

ImpEngineModule::World::World()
    : _lastEntityID(0)
    , _entities{}
    , _eventManager(nullptr)
    , _inputManager(nullptr)
    , _resourceManager(nullptr)
{
}

ImpEngineModule::World::~World()
{
}

void ImpEngineModule::World::Initialize(EventManager* eventMgr,
    InputManager* inputMgr,
    ResourceManager* resMgr)
{
    _eventManager    = eventMgr;
    _inputManager    = inputMgr;
    _resourceManager = resMgr;
}

void ImpEngineModule::World::Reset()
{
    _lastEntityID = 0;

    for (std::shared_ptr<Entity>& ent : _entities)
    {
        if (ent->GetState() == Entity::EntityState::Active)
        {
            _eventManager->Emit<Event::OnEntityDestroyed>({ ent.get() });
        }
        ent->DestroyAllComponents();
    }

    _entities.clear();
}


void ImpEngineModule::World::Start()
{
    for (std::shared_ptr<Entity>& ent : _entities)
    {
        ent->Start();
        _eventManager->Emit<Event::OnEntityStarted>({ ent.get() });
    }
}

void ImpEngineModule::World::Update(float dt)
{
    for (std::shared_ptr<Entity>& ent : _entities)
    {
        ent->Update(dt);
    }
}

void ImpEngineModule::World::Finalize()
{
    Reset();
}

void ImpEngineModule::World::CleanUp()
{
    _entities.erase(std::remove_if(_entities.begin(),
                        _entities.end(),
                        [](std::shared_ptr<Entity>& ent)
                        {
                            if (ent->GetState() != Entity::EntityState::Active)
                            {
                                return true;
                            }
                            return false;
                        }),
        _entities.end());
}

ImpEngineModule::Entity* ImpEngineModule::World::CreateEntity()
{
    ++_lastEntityID;
    std::shared_ptr<Entity> ent = std::make_shared<Entity>(this, _lastEntityID);

    _entities.push_back(ent);
    _eventManager->Emit<Event::OnEntityCreated>({ ent.get() });

    return ent.get();
}

ImpEngineModule::Entity* ImpEngineModule::World::RegisterEntity(
    const std::shared_ptr<Entity>& ent)
{
    for (auto& originEnt : _entities)
    {
        if (originEnt.get() == ent.get())
        {
            assert(!L"중복으로 등록한 Entity입니다");
        }
    }

    ent->SetWorld(this);
    ent->_state = Entity::EntityState::Active;

    _eventManager->PushPostEvent(
        [this, ent]()
        {
            // 이름중복 제거
            std::string entName = ent->GetName();
            ImpEngineModule::NameCheck::CheckNameDuplication(
                this, ent.get(), entName);
            ent->SetName(entName);

            _entities.push_back(ent);
            ent->Start();
            _eventManager->Emit<Event::OnEntityStarted>({ ent.get() });
        });

    return ent.get();
}

void ImpEngineModule::World::DestroyEntity(Entity* ent)
{
    if (ent == nullptr || ent->GetState() != Entity::EntityState::Active)
    {
        return;
    }

    ent->_state = Entity::EntityState::Destroyed;
    _eventManager->Emit<Event::OnEntityDestroyed>({ ent });

    auto transform = ent->GetComponent<Transform>();

    auto parent = transform->GetParent();

    if (parent != nullptr)
    {
        // 부모쪽에서 연결 해제
        auto& parentChildren = parent->GetChildren();

        parentChildren.erase(
            std::remove_if(parentChildren.begin(),
                parentChildren.end(),
                [transform](ImpEngineModule::Transform* t)
                {
                    if (t == transform)
                    {
                        return true;
                    }
                    return false;
                }),
            parentChildren.end());
    }

    // 자식들도 모두 삭제한다.
    auto& children = transform->GetChildren();

    for (Transform* childT : children)
    {
        Entity* child = childT->GetEntity();
        childT->SetParent(nullptr);
        DestroyEntity(child);
    }
}

ImpEngineModule::Entity* ImpEngineModule::World::GetByIndex(size_t index) const
{
    if (_entities.size() <= index) return nullptr;

    return _entities[index].get();
}

void ImpEngineModule::World::DestroyEntity(size_t id)
{
    if (id >= _lastEntityID)
    {
        return;
    }

    Entity* ent = GetEntity(id);

    DestroyEntity(ent);
}

ImpEngineModule::Entity* ImpEngineModule::World::GetEntity(
    const std::string& name) const
{
    for (const std::shared_ptr<Entity>& ent : _entities)
    {
        if (name == ent->_name)
        {
            return ent.get();
        }
    }
    return nullptr;
}

ImpEngineModule::Entity* ImpEngineModule::World::GetEntity(size_t id) const
{
    for (const std::shared_ptr<Entity>& ent : _entities)
    {
        if (id == ent->_id)
        {
            return ent.get();
        }
    }
    return nullptr;
}

ImpEngineModule::Internal::EntityView ImpEngineModule::World::All(
    bool isIncludeToBeDestroyed /*= false*/)
{
    Internal::EntityIterator first(this, 0, false, isIncludeToBeDestroyed);
    Internal::EntityIterator last(this, GetSize(), true, isIncludeToBeDestroyed);

    return Internal::EntityView(first, last);
}

void ImpEngineModule::World::All(std::function<void(Entity*)> viewFunc,
    bool isIncludeToBeDestroyed /*= false*/)
{
    for (Entity* ent : All(isIncludeToBeDestroyed))
    {
        viewFunc(ent);
    }
}
