#pragma once


namespace ImpClient
{
class PlayerAttack;
class PlayerState;

/// <summary>
/// 공격의 정보를 표시하는 UI
/// </summary>
class AttackUI: public ImpEngineModule::Component
{
public:
    AttackUI();
    ~AttackUI();

    ImpEngineModule::Component* Clone(
        Component* clone /* = nullptr */) const override;

    void Update(float dt) override;

    void Set(ImpEngineModule::Entity* player);

private:
    unsigned int _padID;
    PlayerAttack* _playerAttack;
    PlayerState* _playerState;

    ImpEngineModule::UI* _ui;
    ImpEngineModule::UI* _blackScreenUI;
    ImpEngineModule::UI* _whiteScreenUI;

    std::wstring _blackScreenPrefab;
    std::wstring _whiteScreenPrefab;

    REFLECTION_FRIEND(AttackUI);
};

} // namespace ImpClient