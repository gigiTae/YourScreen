#pragma once


namespace ImpClient
{

/// <summary>
/// 게임패드 식별전용 컴포넌트
/// </summary>
class GamePad: public ImpEngineModule::Component
{
public:
    GamePad();
    ~GamePad();

    Component* Clone(Component* clone) const override;

    // 게임패드 식별자 반환S
    unsigned int GetPadID() const
    {
        return _padID;
    }

private:
    unsigned int _padID;

    REFLECTION_FRIEND(GamePad);
};

} // namespace ImpClient