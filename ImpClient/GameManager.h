#pragma once
#pragma once

namespace ImpClient
{
/// <summary>
/// ��ü���� ������ �����ϴ� ������Ʈ
/// </summary>
class GameManager: public ImpEngineModule::Component
{
public:
    GameManager();
    ~GameManager();

    Component* Clone(Component* clone) const override;

    void Start() override;

    void Update(float dt) override;

    ImpEngineModule::Entity* GetLeftPlayer() const
    {
        return _leftPlayer;
    }

    ImpEngineModule::Entity* GetRightPlayer() const
    {
        return _rightPlayer;
    }

    void AddLeftPlayerHackingCount();
    void AddRightPlayerHackingCount();

    ImpEngineModule::EntityResource* GetQTEResource();

    short GetLeftHackingCount() const
    {
        return _leftHackingCount;
    }

    short GetRighttHackingCount() const
    {
        return _rightHackingCount;
    }

    ImpEngineModule::Entity* GetLeftHackableUI();
    ImpEngineModule::Entity* GetRightHackableUI();

private:
    // ��ŷ�ؾ��ϴ� ������Ʈ���� �����Ѵ�.
    void SelectHackingPoints();

    // �÷��̵鿡�� ��ŷ ȭ��ǥ�� �߰��Ѵ�.
    void AddHackingArrow();

    void LoadQTEResource();

    void LoadHackableUI();

    // ���� ����
    void EndGame();

    void LoadHackingCountUI();

private:
    bool _isEnd;
    bool _isStart;

    ImpEngineModule::World* _world;
    std::vector<ImpEngineModule::Entity*> _hackableObjects; // ��ŷ������ ������Ʈ��
    std::vector<ImpEngineModule::Entity*> _hackingPoints; // ��ŷ�ؾ��ϴ� ������Ʈ��
    std::vector<ImpEngineModule::Entity*> _hackingArrows; // ��ŷȭ��ǥ��

    std::string _leftPlayerName;
    std::string _rightPlayerName;
    
    short _leftHackingCount;
    short _rightHackingCount;
    
    ImpEngineModule::Entity* _leftPlayer;
    ImpEngineModule::Entity* _rightPlayer;

    ImpEngineModule::EntityResource* _hackingArrowResource;
    std::wstring _hackingArrowPrefab;

    ImpEngineModule::EntityResource* _qteResource;
    std::wstring _qtePrefab;

    std::wstring _hackingCountPrefab;
    ImpEngineModule::Entity* _leftHackingCountUI;
    ImpEngineModule::Entity* _rightHackingCountUI;

    std::wstring _hackableUIPrefab;
    ImpEngineModule::Entity* _leftHackableUI;
    ImpEngineModule::Entity* _rightHackableUI;

    REFLECTION_FRIEND(GameManager);

private: // ���� �Ϸ� ���� (2024-02-21 ���ֿ�)
    void Vicory(ImpEngineModule::Entity* winner);
    void Defeat(ImpEngineModule::Entity* loser);
};

} // namespace ImpClient