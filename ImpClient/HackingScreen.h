#pragma once

namespace ImpClient
{

/// <summary>
/// 해킹 스크린 컴포넌트
/// </summary>
class HackingScreen: public ImpEngineModule::Component
{
public:
    HackingScreen();
    ~HackingScreen();

    Component* Clone(Component* clone = nullptr) const override;

    void Update(float dt) override;

    void SetIsLeft(bool isLeft)
    {
        _isLeft = isLeft;
    }

private:
    void SetUp();

private:
    bool _isLeft;
    bool _isSet;
    bool _onNoise;

    std::wstring _hackingScreen2Prefab;
    ImpEngineModule::UI* _hackingScreenUI2;
    ImpEngineModule::UI* _hackingScreenUI1;

    bool _onSecondScreen;
    float _elapsedTime;

    REFLECTION_FRIEND(HackingScreen);
};

} // namespace ImpClient