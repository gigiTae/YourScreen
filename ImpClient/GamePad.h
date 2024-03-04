#pragma once


namespace ImpClient
{

/// <summary>
/// �����е� �ĺ����� ������Ʈ
/// </summary>
class GamePad: public ImpEngineModule::Component
{
public:
    GamePad();
    ~GamePad();

    Component* Clone(Component* clone) const override;

    // �����е� �ĺ��� ��ȯS
    unsigned int GetPadID() const
    {
        return _padID;
    }

private:
    unsigned int _padID;

    REFLECTION_FRIEND(GamePad);
};

} // namespace ImpClient