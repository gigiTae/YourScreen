#pragma once
#include "Resource.h"


namespace ImpEngineModule
{
	class Entity;

	// <summary>
	// Entity의 정보를 가지고 있는 리소스 ( 유니티 Prefab 개념)
	// Entity를 복사해서 전달한다.(프로토 타입 패턴 )
	// 
	// 오브젝트 풀링의 역할을 같이한다. 
	// 고정된 풀의사이즈를 초과하는경우 동적으로 할당해서 사용한다.
	// </summary>
	class EntityResource : public Resource
	{
	public:
		// 로드할 Entity의 경로와 PoolSize를 할당한다.
		EntityResource(const std::wstring& path, size_t poolSize = 1);
		EntityResource(EntityResource* other) = delete;

		~EntityResource();
		
		// Prototype을 복사한다.
		void Clone(Entity* ent) const;

		// Entity 리소스 데이터를 읽어서 Entity객체를 생성
		void Load()override;

		// 엔티티 풀에서 엔티티를 얻어온다.
		std::shared_ptr<Entity> GetEntity();

		// 프로토 타입을 얻어온다
        const std::unique_ptr<Entity>& GetPrototype()const;

	private:
		void LoadEntity(); 

		void FillEntityPool();

		// 복사해서 Entity를 생성한다.
		[[nodiscard]] Entity* Clone()const;

	private:   
		size_t _poolSize; 

		std::vector<std::shared_ptr<Entity>> _entityPool;
		std::unique_ptr<Entity> _prototype; 
	};
	
}
