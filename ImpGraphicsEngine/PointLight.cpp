#include "pch.h"
#include "PointLight.h"

#include "IImpGraphicsEngine.h"
#include "../ImpStructure/ImpMath.h"

ImpGraphics::PointLight::PointLight()
	:_lightData(nullptr)
{
	_lightData = new PointLightData;
}

ImpGraphics::PointLight::~PointLight()
{
	delete _lightData;
}

void ImpGraphics::PointLight::SetLight(const LightInfo& lightInfo)
{
	_lightID = lightInfo._lightID;
	_lightType = lightInfo._lightType;

	_lightData->_color.x = lightInfo._color.x;
	_lightData->_color.y = lightInfo._color.y;
	_lightData->_color.z = lightInfo._color.z;
	_lightData->_color.w = lightInfo._color.w;

	_lightData->_intensity = lightInfo._intensity;
	_lightData->_range = lightInfo._range;
	_lightData->_attenuation = DirectX::XMFLOAT3{0, 0, 1};
}

void ImpGraphics::PointLight::Update(ImpMath::Matrix transformMatrix)
{
	_lightData->_position.x = transformMatrix._41;
	_lightData->_position.y = transformMatrix._42;
	_lightData->_position.z = transformMatrix._43;
}