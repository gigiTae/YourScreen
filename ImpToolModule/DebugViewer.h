#pragma once

namespace ImpToolModule::Debug
{
class SphereCollider;
class Cube;
class Light;
class Camera;
class CapsuleCollider;

/// <summary>
/// 디버그 정보를 표시하는 viewer
/// </summary>
class DebugViewer:
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::LoadedWorld>
{
public:
    DebugViewer();
    ~DebugViewer();

    void Initialize(ImpGraphics::IImpGraphicsEngine* graphicsEngine,
                    ImpEngineModule::EngineModule* engineModule);

    void Update();

    void SetShowCollider(bool isShowCollider)
    {
        _isShowCollider = isShowCollider;
    }
    void SetShowLightGizumo(bool isShowLight)
    {
        _isShowLightGizumo = isShowLight;
    }
    void SetShowCameraGizumo(bool isShowCamera)
    {
        _isShowCameraGizumo = isShowCamera;
    }

private:
    void Receive(ImpEngineModule::World* world,
                 const ImpEngineModule::Event::LoadedWorld& event) override;

private:
    bool _isShowCollider;
    bool _isShowLightGizumo;
    bool _isShowCameraGizumo;

    ImpGraphics::IImpGraphicsEngine* _graphicsEngine;
    ImpEngineModule::EngineModule* _engineModule;
    ImpEngineModule::World* _world;

    std::unordered_map<size_t, std::unique_ptr<SphereCollider>> _sphereColliders;
    std::unordered_map<size_t, std::unique_ptr<Cube>> _cubeColliders;
    std::unordered_map<size_t, std::unique_ptr<Light>> _lights;
    std::unordered_map<size_t, std::unique_ptr<Camera>> _cameras;
    std::unordered_map<size_t, std::unique_ptr<CapsuleCollider>> _capsuleColliders;
};

} // namespace ImpToolModule::Debug