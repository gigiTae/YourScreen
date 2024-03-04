#pragma once
#pragma once

namespace ImpClient
{
/// <summary>
/// 전체적인 게임을 관리하는 컴포넌트
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
    // 해킹해야하는 오브젝트들을 결정한다.
    void SelectHackingPoints();

    // 플레이들에게 해킹 화살표를 추가한다.
    void AddHackingArrow();

    void LoadQTEResource();

    void LoadHackableUI();

    // 게임 종료
    void EndGame();

    void LoadHackingCountUI();

private:
    bool _isEnd;
    bool _isStart;

    ImpEngineModule::World* _world;
    std::vector<ImpEngineModule::Entity*> _hackableObjects; // 해킹가능한 오브젝트들
    std::vector<ImpEngineModule::Entity*> _hackingPoints; // 해킹해야하는 오브젝트들
    std::vector<ImpEngineModule::Entity*> _hackingArrows; // 해킹화살표들

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

private: // 게임 완료 연출 (2024-02-21 김주영)
    void Vicory(ImpEngineModule::Entity* winner);
    void Defeat(ImpEngineModule::Entity* loser);
};

} // namespace ImpClient