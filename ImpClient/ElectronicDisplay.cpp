#include "ImpClientPCH.h"
#include "ElectronicDisplay.h"

ImpClient::ElectronicDisplay::ElectronicDisplay()
    : _isIncrease(true)
    , _intensity(0.f)
    , _maxIntensity(1.f)
    , _minmumIntensity(0.f)
    , _changeSpeed(0.f)
{
}

ImpClient::ElectronicDisplay::~ElectronicDisplay()
{
}

void ImpClient::ElectronicDisplay::Update(float dt)
{

    if (_isIncrease)
    {
        _intensity += dt * _changeSpeed;

        if (_maxIntensity <= _intensity)
        {
            _intensity  = _maxIntensity;
            _isIncrease = false;
        }
    }
    else
    {
        _intensity -= dt * _changeSpeed;

        if (_minmumIntensity >= _intensity)
        {
            _intensity  = _minmumIntensity;
            _isIncrease = true;
        }
    }

    auto mesh = GetComponent<Mesh>();

    if (mesh != nullptr)
    {
        mesh->SetEmissionIntensity(_intensity);
    }
}

ImpEngineModule::Component* ImpClient::ElectronicDisplay::Clone(
    Component* clone /*= nullptr*/) const
{
    ElectronicDisplay* component = static_cast<ElectronicDisplay*>(clone);

    if (component == nullptr) // 새로 생성해서 복사본을 준다
    {
        component = new ElectronicDisplay(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *component = *this;
    }

    return component;
}
