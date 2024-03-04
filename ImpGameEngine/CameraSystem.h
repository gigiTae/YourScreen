#pragma once

#include "../ImpGraphicsEngine/ImpGraphicsEngine.h"

namespace ImpGameEngine
{
/// <summary>
/// 카메라를 관리하는 시스템
/// </summary>
class CameraSystem:
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::SetSecondCamera>
{
public:
    CameraSystem();
    ~CameraSystem();

    /// 초기화
    void Initialize(ImpGraphics::IImpGraphicsEngine* graphicsEngine,
        ImpEngineModule::EngineModule* engineModule);

    // Tool 카메라를 설정
    void SetToolCamera(ImpEngineModule::Entity* main);

    void Start();

    // 카메라 정보를 설정한다.
    const ImpGraphics::CameraInfo& GetMainCameraInfo();

    ImpMath::Matrix GetMainCameraMarix() const;

    ImpMath::Matrix GetSecondCameraMarix() const;

    bool OnSecondCamera() const
    {
        return _onSecondCamera;
    }

    void SetSecondCamera(bool onSecondCamera);

private:
    // 그래픽스 엔진에 넘겨줄 카메라 정보를 설정한다.
    void SetMainCameraInfo(ImpEngineModule::Entity* main);

    // 그래픽스에 넘겨줄 2번째 카메라 정보를 설정한다.
    void SetSecondCameraInfo(ImpEngineModule::Entity* second);

    // 두번째 카메라 설정에대한 이벤트를 처리한다.
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::SetSecondCamera& event) override;

private:
    bool _onSecondCamera;

    ImpEngineModule::Entity* _toolCamera;
    ImpEngineModule::EngineModule* _engineModule;
    ImpGraphics::CameraInfo _mainCameraInfo;
    ImpGraphics::CameraInfo _secondCameraInfo;
    ImpGraphics::IImpGraphicsEngine* _graphicsEngine;
};

} // namespace ImpGameEngine
