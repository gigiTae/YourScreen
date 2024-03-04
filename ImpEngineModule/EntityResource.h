#pragma once
#include "Resource.h"


namespace ImpEngineModule
{
	class Entity;

	// <summary>
	// Entity�� ������ ������ �ִ� ���ҽ� ( ����Ƽ Prefab ����)
	// Entity�� �����ؼ� �����Ѵ�.(������ Ÿ�� ���� )
	// 
	// ������Ʈ Ǯ���� ������ �����Ѵ�. 
	// ������ Ǯ�ǻ���� �ʰ��ϴ°�� �������� �Ҵ��ؼ� ����Ѵ�.
	// </summary>
	class EntityResource : public Resource
	{
	public:
		// �ε��� Entity�� ��ο� PoolSize�� �Ҵ��Ѵ�.
		EntityResource(const std::wstring& path, size_t poolSize = 1);
		EntityResource(EntityResource* other) = delete;

		~EntityResource();
		
		// Prototype�� �����Ѵ�.
		void Clone(Entity* ent) const;

		// Entity ���ҽ� �����͸� �о Entity��ü�� ����
		void Load()override;

		// ��ƼƼ Ǯ���� ��ƼƼ�� ���´�.
		std::shared_ptr<Entity> GetEntity();

		// ������ Ÿ���� ���´�
        const std::unique_ptr<Entity>& GetPrototype()const;

	private:
		void LoadEntity(); 

		void FillEntityPool();

		// �����ؼ� Entity�� �����Ѵ�.
		[[nodiscard]] Entity* Clone()const;

	private:   
		size_t _poolSize; 

		std::vector<std::shared_ptr<Entity>> _entityPool;
		std::unique_ptr<Entity> _prototype; 
	};
	
}
