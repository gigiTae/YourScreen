#include "pch.h"
#include "DirectionalLight.h"

#include "IImpGraphicsEngine.h"
#include "../ImpStructure/ImpMath.h"

using namespace ImpGraphics;

DirectionalLight::DirectionalLight()
	:_lightData(nullptr)
{
	_lightData = new DirectionalLightData;
}


DirectionalLight::~DirectionalLight()
{
	delete _lightData;
}

void DirectionalLight::SetLight(const LightInfo& lightInfo)
{
	_lightID = lightInfo._lightID;
	_lightType = lightInfo._lightType;

	_lightData->_color.x = lightInfo._color.x;
	_lightData->_color.y = lightInfo._color.y;
	_lightData->_color.z = lightInfo._color.z;
	_lightData->_color.w = lightInfo._color.w;

	_lightData->_intensity = lightInfo._intensity;
}

void DirectionalLight::Update(ImpMath::Matrix transformMatrix)
{
	_lightData->_direction.x = transformMatrix._31;
	_lightData->_direction.y = transformMatrix._32;
	_lightData->_direction.z = transformMatrix._33;
}