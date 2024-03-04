#include "EngineModulePCH.h"
#include "Light.h"

ImpEngineModule::Light::Light()
    : _type(LightType::Point)
    , _intensity(1.f)
    , _isBindGraphics(false)
    , _color{ 1.f, 1.f, 1.f, 1.f }
    , _range(1.f)
    , _spotRadius(1.f)
{
}

ImpEngineModule::Light::~Light()
{
}

ImpEngineModule::Component* ImpEngineModule::Light::Clone(
    Component* clone /*= nullptr*/) const
{
    Light* light = static_cast<Light*>(clone);

    if (light == nullptr)
    {
        light = new Light(*this);
    }
    else
    {
        *light = *this;
    }

    light->_isBindGraphics = false;

    return light;
}

void ImpEngineModule::Light::Update(float dt)
{
}
