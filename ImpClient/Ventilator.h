#pragma once

namespace ImpClient
{

/// <summary>
///  È¯Ç³±â
/// </summary>
class Ventilator: public ImpEngineModule::Component
{
public:
    Ventilator();
    ~Ventilator();

    Component* Clone(Component* clone = nullptr) const override;

    void OnTriggerStay(const ImpEngineModule::Physics::Collision& collision) override;
    void OnTriggerExit(const ImpEngineModule::Physics::Collision& collision) override;

    void Update(float dt) override;

private:
    float _windForce;

    float _windTurbineRotateSpeed;
    float _forceRatio = 0.f;

    bool _onWind;

    REFLECTION_FRIEND(Ventilator);
};
} // namespace ImpClient
