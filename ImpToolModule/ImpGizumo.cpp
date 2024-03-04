#include "ImpToolModulePCH.h"
#include "ImpGizumo.h"
#include "ToolCamera.h"
#include "EntityViewer.h"


ImpToolModule::ImpGizumo::ImpGizumo()
    : _input(nullptr)
    , _toolCamera(nullptr)
    , _entityViewer(nullptr)
    , _operation(IMGUIZMO_NAMESPACE::OPERATION::TRANSLATE)
{
}

ImpToolModule::ImpGizumo::~ImpGizumo()
{
}


void ImpToolModule::ImpGizumo::Initialize(ToolCamera* camera,
    EntityViewer* entViewer,
    ImpEngineModule::InputManager* input)
{
    _input        = input;
    _toolCamera   = camera;
    _entityViewer = entViewer;
}

void ImpToolModule::ImpGizumo::Update()
{
    SetGizumode();

    ShowUI();

    auto ent = _entityViewer->GetSelectEntity();

    if (ent == nullptr || _operation == IMGUIZMO_NAMESPACE::UNIVERSAL)
    {
        ImGuizmo::Enable(false);
        return;
    }


    ImGuizmo::Enable(true);

    auto view   = _toolCamera->GetViewTM();
    auto proj   = _toolCamera->GetProjTM();
    auto entT   = ent->GetComponent<ImpEngineModule::Transform>();
    auto entMat = entT->GetWorldMatrix();
    auto rect   = _input->GetScreenRect();
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(static_cast<float>(rect.left),
        static_cast<float>(rect.top),
        io.DisplaySize.x,
        io.DisplaySize.y);

    ImGuizmo::Manipulate(
        &view._11, &proj._11, _operation, IMGUIZMO_NAMESPACE::WORLD, &entMat._11);

    if (entT->IsRoot())
    {
        entT->SetLocalMatrix(entMat);
    }
    else
    {
        ImpMath::Matrix parentM = entT->GetParent()->GetWorldMatrix();
        entMat                  = entMat * parentM.Inverse();
        entT->SetLocalMatrix(entMat);
    }
}

void ImpToolModule::ImpGizumo::SetGizumode()
{
    if (_input->IsKeyState(Key::RMouse, KeyState::Hold))
    {
        return;
    }
    // W 이동
    if (_input->IsKeyState(Key::W, KeyState::Tap))
    {
        _operation = IMGUIZMO_NAMESPACE::TRANSLATE;
    }
    // E 회전
    if (_input->IsKeyState(Key::E, KeyState::Tap))
    {
        _operation = IMGUIZMO_NAMESPACE::ROTATE;
    }
    // R 스케일
    if (_input->IsKeyState(Key::R, KeyState::Tap))
    {
        _operation = IMGUIZMO_NAMESPACE::SCALE;
    }

    if (_input->IsKeyState(Key::Q, KeyState::Tap))
    {
        _operation = IMGUIZMO_NAMESPACE::UNIVERSAL;
    }
}

void ImpToolModule::ImpGizumo::ShowUI()
{
    static bool open = true;

    static int location           = 0;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                                    ImGuiWindowFlags_NoDocking |
                                    ImGuiWindowFlags_AlwaysAutoResize |
                                    ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_NoFocusOnAppearing |
                                    ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD               = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) :
                                        (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) :
                                        (work_pos.y + PAD);

        window_pos.y *= 1.f;
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
            ImGuiCond_Always,
            ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Mode", &open, window_flags))
    {
        ImGui::Text("Mode");
        ImGui::Separator();

        std::string mode;

        switch (_operation)
        {
            case ImGuizmo::TRANSLATE:
            {
                mode = "TRANSLATE";
            }
            break;
            case ImGuizmo::ROTATE:
            {
                mode = "ROTATE";
            }
            break;
            case ImGuizmo::SCALE:
            {
                mode = "SCALE";
            }
            break;
            case ImGuizmo::UNIVERSAL:
            {
                mode = "PICKING";
            }
            break;
            default:
                break;
        }

        ImGui::Text(mode.c_str());
        ImGui::End();
    }
}
