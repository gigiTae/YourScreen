#pragma once


namespace ImpToolModule::Debug
{
	// �����뵵�� ������Ʈ
	// ex) �ٿ�� ����, ����Ʈ ����� ���
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