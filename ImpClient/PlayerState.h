#pragma once

namespace ImpClient
{
class PlayerMovement;

/// <summary>
/// �÷��̾��� ���¸� �����ϴ� ������Ʈ
/// </summary>
class PlayerState: public ImpEngineModule::Component
{
public:
    // �̵� ����
    enum class MoveState
    {
        Idle,
        Walk,
        Foucs, // ���߻���
        Dance,
        Swing,
        Defeat
    };

    // �ൿ����
    enum class ActionState
    {
        Idle,
        Jump,   // ����
        Falling,
        Flying,
        Land,
        Hack,   // ��ŷ��
        Attack, // ����
        Sturn,  // ����
    };


    //
    // �ִϸ��̼� Ŭ���� �ִϸ��̼� �ε��� (2024-02-19 ���ֿ�)
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
//    �ִϸ��̼� ����(state) �Ӹ� �ƴ϶�
//    ���¿� ���� �ִϸ��̼� ������ ���Ѵ�.
//