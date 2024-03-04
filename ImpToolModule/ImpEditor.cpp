#include "ImpToolModulePCH.h"
#include "ImpEditor.h"
#include "ComponentViewer.h"
#include "DebugViewer.h"
#include "EntityViewer.h"
#include "ResourceViewer.h"
#include "ToolCamera.h"
#include "WorldViewer.h"
#include "Shortcut.h"
#include "imgui_internal.h"
#include "ImpGizumo.h"

ImpToolModule::ImpEditor::ImpEditor()
    : _engineModule(nullptr)
    , _mode(ToolMode::EditorMode)
    , _currentWorld()
    , _graphicsEngine(nullptr)
    , _onPicking(true)
    , _onEntityViewer(true)
    , _imguiID{}
{
    _debugViewer      = std::make_unique<Debug::DebugViewer>();
    _worldViewer      = std::make_unique<WorldViewer>();
    _entityViewer     = std::make_unique<EntityViewer>();
    _componentViewer  = std::make_unique<ComponentViewer>();
    _resourceViewer   = std::make_unique<ResourceViewer>();
    _toolInputManager = std::make_unique<ImpEngineModule::InputManager>();
    _shortcut         = std::make_unique<Shortcut>();
    _gizumo           = std::make_unique<ImpGizumo>();
}

ImpToolModule::ImpEditor::~ImpEditor()
{
}

void ImpToolModule::ImpEditor::Initialize(HWND hWnd,
    void* device,
    void* deviceContext,
    ImpEngineModule::EngineModule* engineModule,
    ImpGraphics::IImpGraphicsEngine* grahicsEngnie)
{
    // ImGui 초기화
    InitializeImGui(hWnd,
        reinterpret_cast<ID3D11Device*>(device),
        reinterpret_cast<ID3D11DeviceContext*>(deviceContext));

    _engineModule   = engineModule;
    _graphicsEngine = grahicsEngnie;
    _toolCamera     = std::make_unique<ToolCamera>();

    _worldViewer->Initialize(_engineModule,
        _graphicsEngine,
        this,
        _shortcut.get(),
        _debugViewer.get(),
        _toolCamera.get());

    _resourceViewer->Initalize();
    _entityViewer->Initialize(
        _engineModule, _resourceViewer.get(), _graphicsEngine, _toolCamera.get());

    _componentViewer->Initialize(
        _graphicsEngine, _engineModule, _toolInputManager.get());

    _toolInputManager->Initalize(hWnd);
    _debugViewer->Initialize(_graphicsEngine, _engineModule);

    _shortcut->Initialize(engineModule,
        _entityViewer.get(),
        _toolInputManager.get(),
        _graphicsEngine,
        _worldViewer.get(),
        this);

    _gizumo->Initialize(
        _toolCamera.get(), _entityViewer.get(), _toolInputManager.get());

    // ToolCamera
    _toolCamera->Initialize(_engineModule, this, _toolInputManager.get());
}


void ImpToolModule::ImpEditor::Finalize()
{
    FinalizeImGui();
    _shortcut->Finalize();
    _toolCamera->Finalize();
}

