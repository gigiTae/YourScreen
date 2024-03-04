#pragma once

namespace ImpToolModule
{
class ToolCamera;
class EntityViewer;

/// <summary>
///  ����� Ŭ����
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
    // â�Ƿ� ���� ���� 5�� �帳�ϴ�~ // 2024.02.22 ���� 3�� 26��
    void SetGizumode();

    void ShowUI();


private:
    ToolCamera* _toolCamera;
    EntityViewer* _entityViewer;
    ImpEngineModule::InputManager* _input;

    IMGUIZMO_NAMESPACE::OPERATION _operation;
};

} // namespace ImpToolModule