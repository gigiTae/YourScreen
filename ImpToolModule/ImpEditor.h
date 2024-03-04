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
/// ���� ����� ��Ƽ�� �������Ѵ�.
/// �ٸ� �����͵��� �����Ѵ�.
/// </summary>
class ImpEditor: public IEditor
{
public:
    ImpEditor();
    ~ImpEditor();

public:
    // Editor �ʱ�ȭ
    void Initialize(HWND hWnd,
        void* device,
        void* deviceContext,
        ImpEngineModule::EngineModule* engineModule,
        ImpGraphics::IImpGraphicsEngine* grahicsEngnie) override;

    // Editor ����
    void Finalize() override;

    // Editor�� �����Ѵ�.
    void Update() override;

    // ImGui ���ο� ������ ����
    void NewFrame() override;

    // GUI�� �׸���.
    void EndRender() override;

    // �̹������ӿ� ��尡 ����Ǿ�����
    bool IsChangeMode() override;

    // Tool ī�޶� ���´�.
    ImpEngineModule::Entity* GetToolCamera() const override;

    // ���� ��带 �ٲ۴�.
    void ChangeMode(ToolMode mode) override;

    // ���� �������� ��带 ��ȯ
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


    // ī�޶� ����
    std::unique_ptr<ImpEngineModule::InputManager> _toolInputManager;
    std::unique_ptr<ToolCamera> _toolCamera;

    bool _onEntityViewer;
    bool _onPicking;
};

} // namespace ImpToolModule