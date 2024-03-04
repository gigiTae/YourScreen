#include "EngineModulePCH.h"
#include "EventManager.h"
#include "WorldManager.h"

ImpEngineModule::EventManager::EventManager()
    : _world(nullptr)
    , _worldManager(nullptr)
{
}

ImpEngineModule::EventManager::~EventManager()
{
}

void ImpEngineModule::EventManager::Initialize(World* world, WorldManager* worldMgr)
{
    _world        = world;
    _worldManager = worldMgr;
}


void ImpEngineModule::EventManager::Finalize()
{
}

void ImpEngineModule::EventManager::Update()
{
    while (!_eventQueue.empty())
    {
        auto& func = _eventQueue.front();
        func();
        _eventQueue.pop();
    }
}

void ImpEngineModule::EventManager::UnSubScribeAll(void* subscriber)
{
}

void ImpEngineModule::EventManager::PushPostEvent(const std::function<void()>& func)
{
    _eventQueue.push(func);
}

void ImpEngineModule::EventManager::ChangeWorld(const std::wstring& worldName)
{
    _eventQueue.push([this, worldName]()
                     { _worldManager->ChangeWorld(worldName); });
}
