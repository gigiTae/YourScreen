#pragma once

namespace ImpToolModule
{
class ImpEditor;

/// <summary>
/// Tool ���� ī�޶�
/// </summary>
class ToolCamera
{
public:
    ToolCamera();
    ~ToolCamera();

    // �ʱ�ȭ
    void Initialize(ImpEngineModule::EngineModule* engineModule,
        ImpEditor* editor,
        ImpEngineModule::InputManager* input);
    // ī�޶� �̵� ó��
    void Update();
    // ����
    void Finalize();

    // Ÿ�ٿ��� �Ÿ���ŭ ������ �Ÿ��� �̵��Ѵ�(ȸ�� X)
    void LookAt(ImpMath::Vector3 target, float distance);

    // ��ī�޶� ����ϴ��� Ȯ��
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