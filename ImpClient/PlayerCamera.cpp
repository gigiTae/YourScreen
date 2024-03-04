#include "ImpClientPCH.h"
#include "PlayerCamera.h"
#include "GamePad.h"

ImpClient::PlayerCamera::PlayerCamera()
    : _player(nullptr)
    , _playerToCameraDistance(20.f)
    , _padID(-1)
{
}

ImpClient::PlayerCamera::~PlayerCamera()
{
}

ImpEngineModule::Component* ImpClient::PlayerCamera::Clone(Component* clone) const
{
    PlayerCamera* camera = static_cast<PlayerCamera*>(clone);

    if (camera == nullptr)
    {
        camera = new PlayerCamera(*this);
    }
    else
    {
        *camera = *this;
    }

    return camera;
}

void ImpClient::PlayerCamera::Start()
{
    _player = GetWorld()->GetEntity(_playerName);

    if (_player == nullptr) return;
    _padID  = _player->GetComponent<GamePad>()->GetPadID();
}

void ImpClient::PlayerCamera::Update(float dt)
{
    using namespace ImpMath;
    assert(_player && "플레이어를 찾지 못했습니다");
    auto input = GetWorld()->GetInputManager();

    auto cameraT = GetComponent<ImpEngineModule::Transform>();
    auto playerT = _player->GetComponent<ImpEngineModule::Transform>();

    float x = input->GetStickInfomation(_padID, PadStick::rightX);
    float y = -input->GetStickInfomation(_padID, PadStick::rightY);

    // 카메라 회전
    Matrix rotM = Matrix::MakeFromQuaternion(cameraT->GetRotation());
    Vector3 zAxis{ rotM._31, rotM._32, rotM._33 };

    cameraT->AddYAxisRotation(x * dt);

    static const float degree70 = -std::sin(HalfPI * 7 / 9);
    static const float degree10 = -std::sin(HalfPI * 1 / 9);

    if (zAxis.y >= degree70 && y > 0.f)
    {
        cameraT->AddPitchRotation(y * dt);
    }
    else if (zAxis.y <= degree10 && y < 0.f)
    {
        cameraT->AddPitchRotation(y * dt);
    }

    ImpMath::Vector3 playerPos = playerT->GetWorldPostiion();

    // 카메라 회전값을 바탕으로 거리를 계산한다.
    // Matrix rotM = Matrix::MakeFromQuaternion(cameraT->GetRotation());

    ImpMath::Vector3 cameraPos = playerPos - zAxis * _playerToCameraDistance;
    cameraT->SetPosition(cameraPos);

    if (input->IsPadKeyState(_padID, PadKey::RightThumb, KeyState::Tap))
    {
        if (_playerToCameraDistance == 30.f)
        {
            _playerToCameraDistance = 20.f;
        }
        else if (_playerToCameraDistance == 20.f)
        {
            _playerToCameraDistance = 10.f;
        }
        else
        {
            _playerToCameraDistance = 30.f;
        }
    }
}

void ImpClient::PlayerCamera::RoatateCamera()
{
}
