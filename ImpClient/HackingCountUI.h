#pragma once

namespace ImpClient
{

class HackableObject;

/// <summary>
/// 해킹 카운트를 나타내는 UI
/// </summary>
class HackingCountUI: ImpEngineModule::Component
{
public:
    HackingCountUI();
    ~HackingCountUI();

    ImpEngineModule::Component* Clone(Component* clone) const override;
    
    void SetHackingUI(ImpEngineModule::Entity* player,
        const std::vector<ImpEngineModule::Entity*>& hackingObject);
    void Update(float dt) override;

private:
    void MakeBigHackingChip();
    void UpdateSmallHackingChip();
    void UpdateBigHackingChip();
    void ChangeToBigChip();

private:
    bool _isLastPoint;
    bool _isLeftPlayer;
    std::vector<ImpEngineModule::Entity*> _hackingObjects;
    std::vector<ImpEngineModule::UI*> _hackingChips;

    std::wstring _hackingChipPrefab;
    std::wstring _bigHackingChipPrefab;

    std::wstring _attackUIPrefab;
    ImpEngineModule::Entity* _attackUIEntity;

    std::wstring _hackSkillUIPrefab;
    ImpEngineModule::Entity* _hackSkillUIEntity;

    REFLECTION_FRIEND(HackingCountUI)
};

} // namespace ImpClient