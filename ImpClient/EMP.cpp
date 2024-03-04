#include "ImpClientPCH.h"
#include "EMP.h"
#include "PlayerAttack.h"

ImpClient::EMP::EMP()
    : _transform(nullptr)
    , _mesh(nullptr)
    , _currentScale(0.f)
    , _elapsedTime(0.f)
    , _startPosition{}
    , _playerAttack(nullptr)
    , _attackRange(0.f)
    , _ring(nullptr)
    , _ringPrefab{}
{
}

ImpClient::EMP::~EMP()
{
}

ImpEngineModule::Component* ImpClient::EMP::Clone(Component* clone) const
{
    EMP* emp = static_cast<EMP*>(clone);

    if (emp == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        emp = new EMP(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *emp = *this;
    }

    return emp;
}

void ImpClient::EMP::Update(float dt)
{
    constexpr float maxScaleTime     = 0.25f;
    constexpr float minmizeTime      = 0.3f;
    constexpr float eraseTime        = 1.f;
    static constexpr float sizeRatio = 1.f / 3.f;

    _elapsedTime = std::clamp(_elapsedTime + dt, 0.f, eraseTime);

    _transform->SetPosition(_startPosition);

    // 1. EMP�� ���ݹ������� ����
    ///  scale 1�϶� ������� radius 3�̴�

    if (_elapsedTime <= maxScaleTime)
    {
        float ratio   = std::min(_elapsedTime, maxScaleTime) / maxScaleTime;
        _currentScale = _attackRange * ratio * sizeRatio;
        _transform->SetScale(Vector3(_currentScale, _currentScale, _currentScale));
    }

    // 2. ȸ��
    constexpr float rotateSpeed = 1.f;
    _transform->AddYAxisRotation(dt * rotateSpeed);

    // ring ȸ��
    if (_ring)
    {
         _ring->GetComponent<Transform>()->AddYAxisRotation(-dt * rotateSpeed * 2.f);
    }

    // 3. �ð��� ���������� �����ϰ�
    if (_elapsedTime >= 0.75F)
    {
        constexpr float remainTime = eraseTime - 0.75f;

        float alpha = 1.f - (_elapsedTime - 0.75f) / remainTime;

        _mesh->SetEmissionIntensity(alpha);
        _ring->GetComponent<Mesh>()->SetEmissionIntensity(alpha);
    }

    // 4. �����ð� ������ �Ҹ�
    if (_elapsedTime == eraseTime)
    {
        GetWorld()->DestroyEntity(GetEntity());
    }
}

void ImpClient::EMP::Set(PlayerAttack* playerAttack)
{
    _mesh      = GetComponent<Mesh>();
    _transform = GetComponent<Transform>();
    _transform->SetScale(Vector3(0.f, 0.f, 0.f));
    _startPosition = playerAttack->GetComponent<Transform>()->GetWorldPostiion();

    _playerAttack = playerAttack;
    _attackRange  = _playerAttack->GetAttackRange();
    _elapsedTime  = 0.f;

    _ring = GetWorld()->RegisterEntity(
        GetWorld()->GetResourceManager()->GetEntityResource(_ringPrefab)->GetEntity());

    _ring->GetComponent<Transform>()->SetParent(_transform);
    _ring->GetComponent<Transform>()->SetPosition(Vector3::Zero);
}
