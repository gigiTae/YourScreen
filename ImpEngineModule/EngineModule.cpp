#include "EngineModulePCH.h"
#include "EngineModule.h"
#include "World.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"

ImpEngineModule::EngineModule::EngineModule()
{
    _world = std::make_unique<World>();

    _eventManager     = std::make_unique<EventManager>();
    _resourceManager  = std::make_unique<ResourceManager>();
    _inputManager     = std::make_unique<InputManager>();
    _collisionManager = std::make_unique<CollisionManager>();

    _timeManager   = std::make_unique<TimeManager>();
    _worldManager  = std::make_unique<WorldManager>();
    _cameraManager = std::make_unique<CameraManager>();
}

ImpEngineModule::EngineModule::~EngineModule()
{
}

void ImpEngineModule::EngineModule::Initialize(HWND hWnd, std::wstring startWorld)
{
    _world->Initialize(_eventManager.get(),
                       _inputManager.get(),
                       _resourceManager.get());

    _inputManager->Initalize(hWnd);
    _eventManager->Initialize(_world.get(),_worldManager.get());

    _cameraManager->Initialize(_world.get());
    _collisionManager->Initialize(_eventManager.get(), _world.get());
    _resourceManager->Initailize(startWorld, _eventManager.get());
    _worldManager->Initialize(startWorld,
                              _world.get(),
                              _resourceManager.get(),
                              _collisionManager.get(),
                              _timeManager.get(),
                              _eventManager.get(),
                              _cameraManager.get(),
                              _inputManager.get());

    SoundManager::GetInstance()->Initialize();

    _timeManager->Initialize();
}

void ImpEngineModule::EngineModule::Start()
{
    _worldManager->LoadWorld();
    SoundManager::GetInstance()->LoadSoundList(_worldManager->GetWorldName());

    // 메인 카메라 설정
    _cameraManager->Initialize(_world.get());
    _world->Start();
}

float ImpEngineModule::EngineModule::Update()
{
    float dt = _timeManager->Update();
    _inputManager->Update(dt);
    SoundManager::GetInstance()->Update();

    // 충돌 처리
    _collisionManager->Update(dt);

    _world->CleanUp();
    _world->Update(dt);

    return dt;
}

void ImpEngineModule::EngineModule::PostUpdate()
{
    _eventManager->Update();
}

void ImpEngineModule::EngineModule::Finalize()
{
    SoundManager::GetInstance()->Finalize();
    _eventManager->Finalize();
    _timeManager->Finalize();
    _resourceManager->Finalize();
    _inputManager->Finalize();
    _worldManager->Finalize();
    _world->Reset();
}
