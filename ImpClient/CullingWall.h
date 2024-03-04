#pragma once

namespace ImpClient
{

/// <summary>
/// Culling이 필요한 벽 컴포넌트
/// 카메라와 충돌하는 경우 Mesh의 랜더링을 제어한다
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