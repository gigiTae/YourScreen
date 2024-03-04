#pragma once


namespace ImpEngineModule
{
	class Entity;
}

namespace ImpEngineModule::Event
{
	// Entity ���� �̺�Ʈ 
	struct OnEntityCreated
	{
		Entity* entity;
	};
	
	// Entity Start ȣ��
	struct OnEntityStarted
	{
		Entity* entity;
	};

	// Entity ���� �̺�Ʈ
	struct OnEntityDestroyed
	{
		Entity* entity;
	};


}
