#pragma once

namespace ImpClient
{
class PlayerMovement;

/// <summary>
/// 플레이어의 상태를 관리하는 컴포넌트
/// </summary>
class PlayerState: public ImpEngineModule::Component
{
public:
    // 이동 관련
    enum class MoveState
    {
        Idle,
        Walk,
        Foucs, // 집중상태
        Dance,
        Swing,
        Defeat
    };

    // 행동관련
    enum class ActionState
    {
        Idle,
        Jump,   // 점프
        Falling,
        Flying,
        Land,
        Hack,   // 해킹중
        Attack, // 공격
        Sturn,  // 기절
    };


    //
    // 애니메이션 클립의 애니메이션 인덱스 (2024-02-19 김주영)
    //

    enum class AnimClipIndex
    {
        Walk = 0,
        Idle,
        KneelingPointing,
        MagicHeal,
        MagicAreaAttack,
        BeingElectrocuted,
        Falling,
        FallingIdle,
        ZombiStumbling,
        HouseDancing,
        SwingToLand,
    };

public:
    PlayerState();
    ~PlayerState();

    void Start() override;
    void Update(float dt) override;

    Component* Clone(Component* clone) const override;

    MoveState GetMoveState() const;
    void SetMoveState(MoveState state);

    ImpClient::PlayerState::ActionState GetActionState() const;
    void SetActionState(ImpClient::PlayerState::ActionState state);

    void CheckOff();

private:
    MoveState _moveState;
    ActionState _actionState;

    PlayerMovement* _movement;

    ImpEngineModule::AnimationClip* _animationClip;
    ImpEngineModule::Rigidbody* _rigid;

    bool _isPlay;

    REFLECTION_FRIEND(PlayerState);

private:
    void UpdateAimation();
    void UpdateAnimationSpeed(float dt);
};

} // namespace ImpClient


// Note
//    애니메이션 상태(state) 뿐만 아니라
//    상태에 따른 애니메이션 종류도 정한다.
//