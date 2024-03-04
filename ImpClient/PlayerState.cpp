#include "ImpClientPCH.h"
#include "ImpClientPCH.h"
#include "PlayerState.h"
#include "PlayerMovement.h"
#include "PlayerAnimationSpeed.h"

ImpClient::PlayerState::PlayerState()
    : _moveState(MoveState::Idle)
    , _actionState(ActionState::Idle)
    , _animationClip(nullptr)
    , _rigid(nullptr), _isPlay(true)
{
}

ImpClient::PlayerState::~PlayerState()
{
}

void ImpClient::PlayerState::Start()
{
    _animationClip = GetComponent<AnimationClip>();
    _rigid         = GetComponent<Rigidbody>();
    _movement      = GetComponent<PlayerMovement>();

    // 게임 시작 시 Idle 상태도록 설정(2024-02-19 김주영)
    const auto& animations = _animationClip->GetAnimations();
    _animationClip->PlayAnimation(animations[(int)AnimClipIndex::Idle]);
}

void ImpClient::PlayerState::Update(float dt)
{
    // 액션 스테이트에 따른 애니메이션 재생(2024-02-19 김주영)
    if (_isPlay)
    {
        UpdateAimation();
        UpdateAnimationSpeed(dt);
    }
}

ImpEngineModule::Component* ImpClient::PlayerState::Clone(Component* clone) const
{
    PlayerState* playerState = static_cast<PlayerState*>(clone);

    if (playerState == nullptr) // 새로 생성해서 복사본을 준다
    {
        playerState = new PlayerState(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *playerState = *this;
    }

    return playerState;
}

ImpClient::PlayerState::MoveState ImpClient::PlayerState::GetMoveState() const
{
    return _moveState;
}

void ImpClient::PlayerState::SetMoveState(MoveState state)
{
    _moveState = state;
}

ImpClient::PlayerState::ActionState ImpClient::PlayerState::GetActionState() const
{
    return _actionState;
}

void ImpClient::PlayerState::SetActionState(ActionState state)
{
    _actionState = state;
}


void ImpClient::PlayerState::CheckOff()
{
    _isPlay = false;
}

void ImpClient::PlayerState::UpdateAnimationSpeed(float dt)
{
    const auto& animations = _animationClip->GetAnimations();

    switch (_actionState)
    {
        case ImpClient::PlayerState::ActionState::Idle:
        {
            switch (_moveState)
            {
                case MoveState::Idle:
                {
                    _animationClip->SetPlaybackSpeed(1.f);
                }
                break;
                case MoveState::Walk:
                {
                    ImpMath::Vector3 vel = _rigid->GetVelocity();

                    float scala     = _rigid->GetVelocity().Length();
                    float ainmSpeed = _animationClip->GetPlaybackSpeed();

                    if (scala > 8.f && vel.y < 1.f && ainmSpeed < 2.4f)
                    {
                        _animationClip->SetPlaybackSpeed(ainmSpeed + dt );
                    }
                    else if (/*scala > 6.f && */ vel.y < 1.f && ainmSpeed > 0.5f)
                    {
                        _animationClip->SetPlaybackSpeed(ainmSpeed - dt);
                    }
                    else if (scala < 0.0001f)
                    {
                        _animationClip->SetPlaybackSpeed(0.1f);
                    }
                }
                break;
                case MoveState::Foucs:
                {
                    if (_actionState != ImpClient::PlayerState::ActionState::Idle)
                        return;
                    _animationClip->SetPlaybackSpeed(1.f);
                }
                break;
                case MoveState::Dance:
                {
                    _animationClip->SetPlaybackSpeed(1.f);
                }
                break;
                case MoveState::Swing:
                {
                    _animationClip->SetPlaybackSpeed(1.f);
                }
                break;
                default:
                    break;
            }
        }
        break;
        case ImpClient::PlayerState::ActionState::Jump:
        {
            _animationClip->SetPlaybackSpeed(2.f);
        }
        break;
        case ImpClient::PlayerState::ActionState::Hack:
        {
            _animationClip->SetPlaybackSpeed(2.f);
        }
        break;
        case ImpClient::PlayerState::ActionState::Attack:
        {
            _animationClip->SetPlaybackSpeed(2.4f);
        }
        break;
        case ImpClient::PlayerState::ActionState::Sturn:
        {
            _animationClip->SetPlaybackSpeed(1.f);
        }
        break;
        case ImpClient::PlayerState::ActionState::Falling:
        {
            _animationClip->SetPlaybackSpeed(2.f);
        }
        break;
        case ImpClient::PlayerState::ActionState::Flying:
        {
            _animationClip->SetPlaybackSpeed(1.f);
        }
        break;
        default:
            break;
    }
}


void ImpClient::PlayerState::UpdateAimation()
{
    const auto& animations = _animationClip->GetAnimations();
    int idx{ 0 };

    switch (_actionState)
    {
        case ImpClient::PlayerState::ActionState::Idle:
        {
            switch (_moveState)
            {
                case MoveState::Idle:
                {
                    idx = (int)AnimClipIndex::Idle;
                    _animationClip->PlayAnimation(idx);
                }
                break;
                case MoveState::Walk:
                {
                    _animationClip->PlayAnimation(animations[0]);
                }
                break;
                case MoveState::Foucs:
                {
                    if (_actionState != ImpClient::PlayerState::ActionState::Idle)
                        return;

                    idx = (int)AnimClipIndex::KneelingPointing;
                    _animationClip->PlayAnimation(idx);
                }
                break;
                case MoveState::Dance:
                {
                    idx = (int)AnimClipIndex::HouseDancing;
                    _animationClip->PlayAnimationNoneLoop(idx, idx);
                }
                break;
                case MoveState::Swing:
                {
                    idx = (int)AnimClipIndex::SwingToLand;
                    _animationClip->PlayAnimationNoneLoop(idx, idx);
                }
                break;
                case MoveState::Defeat:
                {
                    idx = (int)AnimClipIndex::ZombiStumbling;
                    _animationClip->PlayAnimationNoneLoop(idx, idx);
                }
                break;
                default:
                    break;
            }
        }
        break;
        case ImpClient::PlayerState::ActionState::Jump:
        case ImpClient::PlayerState::ActionState::Falling:
        {
            idx = (int)AnimClipIndex::FallingIdle;
            _animationClip->PlayAnimation(idx);
        }
        break;
        case ImpClient::PlayerState::ActionState::Hack:
        {
            idx = (int)AnimClipIndex::MagicHeal;
            _animationClip->PlayAnimation(animations[idx]);
        }
        break;
        case ImpClient::PlayerState::ActionState::Attack:
        {
            idx = (int)AnimClipIndex::MagicAreaAttack;
            _animationClip->PlayAnimation(animations[idx]);
        }
        break;
        case ImpClient::PlayerState::ActionState::Sturn:
        {
            idx = (int)AnimClipIndex::BeingElectrocuted;
            _animationClip->PlayAnimation(animations[idx]);
        }
        break;
        break;
        case ImpClient::PlayerState::ActionState::Flying:
        {
            idx = (int)AnimClipIndex::Falling;
            _animationClip->PlayAnimation(animations[idx]);
        }
        break;
        default:
            break;
    }
}
