#pragma once
#include "IEditor.h"

namespace ImpToolModule::Debug
{
class DebugViewer;
}


namespace ImpToolModule
{
class WorldViewer;
class EntityViewer;
class ComponentViewer;
class ResourceViewer;
class ToolCamera;
class Shortcut;
class ImpGizumo;

/// <summary>
/// 가장 상단의 에티터 역할을한다.
/// 다른 에디터들을 포함한다.
/// </summary>
class ImpEditor: public IEditor
{
public:
    ImpEditor();
    ~ImpEditor();

public:
    // Editor 초기화
    void Initialize(HWND hWnd,
        void* device,
        void* deviceContext,
        ImpEngineModule::EngineModule* engineModule,
        ImpGraphics::IImpGraphicsEngine* grahicsEngnie) override;

    // Editor 종료
    void Finalize() override;

    // Editor를 갱신한다.
    void Update() override;

    // ImGui 새로운 프레임 갱신
    void NewFrame() override;

    // GUI를 그린다.
    void EndRender() override;

    // 이번프레임에 모드가 변경되었는지
    bool IsChangeMode() override;

    // Tool 카메라를 얻어온다.
    ImpEngineModule::Entity* GetToolCamera() const override;

    // 툴의 모드를 바꾼다.
    void ChangeMode(ToolMode mode) override;

    // 현재 에디터의 모드를 반환
    ToolMode GetMode() const override
    {
        return _mode;
    }

private:
    void InitializeImGui(HWND hWnd,
        ID3D11Device* device,
        ID3D11DeviceContext* deviceContext);
    void FinalizeImGui();

    void PickingObject();

private:
    ToolMode _mode;
    std::wstring _currentWorld;

    ImGuiID _imguiID;

    ImpEngineModule::EngineModule* _engineModule;
    ImpGraphics::IImpGraphicsEngine* _graphicsEngine;

    std::unique_ptr<WorldViewer> _worldViewer;
    std::unique_ptr<EntityViewer> _entityViewer;
    std::unique_ptr<ComponentViewer> _componentViewer;
    std::unique_ptr<ResourceViewer> _resourceViewer;
    std::unique_ptr<Debug::DebugViewer> _debugViewer;
    std::unique_ptr<Shortcut> _shortcut;
    std::unique_ptr<ImpGizumo> _gizumo;


    // 카메라 관련
    std::unique_ptr<ImpEngineModule::InputManager> _toolInputManager;
    std::unique_ptr<ToolCamera> _toolCamera;

    bool _onEntityViewer;
    bool _onPicking;
};

} // namespace ImpToolModule