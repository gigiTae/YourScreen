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
// Tool ���� ���ӿ���
//
// </summary>
class ImpToolEngine: public IImpEngine
{
public:
    ImpToolEngine();
    ~ImpToolEngine();

    // ToolEngine �ʱ�ȭ
    void Initialize(WindowInfomation info) override;

    // Tool���� ����
    void Process() override;

    // ToolEngine ����
    void Finalize() override;

private:
    ImpGraphics::IImpGraphicsEngine* _graphicsEngine;
    ImpEngineModule::EngineModule* _engineModule;

    CameraSystem* _cameraSystem;
    RenderingSystem* _renderingSystem;

    ImpToolModule::IEditor* _editor;
};


} // namespace ImpGameEngine