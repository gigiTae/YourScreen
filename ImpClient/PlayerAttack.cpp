#include "ImpClientPCH.h"
#include "PlayerAttack.h"
#include "PlayerState.h"
#include "PlayerMovement.h"
#include "GamePad.h"
#include "HackingArrow.h"
#include "EMP.h"

ImpClient::PlayerAttack::PlayerAttack()
    : _attackRange(5.f)
    , _dectectionRange(10.f)
    , _padID(-1)
    , _otherPlayer(nullptr)
    , _attackElapsedTime(0.f)
    , _hackingElapsedTime(0.f)
    , _vibrationElapsedTime(0.f)
    , _vibrationTerm(0.25f)
    , _state(nullptr)
    , _empPrefab{}
    , _playerTracker{ nullptr }
    , _attackActiveTime(0.f)
    , _movement(nullptr)
{
    _attackElapsedTime  = _attackCoolTime;
    _hackingElapsedTime = _hackingCoolTime;
}

ImpClient::PlayerAttack::~PlayerAttack()
{
}

ImpEngineModule::Component* ImpClient::PlayerAttack::Clone(
    Component* clone /*= nullptr*/) const
{
    PlayerAttack* playerAttack = static_cast<PlayerAttack*>(clone);

    if (playerAttack == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        playerAttack = new PlayerAttack(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *playerAttack = *this;
    }

    return playerAttack;
}


void ImpClient::PlayerAttack::OnTriggerEnter(
    const ImpEngineModule::Physics::Collision& collision)
{
    ImpEngineModule::Collider* otherCollider = collision.GetOtherCollider(
        GetEntity());

    // ��� �÷��̾� ����
    if (otherCollider->GetTag() == ImpEngineModule::Physics::Tag::Player)
    {
        _otherPlayer = collision.GetOtherCollider(GetEntity())->GetEntity();
    }
}

void ImpClient::PlayerAttack::OnTriggerStay(
    const ImpEngineModule::Physics::Collision& collision)
{
    ImpEngineModule::Collider* otherCollider = collision.GetOtherCollider(
        GetEntity());

    // ��� �÷��̾� ����
    if (otherCollider->GetTag() == ImpEngineModule::Physics::Tag::Player)
    {
        // ���� ������ ���� ���� ������ �� ��� (2024-02-21 ���ֿ�)
        PlayerState* state = otherCollider->GetComponent<PlayerState>();
        if (state->GetMoveState() == PlayerState::MoveState::Dance ||
            state->GetMoveState() == PlayerState::MoveState::Defeat)
            return;

        auto input = GetWorld()->GetInputManager();

        _vibrationElapsedTime += collision.dt;

        // ���� On
        if (_vibrationElapsedTime <= _vibrationTerm)
        {
            input->SetVibration(
                _padID, VibrationMode::Both, 50000, _dectectVibration._second);
        }
        else // ���� Off
        {
            input->SetVibration(_padID, VibrationMode::Both, 0, 0.f);
            if (_vibrationElapsedTime >= _vibrationTerm * 4.f)
            {
                _vibrationElapsedTime = 0.f;
            }
        }
    }
}

void ImpClient::PlayerAttack::OnTriggerExit(
    const ImpEngineModule::Physics::Collision& collision)
{
    ImpEngineModule::Collider* otherCollider = collision.GetOtherCollider(
        GetEntity());

    if (otherCollider->GetTag() == ImpEngineModule::Physics::Tag::Player)
    {
        auto input = GetWorld()->GetInputManager();
        input->SetVibration(_padID, VibrationMode::Both, 0, 0.f);
        _otherPlayer = nullptr;
    }
}

void ImpClient::PlayerAttack::Start()
{
    GetComponent<ImpEngineModule::SphereCollider>()->SetoRadius(_dectectionRange);
    _padID    = GetComponent<GamePad>()->GetPadID();
    _state    = GetComponent<PlayerState>();
    _movement = GetComponent<PlayerMovement>();
}

void ImpClient::PlayerAttack::Update(float dt)
{
    auto input = GetWorld()->GetInputManager();

    if (_attackElapsedTime != _attackCoolTime)
    {
        if (_attackElapsedTime >= 1.f &&
            _state->GetActionState() == PlayerState::ActionState::Attack)
        {
            _state->SetActionState(PlayerState::ActionState::Idle);
        }

        _attackElapsedTime = std::clamp(
            _attackElapsedTime + dt, 0.f, _attackCoolTime);
    }

    if (_hackingCoolTime != _hackingElapsedTime)
    {
        _hackingElapsedTime = std::clamp(
            _hackingElapsedTime + dt, 0.f, _hackingCoolTime);
    }

    // �÷��̾� ����
    if (input->IsPadKeyState(_padID, PadKey::Y, KeyState::Away) &&
        _state->GetActionState() == PlayerState::ActionState::Idle &&
        _attackElapsedTime == _attackCoolTime &&
        _movement->GetStopInputTime() == 0.f)
    {
        Attack();

        // ���� ���� ���
        auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

        size_t audioIndex = audioClip->GetAudioIndex(L"SE_EMPAttack");
        audioClip->PlaySound(audioIndex, false, audioIndex);
    }
}

float ImpClient::PlayerAttack::GetAttackElapsedTime()
{
    return _attackElapsedTime;
}

bool ImpClient::PlayerAttack::CanUseHackSkill()
{
    if (_hackingCoolTime <= _hackingElapsedTime)
    {
        return true;
    }

    return false;
}

void ImpClient::PlayerAttack::UseHackSkill()
{
    _hackingElapsedTime = 0.f;
}

void ImpClient::PlayerAttack::IncreaseHackingRange(float addRange)
{
    _attackRange += addRange;
}

void ImpClient::PlayerAttack::DecreaseHackingCoolTime(float decreaseCoolTime)
{
    _attackCoolTime -= decreaseCoolTime;
}

void ImpClient::PlayerAttack::Attack()
{
    _attackElapsedTime = 0.f;

    // ���� ����Ʈ �߰�
    auto empRes = GetWorld()->GetResourceManager()->GetEntityResource(_empPrefab);
    Entity* empEnt = GetWorld()->RegisterEntity(empRes->GetEntity());
    empEnt->GetComponent<EMP>()->Set(this);

    // ���� ���·� ��ȯ (2024-01-19 ���ֿ�)
    _state->SetActionState(PlayerState::ActionState::Attack);

    // ���ݽÿ� ���� �÷��̾ �����ȿ� �ִ� ��� ����
    if (_otherPlayer == nullptr) return;

    auto otherPlayerT = _otherPlayer->GetComponent<Transform>();

    Vector3 otherPos = otherPlayerT->GetWorldPostiion();
    Vector3 pos      = GetComponent<Transform>()->GetWorldPostiion();

    float distance = (otherPos - pos).Length();

    // ���� ����
    if (_attackRange >= distance)
    {
        // �ǰ� ���� ���
        auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

        size_t audioIndex = audioClip->GetAudioIndex(L"SE_EMPHit");
        audioClip->PlaySound(audioIndex, false, audioIndex);

        _otherPlayer->GetComponent<PlayerMovement>()->Attacked();

        // ������ ȭ��ǥ�� ��ƿ�Ѵ�.
        auto& otherChildren = otherPlayerT->GetChildren();

        for (auto child : otherChildren)
        {
            HackingArrow* arrow = child->GetComponent<HackingArrow>();

            if (arrow != nullptr)
            {
                arrow->Steal(GetEntity());
            }
        }
    }
}
