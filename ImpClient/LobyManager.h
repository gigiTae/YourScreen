#pragma once


namespace ImpClient
{

/// <summary>
/// Loby씬을 관리하는 매니져
/// </summary>
class LobyManager: public ImpEngineModule::Component
{
public:
    LobyManager();
    ~LobyManager();

    ImpEngineModule::Component* Clone(Component* clone) const override;

    void Update(float dt) override;

    bool IsLeftReady();
    bool IsRightReady();

    void SetLeftReady();
    void SetRightReady();

private:
    void MakeUI();

    // 게임 시작
    void StartGame();

    // 게임 종료
    void EndGame();

private:
    bool _isStart;

    bool _isLeftPlayerReady;
    bool _isRightPlayerReady;

    std::wstring _readUIPrefab;

    ImpEngineModule::Entity* _leftReadyUI;
    ImpEngineModule::Entity* _rightReadyUI;

    REFLECTION_FRIEND(LobyManager);
};

} // namespace ImpClient