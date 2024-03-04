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

    if (playerAttack == nullptr) // 새로 생성해서 복사본을 준다
    {
        playerAttack = new PlayerAttack(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *playerAttack = *this;
    }

    return playerAttack;
}


void ImpClient::PlayerAttack::OnTriggerEnter(
    const ImpEngineModule::Physics::Collision& collision)
{
    ImpEngineModule::Collider* otherCollider = collision.GetOtherCollider(
        GetEntity());

    // 상대 플레이어 감지
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

    // 상대 플레이어 감지
    if (otherCollider->GetTag() == ImpEngineModule::Physics::Tag::Player)
    {
        // 만일 게임이 끝날 때의 상태일 때 취소 (2024-02-21 김주영)
        PlayerState* state = otherCollider->GetComponent<PlayerState>();
        if (state->GetMoveState() == PlayerState::MoveState::Dance ||
            state->GetMoveState() == PlayerState::MoveState::Defeat)
            return;

        auto input = GetWorld()->GetInputManager();

        _vibrationElapsedTime += collision.dt;

        // 진동 On
        if (_vibrationElapsedTime <= _vibrationTerm)
        {
            input->SetVibration(
                _padID, VibrationMode::Both, 50000, _dectectVibration._second);
        }
        else // 진동 Off
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

    // 플레이어 공격
    if (input->IsPadKeyState(_padID, PadKey::Y, KeyState::Away) &&
        _state->GetActionState() == PlayerState::ActionState::Idle &&
        _attackElapsedTime == _attackCoolTime &&
        _movement->GetStopInputTime() == 0.f)
    {
        Attack();

        // 공격 사운드 재생
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

    // 공격 이펙트 추가
    auto empRes = GetWorld()->GetResourceManager()->GetEntityResource(_empPrefab);
    Entity* empEnt = GetWorld()->RegisterEntity(empRes->GetEntity());
    empEnt->GetComponent<EMP>()->Set(this);

    // 공격 상태로 전환 (2024-01-19 김주영)
    _state->SetActionState(PlayerState::ActionState::Attack);

    // 공격시에 상대방 플레이어가 범위안에 있는 경우 공격
    if (_otherPlayer == nullptr) return;

    auto otherPlayerT = _otherPlayer->GetComponent<Transform>();

    Vector3 otherPos = otherPlayerT->GetWorldPostiion();
    Vector3 pos      = GetComponent<Transform>()->GetWorldPostiion();

    float distance = (otherPos - pos).Length();

    // 공격 성공
    if (_attackRange >= distance)
    {
        // 피격 사운드 재생
        auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

        size_t audioIndex = audioClip->GetAudioIndex(L"SE_EMPHit");
        audioClip->PlaySound(audioIndex, false, audioIndex);

        _otherPlayer->GetComponent<PlayerMovement>()->Attacked();

        // 상대방의 화살표를 스틸한다.
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
