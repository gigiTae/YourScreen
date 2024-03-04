#pragma once

namespace ImpToolModule
{
class ToolCamera;
class EntityViewer;

/// <summary>
///  기즈모 클래스
/// </summary>
class ImpGizumo
{
public:
    ImpGizumo();
    ~ImpGizumo();

    void Initialize(ToolCamera* camera,
        EntityViewer* entViewer,
        ImpEngineModule::InputManager* input);

    void Update();

    

    IMGUIZMO_NAMESPACE::OPERATION GetMode() const
    {
        return _operation;
    }

private:
    // 창의력 점수 별점 5개 드립니다~ // 2024.02.22 오전 3시 26분
    void SetGizumode();

    void ShowUI();


private:
    ToolCamera* _toolCamera;
    EntityViewer* _entityViewer;
    ImpEngineModule::InputManager* _input;

    IMGUIZMO_NAMESPACE::OPERATION _operation;
};

} // namespace ImpToolModule