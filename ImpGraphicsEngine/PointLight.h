#pragma once
#include "LightBase.h"

namespace ImpGraphics
{
	class PointLight : public LightBase
	{
	public:
		PointLight();
		virtual ~PointLight();

		virtual void SetLight(const LightInfo& lightInfo) override;
		virtual void Update(ImpMath::Matrix transformMatrix) override;

		PointLightData* GetLightData() { return _lightData; }

	private:
		PointLightData* _lightData;
	};
}