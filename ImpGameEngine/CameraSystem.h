#pragma once

#include "../ImpGraphicsEngine/ImpGraphicsEngine.h"

namespace ImpGameEngine
{
/// <summary>
/// ī�޶� �����ϴ� �ý���
/// </summary>
class CameraSystem:
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::SetSecondCamera>
{
public:
    CameraSystem();
    ~CameraSystem();

    /// �ʱ�ȭ
    void Initialize(ImpGraphics::IImpGraphicsEngine* graphicsEngine,
        ImpEngineModule::EngineModule* engineModule);

    // Tool ī�޶� ����
    void SetToolCamera(ImpEngineModule::Entity* main);

    void Start();

    // ī�޶� ������ �����Ѵ�.
    const ImpGraphics::CameraInfo& GetMainCameraInfo();

    ImpMath::Matrix GetMainCameraMarix() const;

    ImpMath::Matrix GetSecondCameraMarix() const;

    bool OnSecondCamera() const
    {
        return _onSecondCamera;
    }

    void SetSecondCamera(bool onSecondCamera);

private:
    // �׷��Ƚ� ������ �Ѱ��� ī�޶� ������ �����Ѵ�.
    void SetMainCameraInfo(ImpEngineModule::Entity* main);

    // �׷��Ƚ��� �Ѱ��� 2��° ī�޶� ������ �����Ѵ�.
    void SetSecondCameraInfo(ImpEngineModule::Entity* second);

    // �ι�° ī�޶� ���������� �̺�Ʈ�� ó���Ѵ�.
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
