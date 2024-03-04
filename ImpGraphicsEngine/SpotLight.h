#pragma once
#include "LightBase.h"

namespace ImpGraphics
{
	class SpotLight : public LightBase
	{
	public:
		SpotLight();
		virtual ~SpotLight();

		virtual void SetLight(const LightInfo& lightInfo) override;
		virtual void Update(ImpMath::Matrix transformMatrix) override;

		SpotLightData* GetLightData() {return _lightData;}

	private:
        SpotLightData* _lightData;
	};
}