void ImpToolModule::ImpEditor::Update()
{
    _imguiID = ImGui::DockSpaceOverViewport(
        ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    _toolInputManager->Update(0.f);

    _currentWorld = _engineModule->GetWorldManager()->GetWorldName();


    if (_mode == ToolMode::EditorMode)
    {
        _gizumo->Update();
    }

    if (_toolCamera->IsActive())
    {
        _toolCamera->Update();
    }
    if (_toolInputManager->IsKeyState(Key::F3, KeyState::Tap))
    {
        _toolCamera->SetActive(!_toolCamera->IsActive());
    }
    if (_toolInputManager->IsKeyState(Key::F8, KeyState::Tap))
    {
        _onEntityViewer = !_onEntityViewer;
    }

    if (_mode == ToolMode::EditorMode)
    {
        _resourceViewer->Update();
    }
    _worldViewer->Update();

    if (_onEntityViewer)
    {
        _entityViewer->Update();
        _componentViewer->Update(_entityViewer->GetSelectEntity());
        _shortcut->Update();
    }

    _debugViewer->Update();

    // 스크린 남은공간 가져오기
    PickingObject();
}

void ImpToolModule::ImpEditor::NewFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void ImpToolModule::ImpEditor::EndRender()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

bool ImpToolModule::ImpEditor::IsChangeMode()
{
    if (_toolInputManager->IsKeyState(Key::F1, KeyState::Tap))
    {
        if (_mode == ImpToolModule::ToolMode::EditorMode)
        {
            ChangeMode(ImpToolModule::ToolMode::GamePlayMode);
            return true;
        }
        else if (_mode == ImpToolModule::ToolMode::GamePlayMode ||
                 _mode == ImpToolModule::ToolMode::PauseMode)
        {
            ChangeMode(ImpToolModule::ToolMode::EditorMode);
            return true;
        }
    }

    if (_toolInputManager->IsKeyState(Key::F4, KeyState::Tap))
    {
        if (_mode == ImpToolModule::ToolMode::GamePlayMode)
        {
            ChangeMode(ImpToolModule::ToolMode::PauseMode);
            return true;
        }
        else if (_mode == ImpToolModule::ToolMode::PauseMode)
        {
            ChangeMode(ImpToolModule::ToolMode::GamePlayMode);
            return true;
        }
    }

    return false;
}

ImpEngineModule::Entity* ImpToolModule::ImpEditor::GetToolCamera() const
{
    if (_toolCamera->IsActive())
    {
        return _toolCamera->GetEntity();
    }

    return nullptr;
}

void ImpToolModule::ImpEditor::ChangeMode(ToolMode mode)
{
    switch (mode)
    {
        case ImpToolModule::ToolMode::EditorMode:
        {
            _mode = mode;
            // 1. 저장해둔 씬을 다시 불러온다.
            _engineModule->GetWorldManager()->ChangeWorld(_currentWorld);
            _toolCamera->SetActive(true);
        }
        break;
        case ImpToolModule::ToolMode::GamePlayMode:
        {
            if (_mode == ToolMode::EditorMode)
            {
                // 1. 현재월드를 저장한다.
                _worldViewer->SaveWorld();

                // 2. Start 현재월드
                _engineModule->GetWorldManager()->ChangeWorld(_currentWorld);
                _toolCamera->SetActive(false);
            }

            _mode = mode;
        }
        break;
        case ImpToolModule::ToolMode::PauseMode:
        {
            _mode = mode;
        }
        break;
        default:
            break;
    }
}

void ImpToolModule::ImpEditor::InitializeImGui(HWND hWnd,
    ID3D11Device* device,
    ID3D11DeviceContext* deviceContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf",
        18.f,
        NULL,
        io.Fonts->GetGlyphRangesKorean());

    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(device, deviceContext);

    {
        auto& colors               = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg]  = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
        colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

        // Border
        colors[ImGuiCol_Border]       = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
        colors[ImGuiCol_BorderShadow] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.24f };

        // Text
        colors[ImGuiCol_Text]         = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
        colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };

        // Headers
        colors[ImGuiCol_Header]        = ImVec4{ 0.13f, 0.13f, 0.17f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
        colors[ImGuiCol_HeaderActive]  = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button]        = ImVec4{ 0.13f, 0.13f, 0.17f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
        colors[ImGuiCol_ButtonActive]  = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
        colors[ImGuiCol_CheckMark]     = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };

        // Popups
        colors[ImGuiCol_PopupBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 0.92f };

        // Slider
        colors[ImGuiCol_SliderGrab] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.54f };
        colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.54f };

        // Frame BG
        colors[ImGuiCol_FrameBg]        = ImVec4{ 0.13f, 0.13f, 0.17f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
        colors[ImGuiCol_FrameBgActive]  = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab]          = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
        colors[ImGuiCol_TabHovered]   = ImVec4{ 0.24f, 0.24f, 0.32f, 1.0f };
        colors[ImGuiCol_TabActive]    = ImVec4{ 0.2f, 0.22f, 0.27f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg]          = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
        colors[ImGuiCol_TitleBgActive]    = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

        // Scrollbar
        colors[ImGuiCol_ScrollbarBg]   = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
        colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.24f, 0.24f, 0.32f, 1.0f };

        // Seperator
        colors[ImGuiCol_Separator]        = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
        colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };
        colors[ImGuiCol_SeparatorActive]  = ImVec4{ 0.84f, 0.58f, 1.0f, 1.0f };

        // Resize Grip
        colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
        colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.29f };
        colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 0.29f };

        // Docking
        colors[ImGuiCol_DockingPreview] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };

        auto& style             = ImGui::GetStyle();
        style.TabRounding       = 4;
        style.ScrollbarRounding = 9;
        style.WindowRounding    = 7;
        style.GrabRounding      = 3;
        style.FrameRounding     = 3;
        style.PopupRounding     = 4;
        style.ChildRounding     = 4;
    }
}

void ImpToolModule::ImpEditor::FinalizeImGui()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImpToolModule::ImpEditor::PickingObject()
{
    if (_toolInputManager->IsKeyState(Key::Ins, KeyState::Tap))
    {
        _onPicking = !_onPicking;
    }

    if (!_onPicking)
    {
        return;
    }

    auto node = ImGui::DockBuilderGetCentralNode(_imguiID);
    auto size = node->Size;

    if (_gizumo->GetMode() != IMGUIZMO_NAMESPACE::UNIVERSAL)
    {
        return;
    }

    // 마우스 피킹
    if (_toolInputManager->IsKeyState(Key::LMouse, KeyState::Tap))
    {
        POINT pos = _toolInputManager->GetMousePosition();

        if (pos.x <= size.x && pos.y <= size.y)
        {
            size_t id = _graphicsEngine->GetPickingObject(pos.x, pos.y);
            if (id != static_cast<size_t>(-1))
            {
                _entityViewer->SetSelectEntity(id);
            }
        }
    }
}
