#pragma once

namespace ImpClient
{
/// <summary>
/// 플레이어 카메라
/// </summary>
class PlayerCamera: public ImpEngineModule::Component
{
public:
    PlayerCamera();
    ~PlayerCamera();

    Component* Clone(Component* clone) const override;

    void Start() override;
    void Update(float dt);

private:
    void RoatateCamera();

private:
    std::string _playerName;

    float _playerToCameraDistance;
    unsigned int _padID;

    ImpMath::Vector3 _offset;
    ImpEngineModule::Entity* _player;

    REFLECTION_FRIEND(PlayerCamera);
};                 
} // namespace ImpClient