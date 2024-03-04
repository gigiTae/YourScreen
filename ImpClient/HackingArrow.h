#pragma once


namespace ImpClient
{
/// <summary>
/// 해킹 포인트를 가리키는 화살표
/// </summary>
class HackingArrow: public ImpEngineModule::Component
{
public:
    HackingArrow();
    ~HackingArrow();

    Component* Clone(Component* clone = nullptr) const override;

    void Start() override;

    void Update(float dt) override;

    std::string GetTargetName() const
    {
        return _targetName;
    }

    void SetTargetName(std::string targetName)
    {
        _targetName = targetName;
    }

    // HacknigArrow를 훔친다
    void Steal(ImpEngineModule::Entity* stealPlayer);

private:
    void CheckReturnPrevPlayer();

private:
    ImpEngineModule::Entity* _prevPlayer; // 본래 주인

    ImpEngineModule::Entity* _target;
    ImpEngineModule::Transform* _transform;

    float _offset;

    std::string _targetName;

    // 리플렉션 선언
    REFLECTION_FRIEND(HackingArrow);
};
} // namespace ImpClient
