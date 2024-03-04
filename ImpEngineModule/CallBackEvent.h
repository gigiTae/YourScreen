#pragma once


namespace ImpEngineModule
{
	class Entity;
}

/// <summary>
/// Call Back 이벤트
/// </summary>
namespace ImpEngineModule::Event
{
	// Entity 생성 이벤트 
	struct OnEntityCreated
	{
		Entity* entity;
	};
	
	// Entity Start 호출
	struct OnEntityStarted
	{
		Entity* entity;
	};

	// Entity 삭제 이벤트
	struct OnEntityDestroyed
	{
		Entity* entity;
	};

	// Entity 등록
	struct OnEntityRegister
	{
		std::shared_ptr<Entity> entity;
	};

	// CubeMapLoad
	struct LoadCubeMap
	{
		std::wstring path;
	};
	
	// World 로드 이벤트
	struct LoadedWorld
	{
		std::wstring worldName;
	};

	// 두번째 설정 이벤트
	struct SetSecondCamera
	{
        bool onSecondCamera;
	};

}
