#include "ImpToolModulePCH.h"
#include "DebugViewer.h"
#include "Cube.h"
#include "SphereCollider.h"
#include "Light.h"
#include "Camera.h"
#include "CapsuleCollider.h"

ImpToolModule::Debug::DebugViewer::~DebugViewer()
{
}

ImpToolModule::Debug::DebugViewer::DebugViewer()
    : _lights{}
    , _cubeColliders{}
    , _sphereColliders{}
    , _capsuleColliders{}
    , _engineModule(nullptr)
    , _graphicsEngine(nullptr)
    , _world(nullptr)
    , _isShowCameraGizumo(false)
    , _isShowCollider(true)
    , _isShowLightGizumo(false)
{
}

void ImpToolModule::Debug::DebugViewer::Update()
{

    const auto& entities = _world->GetEntities();

    for (const auto& ent : entities)
    {
        size_t id = ent->GetID();

        if (ent->GetState() == ImpEngineModule::Entity::EntityState::Destroyed)
        {
            auto iter = _cubeColliders.find(id);
            if (iter != _cubeColliders.end())
            {
                _cubeColliders.erase(iter);
            }

            auto sphereIter = _sphereColliders.find(id);
            if (sphereIter != _sphereColliders.end())
            {
                _sphereColliders.erase(sphereIter);
            }

            auto lightIter = _lights.find(id);
            if (lightIter != _lights.end())
            {
                _lights.erase(lightIter);
            }

            auto cameraIter = _cameras.find(id);
            if (cameraIter != _cameras.end())
            {
                _cameras.erase(cameraIter);
            }

            auto capsuleIter = _capsuleColliders.find(id);
            if (capsuleIter != _capsuleColliders.end())
            {
                _capsuleColliders.erase(capsuleIter);
            }

            continue;
        }


        /// Collider DebugRendering
        if (!_isShowCollider)
        {
            _cubeColliders.clear();
            _sphereColliders.clear();
            _capsuleColliders.clear();
        }
        else
        {
            // Box ÄÄÆ÷³ÍÆ®
            if (ent->HasComponent<ImpEngineModule::BoxCollider>())
            {
                ent->GetComponent<ImpEngineModule::BoxCollider>()->SyncWithTransform();

                auto iter = _cubeColliders.find(id);

                if (iter == _cubeColliders.end())
                {
                    _cubeColliders.insert(
                        { id, std::make_unique<Cube>(_graphicsEngine, ent.get()) });
                }
            }

            // Sphere
            if (ent->HasComponent<ImpEngineModule::SphereCollider>())
            {
                ent->GetComponent<ImpEngineModule::SphereCollider>()->SyncWithTransform();

                auto iter = _sphereColliders.find(id);

                if (iter == _sphereColliders.end())
                {
                    _sphereColliders.insert(
                        { id,
                          std::make_unique<SphereCollider>(_graphicsEngine,
                                                           ent.get()) });
                }
            }

            // Capsule
            if (ent->HasComponent<ImpEngineModule::CapsuleCollider>())
            {
                ent->GetComponent<ImpEngineModule::CapsuleCollider>()
                    ->SyncWithTransform();

                auto iter = _capsuleColliders.find(id);

                if (iter == _capsuleColliders.end())
                {
                    _capsuleColliders.insert(
                        { id,
                          std::make_unique<CapsuleCollider>(_graphicsEngine,
                                                            ent.get()) });
                }
            }
            else
            {
                auto iter = _capsuleColliders.find(id);
                if (iter != _capsuleColliders.end())
                {
                    _capsuleColliders.erase(iter);
                }

            }
        }

        /// LightGizumo Rendernig
        if (!_isShowLightGizumo)
        {
            _lights.clear();
        }
        else
        {
            if (ent->HasComponent<ImpEngineModule::Light>())
            {
                auto iter = _lights.find(id);

                if (iter == _lights.end())
                {
                    _lights.insert(
                        { id, std::make_unique<Light>(_graphicsEngine, ent.get()) });
                }
            }
        }

        /// Camera Rendering
        if (!_isShowCameraGizumo)
        {
            _cameras.clear();
        }
        else
        {
            if (ent->HasComponent<ImpEngineModule::Camera>())
            {
                auto iter = _cameras.find(id);

                if (iter == _cameras.end())
                {
                    _cameras.insert(
                        { id, std::make_unique<Camera>(_graphicsEngine, ent.get()) });
                }
            }
        }
    }

    for (auto& cube : _cubeColliders)
    {
        cube.second->Update();
    }

    for (auto& sphere : _sphereColliders)
    {
        sphere.second->Update();
    }

    for (auto& light : _lights)
    {
        light.second->Update();
    }

    for (auto& camera : _cameras)
    {
        camera.second->Update();
    }

    for (auto& capsule : _capsuleColliders)
    {
        capsule.second->Update();
    }
}

void ImpToolModule::Debug::DebugViewer::Initialize(
    ImpGraphics::IImpGraphicsEngine* graphicsEngine,
    ImpEngineModule::EngineModule* engineModule)
{
    _engineModule   = engineModule;
    _graphicsEngine = graphicsEngine;
    _world          = _engineModule->GetWorld();

    _engineModule->GetEventManager()->SubScribe<ImpEngineModule::Event::LoadedWorld>(
        this);
}

void ImpToolModule::Debug::DebugViewer::Receive(
    ImpEngineModule::World* world,
    const ImpEngineModule::Event::LoadedWorld& event)
{
    _cubeColliders.clear();
    _sphereColliders.clear();
    _lights.clear();
    _capsuleColliders.clear();
}
