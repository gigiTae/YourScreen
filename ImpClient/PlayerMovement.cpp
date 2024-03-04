#include "ImpClientPCH.h"
#include "PlayerMovement.h"
#include "QTE.h"
#include "GamePad.h"
#include "PlayerState.h"
#include "GameManager.h"

ImpClient::PlayerMovement::PlayerMovement()
    : _padID(-1)
    , _moveSpeed(30.f)
    , _jumpPower(5000.f)
    , _transform(nullptr)
    , _rigidBody(nullptr)
    , _state(nullptr)
    , _playerDirection{}
    , _camera(nullptr)
    , _qte(nullptr)
    , _focusTime(0.f)
    , _maxMoveSpeed(20.f)
    , _stopInputTime(0.f)
{
}

ImpClient::PlayerMovement::~PlayerMovement()
{
}

ImpEngineModule::Component* ImpClient::PlayerMovement::Clone(
    Component* clone /*= nullptr*/) const
{
    PlayerMovement* playerMovement = static_cast<PlayerMovement*>(clone);

    if (playerMovement == nullptr) // 새로 생성해서 복사본을 준다
    {
        playerMovement = new PlayerMovement(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *playerMovement = *this;
    }

    return playerMovement;
}


void ImpClient::PlayerMovement::Start()
{
    _transform = GetComponent<ImpEngineModule::Transform>();
    _rigidBody = GetComponent<ImpEngineModule::Rigidbody>();
    _padID     = GetComponent<GamePad>()->GetPadID();
    _state     = GetComponent<PlayerState>();

    _camera = GetWorld()->GetEntity(_cameraName);
}

void ImpClient::PlayerMovement::Update(float dt)
{
    _stopInputTime = std::max(_stopInputTime - dt, 0.f);

    using namespace ImpMath;

    auto input = GetWorld()->GetInputManager();

    // 공격을 당한경우에는 플레이어의 움직임을 제한한다.
    if (_state->GetActionState() == PlayerState::ActionState::Sturn ||
        _state->GetMoveState() == PlayerState::MoveState::Dance ||
        _state->GetMoveState() == PlayerState::MoveState::Defeat)
    {
        return;
    }

    switch (_state->GetActionState())
    {
        case PlayerState::ActionState::Jump:
        case PlayerState::ActionState::Flying:
        {
            _focusTime = 0.f;

            // 점프 모션 전환 후 z 속도가 음수면 떨어지는 모션으로 전환
            auto velocity = _rigidBody->GetVelocity();
            if (velocity.y < 0.f)
                _state->SetActionState(PlayerState::ActionState::Falling);
        }
        break;
        case PlayerState::ActionState::Hack:
        {
            _focusTime = 0.f;
        }
        break;
        case PlayerState::ActionState::Attack:
        {
            _focusTime = 0.f;
        }
        break;
        case PlayerState::ActionState::Falling:
        {
            _focusTime = 0.f;

            // 덜어지는 중일 때 OnGround 라면 Idel 로 전환
            if (_rigidBody->OnGround())
            {
                _state->SetActionState(PlayerState::ActionState::Idle);
            }
        }
        break;
        default:
            break;
    }

    MovePlayer(dt, input);
    JumpPlayer(dt, input);
}

void ImpClient::PlayerMovement::Attacked()
{
    // 만약 플레이어가 해킹중인경우 해킹을 취소한다.
    if (_qte != nullptr)
    {
        _qte->ExitQTE();
    }

    // QTE를 생성한다.
    Entity* gameMgrEnt = GetWorld()->GetEntity("GameManager");
    if (gameMgrEnt == nullptr)
    {
        return;
    }

    _state->SetActionState(PlayerState::ActionState::Sturn);
    auto gameMgr = gameMgrEnt->GetComponent<GameManager>();

    Entity* qteEntity = GetWorld()->RegisterEntity(
        gameMgr->GetQTEResource()->GetEntity());
    _qte = qteEntity->GetComponent<QTE>();
    _qte->Set(GetEntity(), nullptr);

    // 투명화 상태를 해제한다
    auto mesh = GetComponent<SkeletalMesh>();

    if (mesh != nullptr)
    {
        mesh->SetRenderType(SkeletalMesh::RenderType::Default);
    }
}


void ImpClient::PlayerMovement::SetQTE(ImpClient::QTE* qte)
{
    _qte = qte;

    if (_qte == nullptr)
    {
        _stopInputTime = 0.25;
    }
}

void ImpClient::PlayerMovement::MovePlayer(float dt,
    ImpEngineModule::InputManager* input)
{
    using namespace ImpMath;
    constexpr float turnAssistPower = 5.f;
    constexpr float stopFriction    = -5.f;

    float x = input->GetStickInfomation(_padID, PadStick::leftX);
    float y = input->GetStickInfomation(_padID, PadStick::leftY);

    Vector3 currentV = _rigidBody->GetVelocity();

    // 스틱조작을 안하는 경우 정지 마찰력 작용
    if (x == 0.f && y == 0.f)
    {
        currentV.x *= std::exp(stopFriction * dt);
        currentV.z *= std::exp(stopFriction * dt);
        _rigidBody->SetVelocity(currentV);

        // 집중상태 판단
        _focusTime += dt;

        constexpr float foucsTime = 2.f;

        if (_focusTime >= foucsTime)
        {
            _state->SetMoveState(PlayerState::MoveState::Foucs);
        }
        else
        {
            _state->SetMoveState(PlayerState::MoveState::Idle);
        }
    }
    else
    {
        // 집중상태 해제
        _focusTime = 0.f;

        Matrix cameraRotM = Matrix::MakeFromQuaternion(
            _camera->GetComponent<ImpEngineModule::Transform>()->GetRotation());

        Vector3 cameraX = Vector3(cameraRotM._11, 0.f, cameraRotM._13).Normalize();
        Vector3 cameraZ = Vector3(cameraRotM._31, 0.f, cameraRotM._33).Normalize();

        // 플레이어 이동 관련
        Vector3 addV{};
        addV += cameraZ * _moveSpeed * y * dt;
        addV += cameraX * _moveSpeed * x * dt;

        // 방향전환을 도와준다
        if ((addV.x > 0.f && currentV.x < 0.f) || (addV.x < 0.f && currentV.x > 0.f))
        {
            addV.x *= turnAssistPower;
        }

        if ((addV.z > 0.f && currentV.z < 0.f) || (addV.z < 0.f && currentV.z > 0.f))
        {
            addV.z *= turnAssistPower;
        }

        if (_maxMoveSpeed > _rigidBody->GetVelocity().Length())
        {
            _rigidBody->AddVelocity(addV);
        }

        // 플레이어 시선 방향
        _playerDirection = addV.Normalize();

        // 플레이어 애니메이션 방향이 180도 회전상태이므로
        Quaternion rot = Quaternion::MakeLookRotation(
            -_playerDirection, Vector3::UnitY);
        _transform->SetRotation(rot);

        _state->SetMoveState(PlayerState::MoveState::Walk);
    }
}


void ImpClient::PlayerMovement::JumpPlayer(float dt,
    ImpEngineModule::InputManager* input)
{
    // 해킹중이면 점프하지 못한다.
    if (_state->GetActionState() == PlayerState::ActionState::Hack)
    {
        return;
    }

    if (_rigidBody->OnGround() &&
        _state->GetActionState() == PlayerState::ActionState::Jump)
    {
        _state->SetActionState(PlayerState::ActionState::Idle);
    }

    if (input->IsPadKeyState(_padID, PadKey::A, KeyState::Tap) &&
        _rigidBody->OnGround())
    {
        ImpMath::Vector3 v = _rigidBody->GetVelocity();
        v.y                = _jumpPower;
        _rigidBody->SetVelocity(v);

        _state->SetActionState(PlayerState::ActionState::Jump);
    }
}
