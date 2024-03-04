#pragma once

namespace ImpClient
{
class PlayerState;
class PlayerTracker;
class PlayerMovement;

/// <summary>
/// �÷��̾��� ������ ����ϴ� ������Ʈ
/// </summary>
class PlayerAttack: public ImpEngineModule::Component
{
public:
    PlayerAttack();
    ~PlayerAttack();

    Component* Clone(Component* clone = nullptr) const override;

    void OnTriggerEnter(
        const ImpEngineModule::Physics::Collision& collision) override;

    void OnTriggerStay(const ImpEngineModule::Physics::Collision& collision) override;

    void OnTriggerExit(const ImpEngineModule::Physics::Collision& collision) override;

    void Start() override;

    void Update(float dt) override;

    // ���� ����ð� ��ȯ
    float GetAttackElapsedTime();

    float GetAttackRange() const
    {
        return _attackRange;
    }

    float GetAttackCoolTime() const
    {
        return _attackCoolTime;
    }

	float GetHackingCoolTime() const
    {
        return _hackingCoolTime;
    }
    void SetHackingCoolTime(float val)
    {
        _hackingCoolTime = val;
    }

	float GetHackingElapsedTime() const
    {
        return _hackingElapsedTime;
    }

    // ��ŷ��ų�� ��밡������
    bool CanUseHackSkill();

    // ��ŷ��ų ���
    void UseHackSkill();

    // ��ŷ ��������
    void IncreaseHackingRange(float addRange);

    // ��ŷ��Ÿ�� ����
    void DecreaseHackingCoolTime(float decreaseCoolTime);

private:
    void Attack();

private:
    unsigned int _padID;

    float _vibrationTerm;
    float _vibrationElapsedTime;

    float _attackRange;
    float _dectectionRange;

    float _attackElapsedTime;
    float _attackCoolTime = 30.f;

    float _hackingCoolTime = 7.f;
    float _hackingElapsedTime;

    float _attackActiveTime;

    ImpEngineModule::InputManager::Vibration _dectectVibration;
    ImpEngineModule::Entity* _otherPlayer;

    PlayerMovement* _movement;
    PlayerState* _state;
    PlayerTracker* _playerTracker;

    std::wstring _empPrefab;

    REFLECTION_FRIEND(PlayerAttack);
};

} // namespace ImpClient