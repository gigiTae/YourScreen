#include "ImpClientPCH.h"
#include "QTEButtonEffect.h"

ImpClient::QTEButtonEffect::QTEButtonEffect()
    : _elapsedTime(0.f)
    , _speed(1.f)
    , _isCallDestory(false)
    , _destoryTime(0.f)
{
}

ImpClient::QTEButtonEffect::~QTEButtonEffect()
{
}

ImpEngineModule::Component* ImpClient::QTEButtonEffect::Clone(
    Component* clone /*= nullptr*/) const
{
    QTEButtonEffect* qteBtn = static_cast<QTEButtonEffect*>(clone);

    if (qteBtn == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        qteBtn = new QTEButtonEffect(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *qteBtn = *this;
    }

    return qteBtn;
}

void ImpClient::QTEButtonEffect::Update(float dt)
{
    if (!_isCallDestory)
    {
        return;
    }

    _elapsedTime += dt;

    auto ui  = GetComponent<ImpEngineModule::UI>();
    auto pos = ui->GetUIPosition();

    pos.y += _speed * dt;
    ui->SetUIPostition(pos.x, pos.y);
    ui->SetAlpha(1.f - _elapsedTime / _destoryTime);

    if (_destoryTime <= _elapsedTime)
    {
        auto world = GetWorld();

        world->GetEventManager()->PushPostEvent(
            [world, this]() { world->DestroyEntity(this->GetEntity()); });
    }
}

void ImpClient::QTEButtonEffect::Destory(float destroyTime, float speed)
{
    _destoryTime   = destroyTime;
    _elapsedTime   = 0.f;
    _speed         = speed;
    _isCallDestory = true;
}
