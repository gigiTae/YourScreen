#pragma once


namespace ImpClient
{
class PlayerState;
class QTE;

/// <summary>
/// �÷��̾� �������� ����ϴ� ������Ʈ
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

    // �÷��̾ �ٶ󺸴� ����
    ImpMath::Vector3 GetPlayerDirection() const
    {
        return _playerDirection;
    }

    // ���ݴ��� �� ȣ��
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

    // ���÷��� ����
    REFLECTION_FRIEND(PlayerMovement);
};

} // namespace ImpClient