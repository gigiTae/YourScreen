#include "EngineModulePCH.h"
#include "CameraManager.h"
#include "World.h"
#include "Camera.h"
#include "Transform.h"

ImpEngineModule::CameraManager::CameraManager()
    : _world(nullptr)
    , _mainCamera(nullptr)
    , _secondCamera(nullptr)
{
}

ImpEngineModule::CameraManager::~CameraManager()
{
}

void ImpEngineModule::CameraManager::Initialize(World* world)
{
    _world = world;

    _mainCamera = FindMainCamera();
}

void ImpEngineModule::CameraManager::Finalize()
{
}

ImpEngineModule::Entity* ImpEngineModule::CameraManager::FindMainCamera()
{
    for (Entity* ent : _world->Each<Transform, Camera>(false))
    {
        Camera* camera = ent->GetComponent<Camera>();

        assert(camera);

        if (camera->IsMain())
        {
            return ent;
        }
    }
    return nullptr;
}

ImpEngineModule::Entity* ImpEngineModule::CameraManager::FindSecondCamera()
{
    for (Entity* ent : _world->Each<Transform, Camera>(false))
    {
        Camera* camera = ent->GetComponent<Camera>();

        assert(camera);

        if (camera->IsSecond())
        {
            return ent;
        }
    }
    return nullptr;
}

ImpEngineModule::Entity* ImpEngineModule::CameraManager::GetMainCamera()
{
    _mainCamera = FindMainCamera();
    return _mainCamera;
}

ImpEngineModule::Entity* ImpEngineModule::CameraManager::GetSecondCamera()
{
    _secondCamera = FindSecondCamera();
    return _secondCamera;
}
