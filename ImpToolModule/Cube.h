#pragma once
#include "DebugObject.h"


namespace  ImpToolModule::Debug
{
	class Cube :
		public DebugObject
	{
	public:
		Cube(ImpGraphics::IImpGraphicsEngine* graphics, ImpEngineModule::Entity* ent);
		~Cube();

		void Update() override;

	private:
		ImpEngineModule::Entity* _ent;
		ImpEngineModule::Transform* _transform;
		ImpEngineModule::BoxCollider* _boxCollider;
	};

}
