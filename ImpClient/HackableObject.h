#pragma once


namespace ImpClient
{
class QTE;
class GameManager;

/// <summary>
///  ��ŷ������ ������Ʈ�鿡�Դ� �� ������Ʈ�� �����Ѵ�.
/// </summary>
class HackableObject: public ImpEngineModule::Component
{
public:
    enum class State
    {
        None,
        LeftPlayerHacked,
        RightPlayerHacked,
        AllPlayerHacked
    };

    // ��ŷ�� ��� �ɷ� 
    enum class SkillAbility
    {
      Qucik, // EMP�𰨼�
      Scale, // EMPũ�� ����
      Victory, // �¸�
    };

    HackableObject();
    ~HackableObject();

    Component* Clone(Component* clone = nullptr) const override;

    bool IsHackingPoint() const
    {
        return _isHackingPoint;
    }
    void SetHackingPoint(bool isHackingPoint)
    {
        _isHackingPoint = isHackingPoint;
    }

    ImpClient::HackableObject::State GetState() const
    {
        return _state;
    }

    void HackObject(ImpEngineModule::Entity* player);

    void OnTriggerStay(const ImpEngineModule::Physics::Collision& collision) override;
    void OnTriggerExit(const ImpEngineModule::Physics::Collision& collision) override;

    void SetGameManager(ImpClient::GameManager* gameMgr)
    {
        _gameManager = gameMgr;
    }

	bool IsLastPoint() const
    {
        return _isLastPoint;
    }
    void SetLastPoint(bool isLast)
    {
        _isLastPoint = isLast;
    }

	ImpClient::HackableObject::SkillAbility GetAbility() const
    {
        return _ability;
    }
    void SetAbility(ImpClient::HackableObject::SkillAbility val)
    {
        _ability = val;
    }

private:
    // ��ȣ�ۿ� ��������
    bool IsInteractable(ImpEngineModule::Entity* ent) const;
    
    // ��ŷ ��������
    bool IsHacakable(ImpEngineModule::Entity* ent) const;
    void CreateQTE(ImpEngineModule::Entity* player);
    void IncreaseSkillAbility(ImpEngineModule::Entity* player);

private:
    bool _isHackingPoint;
    bool _isLastPoint;
    float _hackableRadius;
    State _state;
    SkillAbility _ability;

    QTE* _leftQTE;
    QTE* _rightQTE;

    GameManager* _gameManager;

    REFLECTION_FRIEND(HackableObject);
};

} // namespace ImpClient