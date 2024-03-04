#pragma once
#include "IWorld.h"

#include "Entity.h"
#include "ComponentHandle.h"
#include "EntityComponentView.h"
#include "EntityComponentIterator.h"
#include "EntityView.h"
#include "EntityIterator.h"

namespace ImpEngineModule
{
	class EventManager;
	class InputManager;
	class ResourceManager;

	/// <summary>
	/// World : Entity�� �����ϴ� �����̳��̴�.
	/// Entity Component Base : Component�� �����͵� �����ϰ� ������ �����Ѵ�.
	/// </summary>
	class World : public IWorld
	{
	public:
		World();
		~World();

		World(const World& other) = delete;
		World& operator =(const World& other) = delete;

	public:
#pragma region Event
		
		// Entity�鿡�� ������ ������ �ּ����� �Ŵ������� ��Ÿ����.
		void Initialize(EventManager* eventMgr, InputManager* inputMgr, ResourceManager* resMgr);
		
		// World Reset
		void Reset() override;

		// World �������� ȣ��
		void Start() override;

		// �� ������ ȣ�� 
		void Update(float dt) override;

		// World ����
		void Finalize();

#pragma endregion Event

#pragma region Enitty

		// ���������� ��ƼƼ���� �����.
		void CleanUp();

		// Entity ����
		Entity* CreateEntity();

		// ��ƼƼ�� �ܺο��� ����Ѵ�
		// ����� ��ƼƼ�� ������ ID�ް� �ش����� ����ȴ�.
		// ���Ŀ��� Start�� ȣ���Ѵ�. 
		Entity* RegisterEntity(const std::shared_ptr<Entity>& ent);

		// Entity ����
		void DestroyEntity(Entity* ent);

		// Entity ����
		void DestroyEntity(size_t id);

		// Entity �迭 Index Ž��
		Entity* GetByIndex(size_t index)const;

		// Entity ID�� Ž��
		Entity* GetEntity(size_t id)const;

		// Entity �̸����� Ž��
		Entity* GetEntity(const std::string& name)const;

		// ���ο� Entity ID �߱�
		size_t AssignNewID() { return ++_lastEntityID; }

		// LastEntity ID ȹ��
		size_t GetLastEntityID()const { return _lastEntityID; }

		// ��ƼƼVector�� ����� ��ȯ
		size_t GetSize()const { return _entities.size(); }

		const std::vector<std::shared_ptr<Entity>>& GetEntities()const { return _entities; }

		// <summary>
		// �������ڿ� �ش��ϴ� Ÿ���� ������Ʈ�� ����ִ� Entity�鸸 �����Ͽ�
		// ���޹��� �Լ��� ȣ���Ѵ�.
		// </summary>
		// <typeparam name="...Types">������ ������Ʈ Ÿ�Ե�</typeparam>
		// <param name="viewFunc">�����Լ�</param>
		// <param name="isIncludeToBeDestroyed">���������� Entity ���Կ���</param>
		template<typename... Types>
		void Each(typename std::common_type<std::function<void(Entity*, ComponentHandle<Types>...)>>::type viewFunc,
			bool isIncludeToBeDestroyed = false);

		// �������ڿ� �ش��ϴ� Ÿ���� View�� �����Ѵ�.
		template<typename... Types>
		Internal::EntityComponentView<Types...> Each(bool isIncludeToBeDestroyed);

		// ��� Entity�鿡 ���ڷ� ���� �Լ��� �����Ѵ�.
		void All(std::function<void(Entity*)> viewFunc, bool isIncludeToBeDestroyed = false);

		// ��� ��ƼƼ�� ��ȸ������ EntityView�����´�.
		Internal::EntityView All(bool isIncludeToBeDestroyed = false);

#pragma endregion Entity

#pragma region Manager
		InputManager* GetInputManager()const { return _inputManager; }
		ResourceManager* GetResourceManager()const { return _resourceManager; }
		EventManager* GetEventManager()const { return _eventManager; }
#pragma endregion Manager

	private:
		size_t _lastEntityID = 0;
		std::vector<std::shared_ptr<Entity>> _entities;

		EventManager* _eventManager;
		ResourceManager* _resourceManager;
		InputManager* _inputManager;
	};


#pragma region template function
	template<typename... Types>
	Internal::EntityComponentView<Types...>
		ImpEngineModule::World::Each(bool isIncludeToBeDestroyed)
	{
		ImpEngineModule::Internal::EntityComponentIterator<Types...>
			first(this, 0, false, isIncludeToBeDestroyed);

		ImpEngineModule::Internal::EntityComponentIterator<Types...>
			last(this, GetSize(), true, isIncludeToBeDestroyed);

		return Internal::EntityComponentView<Types...>(first, last);
	}

	template<typename...Types>
	void ImpEngineModule::World::Each(typename std::common_type<std::function<void(Entity*, ComponentHandle<Types>...)>>::type viewFunc,
		bool isIncludeToBeDestroyed /*= false*/)
	{
		for (Entity* ent : Each<Types...>(isIncludeToBeDestroyed))
		{
			// ���ø� �ĸ������� 
			// ent,GetComponent<Transform>(),GetComponent<BoxCollider>(),... ������ Ȯ���Ѵ�.
			viewFunc(ent, ent->template GetComponent<Types>()...);
		}
	}
#pragma endregion

}

