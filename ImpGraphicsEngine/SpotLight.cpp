#include "pch.h"
#include "SpotLight.h"

#include "IImpGraphicsEngine.h"
#include "../ImpStructure/ImpMath.h"

ImpGraphics::SpotLight::SpotLight()
    : _lightData(nullptr)
{
    _lightData = new SpotLightData;
}

ImpGraphics::SpotLight::~SpotLight()
{
    delete _lightData;
}

void ImpGraphics::SpotLight::SetLight(const LightInfo& lightInfo)
{
    _lightID   = lightInfo._lightID;
    _lightType = lightInfo._lightType;

    _lightData->_color.x = lightInfo._color.x;
    _lightData->_color.y = lightInfo._color.y;
    _lightData->_color.z = lightInfo._color.z;
    _lightData->_color.w = lightInfo._color.w;

    _lightData->_intensity   = lightInfo._intensity;
    _lightData->_range       = lightInfo._range;
    _lightData->_attenuation = DirectX::XMFLOAT3{ 0, 0, 1 };
    _lightData->_spot        = lightInfo._spot;
}

void ImpGraphics::SpotLight::Update(ImpMath::Matrix transformMatrix)
{
    _lightData->_direction.x = transformMatrix._31;
    _lightData->_direction.y = transformMatrix._32;
    _lightData->_direction.z = transformMatrix._33;

    _lightData->_position.x = transformMatrix._41;
    _lightData->_position.y = transformMatrix._42;
    _lightData->_position.z = transformMatrix._43;
}