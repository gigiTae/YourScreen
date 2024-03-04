#pragma once
#include "DebugObject.h"

namespace ImpToolModule::Debug
{
	/// <summary>
	/// Camera Gizumo
	/// </summary>
	class Camera :
		public DebugObject
	{
	public:
		Camera(ImpGraphics::IImpGraphicsEngine* graphics, ImpEngineModule::Entity* ent);
		~Camera();

		void Update() override;

	private:
		ImpEngineModule::Entity* _ent;
		ImpEngineModule::Camera* _camera;
		ImpEngineModule::Transform* _transform;

		const size_t cameraID1 = 300000000000000000;
		const size_t cameraID2 = 400000000000000000;

	};


}