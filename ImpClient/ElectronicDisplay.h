#pragma once


namespace ImpClient
{

/// <summary>
/// Loby¿ëµµ
/// </summary>
class ElectronicDisplay: public ImpEngineModule::Component
{
public:
    ElectronicDisplay();
    ~ElectronicDisplay();

    void Update(float dt) override;

    Component* Clone(Component* clone = nullptr) const override;


private:
    bool _isIncrease;
    float _changeSpeed;

    float _intensity;
    float _maxIntensity;
    float _minmumIntensity;

    REFLECTION_FRIEND(ElectronicDisplay);
}; // namespace ImpClient


} // namespace ImpClient