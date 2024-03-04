#pragma once

namespace ImpClient
{
class QTE;
class LobyManager;

/// <summary>
/// Loby화면 해킹 오브젝트
/// </summary>
class ReadyHacking: public ImpEngineModule::Component
{
public:
    enum class State
    {
        None,
        LeftPlayerHacked,
        RightPlayerHacked,
        AllPlayerHacked
    };

    ReadyHacking();
    ~ReadyHacking();

    ImpEngineModule::Component* Clone(
        Component* clone /* = nullptr */) const override;

    void Update(float dt) override;

    void OnTriggerStay(const ImpEngineModule::Physics::Collision& collision) override;

    bool IsInteractable(ImpEngineModule::Entity* ent) const;
    bool IsHacakable(ImpEngineModule::Entity* ent) const;
    void CreateQTE(ImpEngineModule::Entity* player);

   void FailQTE(unsigned int padID);
    void ClearQTE(unsigned int padID);

    void OnTriggerExit(const ImpEngineModule::Physics::Collision& collision) override;

private:
    std::wstring _qtePrefab;

    State _state;

    QTE* _leftQTE;
    QTE* _rightQTE;

    LobyManager* _lobyManager;

    REFLECTION_FRIEND(ReadyHacking);
};


} // namespace ImpClient