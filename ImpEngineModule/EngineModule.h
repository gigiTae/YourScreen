#pragma once

namespace ImpEngineModule
{
	class World;
	class InputManager;
	class TimeManager;
	class EventManager;
	class WorldManager;
	class ResourceManager;
	class CameraManager;
	class CollisionManager;

	/// <summary>
	/// World, 매니저클래스들을 관리한다.
	/// 
	/// </summary>
	class EngineModule
	{
	public:
		EngineModule();
		~EngineModule();

		// 초기화
		void Initialize(HWND hWnd, std::wstring startWorld);

		// 게임루프 시작시 호출
		void Start();
	
		// EngineModule들의 루프
		float Update();
		
		// 이벤트를 처리한다.
		void PostUpdate();

		//종료
		void Finalize();

		World* GetWorld()const { return _world.get(); }
		InputManager* GetInputManager()const { return _inputManager.get(); }
		TimeManager* GetTimeManager()const { return _timeManager.get(); }
		EventManager* GetEventManager()const { return _eventManager.get(); }
		WorldManager* GetWorldManager()const { return _worldManager.get(); }
		ResourceManager* GetResourceManager()const { return _resourceManager.get(); }
		CameraManager* GetCameraManager()const { return _cameraManager.get(); }
		CollisionManager* GetCollisionManager()const { return _collisionManager.get(); }

	private:
		std::unique_ptr<World> _world;

		std::unique_ptr<CollisionManager> _collisionManager;
		std::unique_ptr<InputManager> _inputManager;
		std::unique_ptr<TimeManager> _timeManager;
		std::unique_ptr<EventManager> _eventManager;
		std::unique_ptr<WorldManager> _worldManager;
		std::unique_ptr<ResourceManager> _resourceManager;
		std::unique_ptr<CameraManager> _cameraManager;
	};


}