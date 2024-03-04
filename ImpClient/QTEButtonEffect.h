#pragma once


namespace ImpClient
{

class QTEButtonEffect: public ImpEngineModule::Component
{
public:
    QTEButtonEffect();
    ~QTEButtonEffect();

    Component* Clone(Component* clone = nullptr) const override;

    void Update(float dt) override;

    void Destory(float destroyTime , float speed);

private:
    float _speed;
    float _elapsedTime;
    float _destoryTime;

    bool _isCallDestory;

    REFLECTION_FRIEND(QTEButtonEffect)
};

} // namespace ImpClient