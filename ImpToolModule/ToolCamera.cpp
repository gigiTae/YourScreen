#include "ImpToolModulePCH.h"
#include "ToolCamera.h"
#include "ImpEditor.h"


ImpToolModule::ToolCamera::ToolCamera()
    : _isArrived(true)
    , _cameraSpeed(100.f)
    , _inputManager(nullptr)
    , _editor(nullptr)
    , _engineModule(nullptr)
    , _elapsedTime(0.f)
    , _isActive(true)
{
}

ImpToolModule::ToolCamera::~ToolCamera()
{
}


void ImpToolModule::ToolCamera::Initialize(
    ImpEngineModule::EngineModule* engineModule,
    ImpEditor* editor,
    ImpEngineModule::InputManager* input)
{
    _engineModule = engineModule;
    _editor       = editor;
    _inputManager = input;

    _ent = std::make_unique<ImpEngineModule::Entity>();

    _ent->AddComponent<ImpEngineModule::Transform>();
    _ent->AddComponent<ImpEngineModule::Camera>();
}


void ImpToolModule::ToolCamera::Update()
{
    UpdateMatrix();

    float dt = _engineModule->GetTimeManager()->GetDeltaTime();
    _ent->Update(dt);

    using namespace ImpEngineModule;


    // Transform 가져오기
    Transform* transform = _ent->GetComponent<Transform>();
    Camera* camera       = _ent->GetComponent<Camera>();
    /// 키입력 -> 카메라 이동
    float distance = dt * 20.f;

    if (_inputManager->IsKeyState(Key::LShift, KeyState::Hold))
    {
        distance *= 5.f;
    }

    if (_inputManager->IsKeyState(Key::RMouse, KeyState::Hold))
    {
        if (_inputManager->IsKeyState(Key::A, KeyState::Hold))
        {
            camera->Strafe(-distance);
        }
        if (_inputManager->IsKeyState(Key::D, KeyState::Hold))
        {
            camera->Strafe(distance);
        }
        if (_inputManager->IsKeyState(Key::W, KeyState::Hold))
        {
            camera->Walk(distance);
        }
        if (_inputManager->IsKeyState(Key::S, KeyState::Hold))
        {
            camera->Walk(-distance);
        }
        if (_inputManager->IsKeyState(Key::Q, KeyState::Hold))
        {
            camera->WorldUpDown(-distance);
        }
        if (_inputManager->IsKeyState(Key::E, KeyState::Hold))
        {
            camera->WorldUpDown(distance);
        }
    }

    /// 키입력 -> 카메라 회전
    if (_inputManager->IsKeyState(Key::RMouse, KeyState::Hold))
    {
        float dx = (0.15f *
                    static_cast<float>(_inputManager->GetDeltaMousePosition()
                                           .x) /* * (3.141592 / 180)*/);
        float dy = (0.15f *
                    static_cast<float>(_inputManager->GetDeltaMousePosition()
                                           .y) /* * (3.141592 / 180)*/);

        camera->Pitch(dy);
        transform->AddYAxisRotation(ImpMath::DegreeToRadian(dx));
    }

    /// Entity로 카메라 이동처리
    if (_isArrived == false)
    {
        _elapsedTime += dt;

        Vector3 cameraPos = transform->GetLocalPosition();

        Vector3 dir = (_targetPosition - cameraPos).Normalize();
        cameraPos += dir * _cameraSpeed *
                     _engineModule->GetTimeManager()->GetDeltaTime();

        transform->SetPosition(cameraPos);

        if (_elapsedTime >= cameraToTargetSecond)
        {
            _isArrived = true;
            transform->SetPosition(_targetPosition);
        }
    }

    if( ImGui::Begin("ToolCamera"))
    {
        Vector3 pos = transform->GetLocalPosition();

        float arr[3] = { pos.x, pos.y, pos.z };

        ImGui::DragFloat3("Pos", arr);

        Quaternion q = transform->GetRotation();

        float q4[4] = { q.x, q.y, q.z, q.w };
        
        ImGui::DragFloat4("quternion", q4);

        std::string posRot = "";
        posRot += std::to_string(pos.x) + "@" + std::to_string(pos.y) + "@" +
                  std::to_string(pos.z) + "@" + std::to_string(q.x) + "@" +
                  std::to_string(q.y) + "@" + std::to_string(q.z) + "@" +
                  std::to_string(q.w);

        if (ImGui::Button("save"))
        {
            std::ofstream ofs("Resources\\CameraWalk\\Points\\" + posRot);
            //std::ofstream ofs("E:\GA5th4Q_Project\EmperorPenguin\5_Project\ImpEngineDemo\Resources\CameraWalk\Points\123456" /*+ posRot*/);
            //std::ofstream ofs("E:\GA5th4Q_Project\EmperorPenguin\5_Project\ImpEngineDemo\Resources\CameraWalk\Points\\" + posRot, std::ios::out | std::ios::app);
        }

        ImGui::End();
    }

}

void ImpToolModule::ToolCamera::Finalize()
{
}

void ImpToolModule::ToolCamera::LookAt(ImpMath::Vector3 target, float distance)
{

    auto q = _ent->GetComponent<ImpEngineModule::Transform>()->GetRotation();

    ImpMath::Matrix rotM = ImpMath::Matrix::MakeFromQuaternion(q);
    ImpMath::Vector3 zAxis(rotM._31, rotM._32, rotM._33);
    _targetPosition = target + zAxis * -distance;

    ImpEngineModule::Transform*
        transform = _ent->GetComponent<ImpEngineModule::Transform>();
    Vector3 pos   = transform->GetLocalPosition();

    float length = (pos - _targetPosition).Length();

    if (length < 1.f)
    {
        transform->SetPosition(_targetPosition);
        return;
    }

    _cameraSpeed = length / cameraToTargetSecond; // 0.25 sec


    _isArrived   = false;
    _elapsedTime = 0.f;
}

bool ImpToolModule::ToolCamera::OnCamera() const
{
    if (_editor->GetMode() == ToolMode::EditorMode)
    {
        return true;
    }

    return false;
}


DirectX::XMFLOAT4X4 ImpToolModule::ToolCamera::GetViewTM() const
{
    return _viewTM;
}


DirectX::XMFLOAT4X4 ImpToolModule::ToolCamera::GetProjTM() const
{
    return _projTM;
}

void ImpToolModule::ToolCamera::UpdateMatrix()
{
    auto camera = _ent->GetComponent<ImpEngineModule::Camera>();
    auto mat = _ent->GetComponent<ImpEngineModule::Transform>()->GetLocalMatrix();

    using namespace DirectX;

    XMMATRIX trasformMatirxXM(&mat._11);
    DirectX::XMMATRIX viewTM(trasformMatirxXM);

    auto rect = _inputManager->GetScreenRect();

    DirectX::XMMATRIX PerspecProjTM = DirectX::XMMatrixPerspectiveFovLH(
        camera->GetFieldOfView(),
        static_cast<float>(std::abs(rect.right - rect.left)) /
            std::abs((rect.bottom - rect.top)),
        camera->GetNearPlain(),
        camera->GerFarPlain());

   //DirectX::XMMatrixTranspose(PerspecProjTM);
   // DirectX::XMMatrixTranspose(viewTM);

    XMStoreFloat4x4(&_viewTM, XMMatrixInverse(nullptr, viewTM));
    XMStoreFloat4x4(&_projTM, PerspecProjTM);
}
