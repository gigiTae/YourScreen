#pragma once

namespace ImpClient
{

/// <summary>
/// Culling�� �ʿ��� �� ������Ʈ
/// ī�޶�� �浹�ϴ� ��� Mesh�� �������� �����Ѵ�
/// </summary>
class CullingWall: public ImpEngineModule::Component
{
public:
    CullingWall();
    ~CullingWall();

    ImpEngineModule::Component* Clone(
        ImpEngineModule::Component* clone) const override;

    void OnTriggerEnter(const ImpEngineModule::Physics::Collision& collision) override;

    void OnTriggerExit(const ImpEngineModule::Physics::Collision& collision) override;

    void Start() override;

private:
    ImpEngineModule::Mesh* _mesh;

    REFLECTION_FRIEND(CullingWall)
};

} // namespace ImpClient