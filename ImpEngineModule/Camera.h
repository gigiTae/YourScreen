#pragma once
#include "Component.h"

namespace ImpEngineModule
{

/// <summary>
/// Camera ������Ʈ
///
/// </summary>successed
class Camera: public Component
{
public:
    Camera();
    ~Camera();

    Component* Clone(Component* clone = nullptr) const override;

    /// ���� ī�޶�� �����Ѵ�.
    void OnMainCamera();

    /// ���� ī�޶� ����
    void OffMainCamera()
    {
        _isMain = false;
    }

    /// ���� ī�޶�����
    bool IsMain() const
    {
        return _isMain;
    }

    // 2��° ī�޶�����
    bool IsSecond() const
    {
        return _isSecond;
    }

    /// 2��° ī�޶� ����
    void OffSecondCamera()
    {
        _isSecond = false;
    }

    // 2��° ī�޶� ����
    void OnSecondCameara();

    void Strafe(float distance);
    void Walk(float distance);
    void WorldUpDown(float distance);

    void Yaw(float degree);
    void Pitch(float degree);
    void Roll(float degree);

    float GetFieldOfView() const
    {
        return _fieldOfView;
    }
    void SetFieldOfView(float val)
    {
        _fieldOfView = val;
    }
    float GetNearPlain() const
    {
        return _nearPlain;
    }
    void SetNearPlain(float val)
    {
        _nearPlain = val;
    }
    float GerFarPlain() const
    {
        return _farPlain;
    }
    void SetFarPlain(float val)
    {
        _farPlain = val;
    }
    bool IsPerspective() const
    {
        return _isPerspective;
    }

private:
    bool _isMain        = false;
    bool _isSecond      = false;
    float _fieldOfView  = 0.25 * 3.14f;
    float _nearPlain    = 0.3f;
    float _farPlain     = 1000.f;
    bool _isPerspective = true;

    REFLECTION_FRIEND(ImpEngineModule::Camera)
};


} // namespace ImpEngineModule