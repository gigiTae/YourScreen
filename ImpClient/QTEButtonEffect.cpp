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

    if (qteBtn == nullptr) // 새로 생성해서 복사본을 준다
    {
        qteBtn = new QTEButtonEffect(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
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
