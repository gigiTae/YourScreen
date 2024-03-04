#pragma once

namespace ImpClient
{
class PlayerAttack;

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
    std::wstring _ringPrefab;
    ImpEngineModule::Entity* _ring;

    ImpEngineModule::Transform* _transform;
    ImpEngineModule::Mesh* _mesh;
    PlayerAttack* _playerAttack;

    ImpMath::Vector3 _startPosition;

    float _elapsedTime;
    float _attackRange;
    float _currentScale;

    REFLECTION_FRIEND(EMP);
};


} // namespace ImpClient