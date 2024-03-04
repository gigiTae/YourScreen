#pragma once

namespace ImpClient
{

/// <summary>
/// 시연용 디버그 키
/// </summary>
class DeubugKey: public ImpEngineModule::Component
{
public:
    DeubugKey();
    ~DeubugKey();


    Component* Clone(Component* clone) const override;

    void Update(float dt) override;
};


} // namespace ImpClient