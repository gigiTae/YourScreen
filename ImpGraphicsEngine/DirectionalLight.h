#pragma once
#include "LightBase.h"

namespace ImpGraphics
{
	class DirectionalLight : public LightBase
	{
	public:
		DirectionalLight();
		virtual ~DirectionalLight();

		virtual void SetLight(const LightInfo& lightInfo) override;
		virtual void Update(ImpMath::Matrix transformMatrix) override;

		DirectionalLightData* GetLightData() { return _lightData; }

	private:
		DirectionalLightData* _lightData;
	};
}