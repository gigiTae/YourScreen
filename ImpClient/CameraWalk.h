#pragma once
/// <summary>
/// 카메라 연출을 위한 CameraWalk Component 
/// 2024.02.21 오전 12시 11분 김유진
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
    // 리플렉션 용 변수
    std::vector<std::wstring> _points; // position, rotation을 /로 구분 총 7개
    std::vector<std::wstring> _pointsPlayTime; // 각 point 재생 시간(귀찮으니까 누적으로 입력하자)
    float _cameraWalkStartTime;
    float _playerAnimationStartTime;
    float _gameStartTime;

    // 내부 변수
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
/// EMP 컴포넌트
///
///  scale 1일때 사이즈는 radius 3이다
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