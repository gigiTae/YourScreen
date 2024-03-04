#pragma once


namespace ImpClient
{
class QTE;
class GameManager;

/// <summary>
///  해킹가능한 오브젝트들에게는 이 컴포넌트를 부착한다.
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

    // 해킹시 얻는 능력 
    enum class SkillAbility
    {
      Qucik, // EMP쿨감소
      Scale, // EMP크기 증가
      Victory, // 승리
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
    // 상호작용 가능한지
    bool IsInteractable(ImpEngineModule::Entity* ent) const;
    
    // 해킹 가능한지
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