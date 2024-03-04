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
// Tool 전용 게임엔진
//
// </summary>
class ImpToolEngine: public IImpEngine
{
public:
    ImpToolEngine();
    ~ImpToolEngine();

    // ToolEngine 초기화
    void Initialize(WindowInfomation info) override;

    // Tool엔진 루프
    void Process() override;

    // ToolEngine 종료
    void Finalize() override;

private:
    ImpGraphics::IImpGraphicsEngine* _graphicsEngine;
    ImpEngineModule::EngineModule* _engineModule;

    CameraSystem* _cameraSystem;
    RenderingSystem* _renderingSystem;

    ImpToolModule::IEditor* _editor;
};


} // namespace ImpGameEngine