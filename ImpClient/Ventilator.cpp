#include "ImpClientPCH.h"
#include "Ventilator.h"

// 2024-02-20 ���ֿ�
#include "PlayerState.h"

ImpClient::Ventilator::Ventilator()
    : _windForce(100.f)
    , _windTurbineRotateSpeed(0.f)
    , _onWind(false)
{
}

ImpClient::Ventilator::~Ventilator()
{
}


ImpEngineModule::Component* ImpClient::Ventilator::Clone(
    Component* clone /*= nullptr*/) const
{

    Ventilator* component = static_cast<Ventilator*>(clone);

    if (component == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        component = new Ventilator(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *component = *this;
    }

    return component;
}

void ImpClient::Ventilator::OnTriggerStay(
    const ImpEngineModule::Physics::Collision& collision)
{

    ImpEngineModule::Collider* otherColider = collision.GetOtherCollider(
        GetEntity());

    if (otherColider->GetTag() != Physics::Tag::Player) return;
    _onWind = true;

    auto rigid = otherColider->GetComponent<ImpEngineModule::Rigidbody>();

    rigid->AddVelocity(
        ImpMath::Vector3(0.f, _windForce * collision.dt * _forceRatio, 0.f));

    // �ٶ� ���� ���
    auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

    size_t audioIndex = audioClip->GetAudioIndex(L"SE_UpDraft");
    if (!SoundManager::GetInstance()->isPlaying(audioIndex))
    {
        audioClip->PlaySound(audioIndex, true, audioIndex);
    }

    // ĳ���� �ִϸ��̼� �۵� (2024-02-20 ���ֿ�)
    auto* playerState = otherColider->GetComponent<PlayerState>();

    if (playerState->GetActionState() != PlayerState::ActionState::Sturn)
        playerState->SetActionState(PlayerState::ActionState::Flying);
}


void ImpClient::Ventilator::OnTriggerExit(
    const ImpEngineModule::Physics::Collision& collision)
{

    // ĳ���� �ִϸ��̼� �۵� (2024-02-20 ���ֿ�)
    ImpEngineModule::Collider* otherColider = collision.GetOtherCollider(
        GetEntity());

    if (otherColider->GetTag() != Physics::Tag::Player) return;
    _onWind = false;


    // �ٶ� ���� �ߴ�
    auto audioClip = GetWorld()->GetEntity("AudioManager")->GetComponent<AudioClip>();

    size_t audioIndex = audioClip->GetAudioIndex(L"SE_UpDraft");
    SoundManager::GetInstance()->StopChannel(audioIndex);


    auto* playerState = otherColider->GetComponent<PlayerState>();

    if (playerState->GetActionState() != PlayerState::ActionState::Sturn)
        playerState->SetActionState(PlayerState::ActionState::Falling);
}

void ImpClient::Ventilator::Update(float dt)
{
    constexpr float maxRotateSpeed = 50.f;

    if (_onWind)
    {
        _windTurbineRotateSpeed += dt * 50.f;

        _windTurbineRotateSpeed = std::clamp(
            _windTurbineRotateSpeed, 0.f, maxRotateSpeed);

        GetComponent<ImpEngineModule::Transform>()->AddYAxisRotation(
            dt * _windTurbineRotateSpeed);
    }
    else
    {
        _windTurbineRotateSpeed -= dt * 50.f;

        _windTurbineRotateSpeed = std::clamp(
            _windTurbineRotateSpeed, 0.f, maxRotateSpeed);

        if (_windTurbineRotateSpeed == 0.f)
        {
            return;
        }

        GetComponent<ImpEngineModule::Transform>()->AddYAxisRotation(
            dt * _windTurbineRotateSpeed);
    }

    _forceRatio = _windTurbineRotateSpeed / maxRotateSpeed;
}
