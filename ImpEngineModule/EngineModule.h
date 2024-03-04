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
	/// World, �Ŵ���Ŭ�������� �����Ѵ�.
	/// 
	/// </summary>
	class EngineModule
	{
	public:
		EngineModule();
		~EngineModule();

		// �ʱ�ȭ
		void Initialize(HWND hWnd, std::wstring startWorld);

		// ���ӷ��� ���۽� ȣ��
		void Start();
	
		// EngineModule���� ����
		float Update();
		
		// �̺�Ʈ�� ó���Ѵ�.
		void PostUpdate();

		//����
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