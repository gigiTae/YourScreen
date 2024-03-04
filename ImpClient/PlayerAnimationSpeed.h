#pragma once


namespace ImpClient
{
/// <summary>
/// 플레이어 애니메이션 속도지정
/// </summary>
class PlayerAnimationSpeed: public ImpEngineModule::Component
{
public:
    PlayerAnimationSpeed();
    ~PlayerAnimationSpeed();

    Component* Clone(Component* clone) const override;

public:
    float _walk        = 1.f;
    float _walkCorrect = 1.f;

    float _idle              = 1.f;
    float _kneedlingPointing = 1.f;
    float _magicHeal         = 1.f;
    float _magicAreaAttack   = 1.f;
    float _falling           = 1.f;
    float _jumpingUp         = 1.f;
    float _landing           = 1.f;
    float _falingIdle        = 1.f;


    REFLECTION_FRIEND(PlayerAnimationSpeed);
};

} // namespace ImpClient