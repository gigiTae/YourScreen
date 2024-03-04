#pragma once


namespace ImpClient
{
class PlayerState;

/// <summary>
/// 플레이어의 흔적을 추적하는 컴포넌트이다
/// </summary>
class PlayerTracker: public ImpEngineModule::Component
{
public:
    PlayerTracker();
    ~PlayerTracker();

    Component* Clone(Component* clone = nullptr) const override;

    void Update(float dt);

private:
    void Make();

    // 추적 On
    void On();

    // 추적 Off
    void Off();

private:
    static constexpr  float trackingTime = 5.f;
    static constexpr float timeStep     = 0.1f;

private:
    bool _startGame;
    bool _onTracking;

    std::wstring _trackingPointPrefab;
    std::vector<ImpEngineModule::Entity*> _trackingPoints;
    std::string _playerName;

    ImpEngineModule::Transform* _playerTransform;
    PlayerState* _playerState;

    size_t _currentIndex;

    float _elapsedTime;

    REFLECTION_FRIEND(PlayerTracker);
};
} // namespace ImpClient
