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
// 게임엔진
//
// </summary>
class ImpGameEngine: public IImpEngine
{
public:
    ImpGameEngine();
    ~ImpGameEngine();

    // GameEngine 초기화
    void Initialize(WindowInfomation info) override;

    // 게임엔진 루프
    void Process() override;

    // GameEngine 종료
    void Finalize() override;

private:
    ImpGraphics::IImpGraphicsEngine* _graphicsEngine;
    std::unique_ptr<ImpEngineModule::EngineModule> _engineModule;

    std::unique_ptr<CameraSystem> _cameraSystem;
    std::unique_ptr<RenderingSystem> _renderingSystem;
};


} // namespace ImpGameEngine