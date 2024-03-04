#pragma once

namespace ImpToolModule
{
class ImpEditor;

/// <summary>
/// Tool 전용 카메라
/// </summary>
class ToolCamera
{
public:
    ToolCamera();
    ~ToolCamera();

    // 초기화
    void Initialize(ImpEngineModule::EngineModule* engineModule,
        ImpEditor* editor,
        ImpEngineModule::InputManager* input);
    // 카메라 이동 처리
    void Update();
    // 종료
    void Finalize();

    // 타겟에서 거리만큼 떨어진 거리로 이동한다(회전 X)
    void LookAt(ImpMath::Vector3 target, float distance);

    // 툴카메라를 사용하는지 확인
    bool OnCamera() const;


    ImpEngineModule::Entity* GetEntity() const
    {
        return _ent.get();
    }

    bool IsActive() const
    {
        return _isActive;
    }

    void SetActive(bool isActive)
    {
        _isActive = isActive;
    }

    DirectX::XMFLOAT4X4 GetViewTM()const;
    DirectX::XMFLOAT4X4 GetProjTM()const;


private:
    void UpdateMatrix();

private:
    bool _isActive;

    std::unique_ptr<ImpEngineModule::Entity> _ent;
    ImpEngineModule::EngineModule* _engineModule;
    ImpEngineModule::InputManager* _inputManager;
    ImpEditor* _editor;

    DirectX::XMFLOAT4X4 _viewTM;
    DirectX::XMFLOAT4X4 _projTM;

    const float cameraToTargetSecond = 0.25f;

    float _elapsedTime;
    Vector3 _targetPosition;
    float _cameraSpeed;
    bool _isArrived;
};


} // namespace ImpToolModule