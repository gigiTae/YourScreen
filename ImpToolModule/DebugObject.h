#pragma once


namespace ImpToolModule::Debug
{
	// 디버깅용도의 오브젝트
	// ex) 바운딩 볼륨, 라이트 기즈모 등등
	class DebugObject
	{
	public: 
		DebugObject(ImpGraphics::IImpGraphicsEngine* graphics, size_t id);
	
		virtual void Update() abstract;

	protected:
		ImpGraphics::IImpGraphicsEngine* _graphicsEngine;
		size_t _id;
	};


}