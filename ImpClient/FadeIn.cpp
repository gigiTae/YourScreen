#include "ImpClientPCH.h"
#include "FadeIn.h"

ImpClient::FadeIn::FadeIn()
    : _isStart(false)
    , _speed(1.f)
    , _isFadeIn(true)
{
}

ImpClient::FadeIn::~FadeIn()
{
}

void ImpClient::FadeIn::Update(float dt)
{
    if (_isFadeIn && _isStart)
    {
        _alpha = std::clamp(_alpha - dt * _speed, 0.f, 1.f);
        _ui->SetAlpha(_alpha);
    }
    else if (!_isFadeIn && _isStart)
    {
        _alpha = std::clamp(_alpha + dt * _speed, 0.f, 1.f);
        _ui->SetAlpha(_alpha);
    }
}


void ImpClient::FadeIn::Start()
{
    _ui = GetComponent<UI>();

    auto input = GetWorld()->GetInputManager();
    RECT rect  = input->GetScreenRect();

    long width  = rect.right - rect.left;
    long height = rect.bottom - rect.top;

    _ui->SetUISize(width, height);
    _ui->SetUIPostition(0.f, 0.f);
    _ui->SetAlpha(0.0f);
}


ImpEngineModule::Component* ImpClient::FadeIn::Clone(Component* clone) const
{
    FadeIn* emp = static_cast<FadeIn*>(clone);

    if (emp == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        emp = new FadeIn(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *emp = *this;
    }

    return emp;
}

void ImpClient::FadeIn::StartFadeIn()
{
    _isStart  = true;
    _isFadeIn = true;
}

void ImpClient::FadeIn::StartFadeOut()
{
    _isStart  = true;
    _isFadeIn = false;
}
