#pragma once

namespace ImpClient
{

/// <summary>
/// �ÿ��� ����� Ű
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