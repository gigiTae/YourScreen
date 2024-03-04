#pragma once

namespace ImpClient
{
class PlayerAttack;
class PlayerState;

// 해킹 스킬 UI
class HackSkillUI: public ImpEngineModule::Component
{
public:
    HackSkillUI();
    ~HackSkillUI();

    ImpEngineModule::Component* Clone(
        Component* clone /* = nullptr */) const override;

    void Update(float dt) override;

    void Set(ImpEngineModule::Entity* player);
private:
    static constexpr float startY = 44.f;

    unsigned int _padID;
    PlayerAttack* _playerAttack;
    PlayerState* _playerState;

    ImpEngineModule::UI* _ui;
    ImpEngineModule::UI* _blackScreenUI;

    std::wstring _blackScreenPrefab;

    REFLECTION_FRIEND(HackSkillUI);
};
} // namespace ImpClient
