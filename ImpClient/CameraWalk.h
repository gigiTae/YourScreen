#pragma once
/// <summary>
/// ī�޶� ������ ���� CameraWalk Component 
/// 2024.02.21 ���� 12�� 11�� ������
/// </summary>
namespace ImpClient
{
class CameraWalk: public ImpEngineModule::Component
{
public:
    CameraWalk();
    ~CameraWalk();

    virtual void Start() override;
    virtual void Update(float dt) override;

    ImpEngineModule::Component* Clone(Component* clone) const override;

private:
    // ���÷��� �� ����
    std::vector<std::wstring> _points; // position, rotation�� /�� ���� �� 7��
    std::vector<std::wstring> _pointsPlayTime; // �� point ��� �ð�(�������ϱ� �������� �Է�����)
    float _cameraWalkStartTime;
    float _playerAnimationStartTime;
    float _gameStartTime;

    // ���� ����
    float _playTime;
    size_t _curPlayPointIndex;

    float _fadeStartTime;
    bool _isFade;

    bool _isPlay;

    REFLECTION_FRIEND(CameraWalk);
};
}

/*
/// <summary>
/// EMP ������Ʈ
///
///  scale 1�϶� ������� radius 3�̴�
/// </summary>
class EMP: public ImpEngineModule::Component
{
public:
    EMP();
    ~EMP();

    ImpEngineModule::Component* Clone(Component* clone) const override;

    void Update(float dt) override;

    void Set(PlayerAttack* playerAttack);

private:
    ImpEngineModule::Transform* _transform;
    ImpEngineModule::Mesh* _mesh;
    PlayerAttack* _playerAttack;

    ImpMath::Vector3 _startPosition;

    float _elapsedTime;
    float _attackRange;
    float _currentScale;
};


} // namespace ImpClient*/