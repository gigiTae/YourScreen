#pragma once

#include "IImpEngine.h"

namespace ImpGraphics
{
class IImpGraphicsEngine;
}
namespace ImpEngineModule
{
class EngineModule;
}

namespace ImpGameEngine
{
class CameraSystem;
class RenderingSystem;

// <summary>
// ���ӿ���
//
// </summary>
class ImpGameEngine: public IImpEngine
{
public:
    ImpGameEngine();
    ~ImpGameEngine();

    // GameEngine �ʱ�ȭ
    void Initialize(WindowInfomation info) override;

    // ���ӿ��� ����
    void Process() override;

    // GameEngine ����
    void Finalize() override;

private:
    ImpGraphics::IImpGraphicsEngine* _graphicsEngine;
    std::unique_ptr<ImpEngineModule::EngineModule> _engineModule;

    std::unique_ptr<CameraSystem> _cameraSystem;
    std::unique_ptr<RenderingSystem> _renderingSystem;
};


} // namespace ImpGameEngine