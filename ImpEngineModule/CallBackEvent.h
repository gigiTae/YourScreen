#pragma once


namespace ImpEngineModule
{
	class Entity;
}

/// <summary>
/// Call Back �̺�Ʈ
/// </summary>
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

	// Entity ���
	struct OnEntityRegister
	{
		std::shared_ptr<Entity> entity;
	};

	// CubeMapLoad
	struct LoadCubeMap
	{
		std::wstring path;
	};
	
	// World �ε� �̺�Ʈ
	struct LoadedWorld
	{
		std::wstring worldName;
	};

	// �ι�° ���� �̺�Ʈ
	struct SetSecondCamera
	{
        bool onSecondCamera;
	};

}
