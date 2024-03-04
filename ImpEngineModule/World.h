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
	/// World : Entity를 관리하는 컨테이너이다.
	/// Entity Component Base : Component가 데이터도 관리하고 로직도 수행한다.
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
		
		// Entity들에서 접근이 가능한 최소한의 매니져들을 나타낸다.
		void Initialize(EventManager* eventMgr, InputManager* inputMgr, ResourceManager* resMgr);
		
		// World Reset
		void Reset() override;

		// World 시작전에 호출
		void Start() override;

		// 매 프레임 호출 
		void Update(float dt) override;

		// World 종료
		void Finalize();

#pragma endregion Event

#pragma region Enitty

		// 삭제예정인 엔티티들을 지운다.
		void CleanUp();

		// Entity 생성
		Entity* CreateEntity();

		// 엔티티를 외부에서 등록한다
		// 등록한 엔티티는 고유의 ID받고 해당월드와 연결된다.
		// 그후에는 Start를 호출한다. 
		Entity* RegisterEntity(const std::shared_ptr<Entity>& ent);

		// Entity 삭제
		void DestroyEntity(Entity* ent);

		// Entity 삭제
		void DestroyEntity(size_t id);

		// Entity 배열 Index 탐색
		Entity* GetByIndex(size_t index)const;

		// Entity ID로 탐색
		Entity* GetEntity(size_t id)const;

		// Entity 이름으로 탐색
		Entity* GetEntity(const std::string& name)const;

		// 새로운 Entity ID 발급
		size_t AssignNewID() { return ++_lastEntityID; }

		// LastEntity ID 획득
		size_t GetLastEntityID()const { return _lastEntityID; }

		// 엔티티Vector의 사이즈를 반환
		size_t GetSize()const { return _entities.size(); }

		const std::vector<std::shared_ptr<Entity>>& GetEntities()const { return _entities; }

		// <summary>
		// 가변인자에 해당하는 타입의 컴포넌트를 들고있는 Entity들만 쿼리하여
		// 전달받은 함수를 호출한다.
		// </summary>
		// <typeparam name="...Types">쿼리할 컴포넌트 타입들</typeparam>
		// <param name="viewFunc">람다함수</param>
		// <param name="isIncludeToBeDestroyed">삭제예정인 Entity 포함여부</param>
		template<typename... Types>
		void Each(typename std::common_type<std::function<void(Entity*, ComponentHandle<Types>...)>>::type viewFunc,
			bool isIncludeToBeDestroyed = false);

		// 가변인자에 해당하는 타입의 View를 제공한다.
		template<typename... Types>
		Internal::EntityComponentView<Types...> Each(bool isIncludeToBeDestroyed);

		// 모든 Entity들에 인자로 받은 함수를 실행한다.
		void All(std::function<void(Entity*)> viewFunc, bool isIncludeToBeDestroyed = false);

		// 모든 엔티티를 순회가능한 EntityView가져온다.
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
			// 탬플릿 파리미터팩 
			// ent,GetComponent<Transform>(),GetComponent<BoxCollider>(),... 등으로 확장한다.
			viewFunc(ent, ent->template GetComponent<Types>()...);
		}
	}
#pragma endregion

}

