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

    if (playerMovement == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        playerMovement = new PlayerMovement(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
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

    // ������ ���Ѱ�쿡�� �÷��̾��� �������� �����Ѵ�.
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

            // ���� ��� ��ȯ �� z �ӵ��� ������ �������� ������� ��ȯ
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

            // �������� ���� �� OnGround ��� Idel �� ��ȯ
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
    // ���� �÷��̾ ��ŷ���ΰ�� ��ŷ�� ����Ѵ�.
    if (_qte != nullptr)
    {
        _qte->ExitQTE();
    }

    // QTE�� �����Ѵ�.
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

    // ����ȭ ���¸� �����Ѵ�
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

    // ��ƽ������ ���ϴ� ��� ���� ������ �ۿ�
    if (x == 0.f && y == 0.f)
    {
        currentV.x *= std::exp(stopFriction * dt);
        currentV.z *= std::exp(stopFriction * dt);
        _rigidBody->SetVelocity(currentV);

        // ���߻��� �Ǵ�
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
        // ���߻��� ����
        _focusTime = 0.f;

        Matrix cameraRotM = Matrix::MakeFromQuaternion(
            _camera->GetComponent<ImpEngineModule::Transform>()->GetRotation());

        Vector3 cameraX = Vector3(cameraRotM._11, 0.f, cameraRotM._13).Normalize();
        Vector3 cameraZ = Vector3(cameraRotM._31, 0.f, cameraRotM._33).Normalize();

        // �÷��̾� �̵� ����
        Vector3 addV{};
        addV += cameraZ * _moveSpeed * y * dt;
        addV += cameraX * _moveSpeed * x * dt;

        // ������ȯ�� �����ش�
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

        // �÷��̾� �ü� ����
        _playerDirection = addV.Normalize();

        // �÷��̾� �ִϸ��̼� ������ 180�� ȸ�������̹Ƿ�
        Quaternion rot = Quaternion::MakeLookRotation(
            -_playerDirection, Vector3::UnitY);
        _transform->SetRotation(rot);

        _state->SetMoveState(PlayerState::MoveState::Walk);
    }
}


void ImpClient::PlayerMovement::JumpPlayer(float dt,
    ImpEngineModule::InputManager* input)
{
    // ��ŷ���̸� �������� ���Ѵ�.
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
