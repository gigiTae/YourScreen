#pragma once


namespace ImpClient
{
class PlayerState;
class QTE;

/// <summary>
/// 플레이어 움직임을 담당하는 컴포넌트
///
///
/// </summary>
class PlayerMovement: public ImpEngineModule::Component
{
public:
    PlayerMovement();
    ~PlayerMovement();

    Component* Clone(Component* clone = nullptr) const override;

    void Start() override;

    void Update(float dt) override;

    // 플레이어가 바라보는 방향
    ImpMath::Vector3 GetPlayerDirection() const
    {
        return _playerDirection;
    }

    // 공격당할 때 호출
    void Attacked();

    ImpClient::QTE* GetQte() const
    {
        return _qte;
    }
    void SetQTE(ImpClient::QTE* qte);

    float GetMaxSpeed() const
    {
        return _maxMoveSpeed;
    }

	float GetStopInputTime() const
    {
        return _stopInputTime;
    }
    void SetStopInputTime(float val)
    {
        _stopInputTime = val;
    }

private:
    void MovePlayer(float dt, ImpEngineModule::InputManager* input);
    void JumpPlayer(float dt, ImpEngineModule::InputManager* input);

private:
    ImpEngineModule::Transform* _transform;
    ImpEngineModule::Rigidbody* _rigidBody;

    float _stopInputTime;
    unsigned int _padID;
    float _maxMoveSpeed;
    float _moveSpeed;
    float _jumpPower;
    ImpMath::Vector3 _playerDirection;

    float _focusTime;

    PlayerState* _state;
    std::string _cameraName;
    ImpEngineModule::Entity* _camera;

    QTE* _qte;

    // 리플렉션 선언
    REFLECTION_FRIEND(PlayerMovement);
};

} // namespace ImpClient