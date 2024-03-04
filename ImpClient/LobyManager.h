#pragma once


namespace ImpClient
{

/// <summary>
/// Loby���� �����ϴ� �Ŵ���
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

    // ���� ����
    void StartGame();

    // ���� ����
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