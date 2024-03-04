#pragma once
#include "DebugObject.h"

namespace ImpToolModule::Debug
{
	/// <summary>
	/// Light 기즈모 
	/// </summary>
	class Light : public DebugObject
	{
	public:
		Light(ImpGraphics::IImpGraphicsEngine* graphics, ImpEngineModule::Entity* ent);
		~Light();

		void Update() override;

	private:
		ImpEngineModule::Entity* _ent;
		ImpEngineModule::Transform* _transform;
		ImpEngineModule::Light* _light;

		ImpEngineModule::Light::LightType _currentLightType;


		/// 기즈모를 그리기위한 light 식별자용 id
		const size_t lightID1 = 100000000000000000;
		const size_t lightID2 = 200000000000000000;
		const size_t lightID3 = 300000000000000000;

	};


}
