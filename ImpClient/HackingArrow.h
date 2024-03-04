#pragma once


namespace ImpClient
{
/// <summary>
/// ��ŷ ����Ʈ�� ����Ű�� ȭ��ǥ
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

    // HacknigArrow�� ��ģ��
    void Steal(ImpEngineModule::Entity* stealPlayer);

private:
    void CheckReturnPrevPlayer();

private:
    ImpEngineModule::Entity* _prevPlayer; // ���� ����

    ImpEngineModule::Entity* _target;
    ImpEngineModule::Transform* _transform;

    float _offset;

    std::string _targetName;

    // ���÷��� ����
    REFLECTION_FRIEND(HackingArrow);
};
} // namespace ImpClient
