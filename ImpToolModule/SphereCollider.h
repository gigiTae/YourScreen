#pragma once
#include "DebugObject.h"


namespace ImpToolModule::Debug
{
	/// <summary>
	/// 디버그용도 구
	/// </summary>
	class SphereCollider :
		public DebugObject
	{
	public:
		SphereCollider(ImpGraphics::IImpGraphicsEngine* graphics, ImpEngineModule::Entity* ent);
		~SphereCollider();

		void Update() override;

	private:
		float _radius;

        const size_t sphereID1 = 800000000000000000;		
		ImpEngineModule::Entity* _ent;	
		ImpEngineModule::Transform* _transform;
		ImpEngineModule::SphereCollider* _sphere;
	};


}