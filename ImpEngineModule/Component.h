#pragma once

namespace ImpEngineModule::Physics
{
	struct Collision;
}

namespace ImpEngineModule
{
	class World;
	class Entity;

	/// <summary>
	/// Entity�� ��ܼ� �����͸� ������ ������ �����ϴ� ��ü
	/// </summary>
	class Component
	{
		friend class Entity;
		
	public:
		Component();
		virtual ~Component();

		// ��������� 
		virtual Component* Clone(Component* clone = nullptr)const abstract;

		// World�� Entity�� �����Ѵ�.
		void Set(World* world, Entity* ent);
		 
		// Component�� �ʱ���¸� �����Ѵ�.
		virtual void Initialize() {};

		// ���� World�� �������� ȣ���Ѵ�.
		virtual void Start() {};

		// �̹� �������� ������ ����.
		virtual void Update(float dt) {};

		// Component�� ����Ʋ �ʱ�ȭ�Ѵ�.
		virtual void Finalize() {};

#pragma region Collision

		virtual void OnCollsiionEnter(const Physics::Collision& collision) {}
		virtual void OnCollsiionExit(const Physics::Collision& collision) {}
		virtual void OnCollsiionStay(const Physics::Collision& collision) {}

		virtual void OnTriggerEnter(const Physics::Collision& collision) {}
		virtual void OnTriggerExit(const Physics::Collision& collision) {}
		virtual void OnTriggerStay(const Physics::Collision& collision) {}

#pragma endregion Collision

		// World ȹ��
		World* GetWorld() const { return _world; }

		// Entity ȹ��
		Entity* GetEntity()const { return _entity; }

		// Component�� Ȱ��ȭ ���¸� Ȯ��
		bool IsActive() const { return _isActive; }

		// ������Ʈ�� Ȱ��ȭ ���θ� ����
		void SetActive(bool isActive) { _isActive = isActive; }

		// �ٸ� ������Ʈ�� �����´�.
		template<typename T>
		T* GetComponent()const;

		// ������Ʈ�� �߰��Ѵ�.
		template<typename T, typename... Types>
		T* AddComponent(Types ...);

	private:
		bool _isActive;
		World* _world;
		Entity* _entity;
	};

	template<typename T, typename... Types>
	T* ImpEngineModule::Component::AddComponent(Types ...)
	{
		Entity* ent = GetEntity();

		return ent->template AddComponent<T>(Types...);
	}

	template<typename T>
	T* ImpEngineModule::Component::GetComponent() const
	{
		Entity* ent = GetEntity();
		assert(ent && "Entity�� nullptr");

		return ent->template GetComponent<T>();
	}


}