#pragma once
namespace ImpClient
{
class FadeIn: public ImpEngineModule::Component
{
public:
    FadeIn();
    ~FadeIn();

    virtual void Update(float dt) override;

    void Start() override;

    ImpEngineModule::Component* Clone(Component* clone) const override;

    void StartFadeIn();
    void StartFadeOut();

private:
    ImpEngineModule::UI* _ui;

    bool _isStart;
    bool _isFadeIn;
    float _alpha;
    float _speed;

    REFLECTION_FRIEND(FadeIn);
};
}
