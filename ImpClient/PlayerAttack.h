#pragma once

namespace ImpClient
{
class PlayerState;
class PlayerTracker;
class PlayerMovement;

/// <summary>
/// 플레이어의 공격을 담당하는 컴포넌트
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

    // 공격 경과시간 반환
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

    // 해킹스킬을 사용가능한지
    bool CanUseHackSkill();

    // 해킹스킬 사용
    void UseHackSkill();

    // 해킹 범위증가
    void IncreaseHackingRange(float addRange);

    // 해킹쿨타임 감소
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