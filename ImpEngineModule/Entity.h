#pragma once
#include "IEntity.h"

namespace ImpEngineModule::Physics
{
	struct Collision;
}

namespace ImpEngineModule
{
	class World;
	class Component;

	/// <summary>
	/// 컴포넌트들을 관리하는 컨테이너 역할을 한다.
	/// Entity는 컴포넌트를 관리하는 로직외에는 아무런 로직을 수행하지않는다. 
	/// </summary>
	class Entity : public IEntity
	{
	public:
		enum class EntityState : int
		{
			Active, // 활성화
			Destroyed, // 파괴
		};

	public:
		Entity();
		Entity(World* world, size_t id);
		Entity(const Entity& other);
		Entity& operator =(const Entity& other);

		~Entity() override;

	public:
		// Entity에 할당한 컴포넌트들을 모두 삭제한다.
		void DestroyAllComponents() override;

		// TypeIndex에 해당하는 컴포넌트를 삭제한다.
		void DestroyComponent(TypeIndex index);

		// Component들의 Start를 호출한다.
		void Start() override;

		// Component들의 Update를 호출한다.
		void Update(float dt) override;

#pragma region Collision

		virtual void OnCollsiionEnter(const Physics::Collision& collision);
		virtual void OnCollsiionExit(const Physics::Collision& collision);
		virtual void OnCollsiionStay(const Physics::Collision& collision);

		virtual void OnTriggerEnter(const Physics::Collision& collision);
		virtual void OnTriggerExit(const Physics::Collision& collision);
		virtual void OnTriggerStay(const Physics::Collision& collision);

#pragma endregion Collision

		void SetID(size_t id)
        {
            _id = id;
        }

		// 월드를 연견한다.
		void SetWorld(World* world);

		// World를 가져온다 
		World* GetWorld()const { return _world; }

		// Entity 고유의 ID를 가져온다.
        size_t GetID() const;

		// Entity의 이름을 가져온다.
		std::string GetName() { return _name; }

		// Entity의 이름을 변경한다.
		void SetName(const std::string& name) { _name = name; }

		// Entity의 현재상태를 가져온다.
		EntityState GetState()const { return _state; }

		// 컴포넌트를 TypeIndex로 가져온다.
		Component* GetComponent(TypeIndex index);

		const std::unordered_map<TypeIndex, Component*>& GetComponents()const { return _components; }

		// 리플렉션을 이용한 컴포넌트 추가
		Component* AddComponent(void* component, TypeIndex index);

		// T타입에 해당하는 컴포넌트를 가져온다
		template <typename T>
		T* GetComponent();

		// 가변인자 템플릿을 사용해서 T 타입에 해당하는 T(Args...)
		// 생성자를 호출하고 Entity에 추가한다.
		template <typename T, typename... Args>
		T* AddComponent(Args&&... args);

		// TypeIndex로 컴포넌트 소유 여부확인
		bool HasComponent(TypeIndex index);
		
		// 컴포넌트를 소유하는지 확인한다.
		template<typename T>
		bool HasComponent();

		// <summary>
		// 컴포넌트들을 소유하는지 확인한다.
		// </summary>
		template<typename T1, typename T2, typename... Types>
		bool HasComponent();

	private:
		World* _world;
		size_t _id;
		std::string _name; 
		EntityState _state;
		std::unordered_map<TypeIndex, Component*> _components;

		friend class World;
	};

	template<typename T1, typename T2, typename... Types>
	bool ImpEngineModule::Entity::HasComponent()
	{
		return HasComponent<T1>() && HasComponent<T2, Types...>();
	}

	template<typename T>
	bool ImpEngineModule::Entity::HasComponent()
	{
		TypeIndex index = GetTypeIndex<T>();

		return _components.find(index) != _components.end();
	}

	template <typename T, typename... Args>
	T* ImpEngineModule::Entity::AddComponent(Args&&... args)
	{
	 	TypeIndex index = GetTypeIndex<T>();
		
		// 이미 생성한 컴포넌트는 생성하지 않는다.
		if (Component* component = GetComponent(index); component !=nullptr)
		{
			return reinterpret_cast<T*>(component);
		}

		// 가변인자 템플릿을 사용한 생성자
		T* component = new T(args...);
		component->Set(GetWorld(), this);

		_components.insert(make_pair(index, component));

		return component;
	}

	template <typename T>
	T* ImpEngineModule::Entity::GetComponent()
	{
		TypeIndex index = GetTypeIndex<T>();
		
		auto iter = _components.find(index);

		if (iter == _components.end())
			return nullptr;

		T* component = reinterpret_cast<T*>(iter->second);

		return component;
	}

}