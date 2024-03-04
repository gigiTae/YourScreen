#include "EngineModulePCH.h"
#include "Entity.h"
#include "Component.h"
#include "World.h"
#include "Collision.h"

ImpEngineModule::Entity::Entity(World* world, size_t id)
	:_world(world), _id(id), _state(EntityState::Active),
	_name("Entity" + std::to_string(id))
{}

ImpEngineModule::Entity& ImpEngineModule::Entity::operator=(const Entity& other)
{
	// 월드와 ID는 복사하지 않는다.
	_state = other._state;
	_name = other._name;

	// 같지 않은 컴포넌트들을 제거한다.
	for (auto iter = _components.begin(); iter != _components.end();)
	{
		if (other._components.find(iter->first) == other._components.end())
		{
			if (iter->second != nullptr)
			{
				delete iter->second;
			}
			iter = _components.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// 컴포넌트들을 복사한다.
	for (const auto& [index, component] : other._components)
	{
		auto iter = _components.find(index);
		// 새로 컴포넌트를 생성하는경우
		if (iter == _components.end())
		{
			Component* clone = component->Clone();
			
			_components.insert({ index,clone });
		}
		else // 복사
		{
			component->Clone(iter->second);
		}
	}

	return *this;
}

ImpEngineModule::Entity::Entity(const Entity& other)
	:_world(other._world), _state(other._state), _name(other._name)
{
	_id = 0;

	// 컴포넌트들의 복사
	for (const auto& [index, component] : other._components)
	{
		_components.insert({ index, component->Clone() });
	}
}

ImpEngineModule::Entity::Entity()
	:_world(nullptr), _id(0), _state(EntityState::Active)
	, _name("Entity")
{
}

ImpEngineModule::Entity::~Entity()
{
	DestroyAllComponents();
}

void ImpEngineModule::Entity::DestroyAllComponents()
{
    for (auto& iter : _components)
    {
		iter.second->Finalize();
		delete iter.second;
	}

	_components.clear();
}

void ImpEngineModule::Entity::DestroyComponent(TypeIndex index)
{
	auto iter = _components.find(index);

	if (iter != _components.end())
	{
		Component* component = iter->second;
		component->Finalize();
		delete component;
		_components.erase(iter);
	}
}

void ImpEngineModule::Entity::Start()
{
	for (auto& iter : _components)
	{
		iter.second->Start();
	}
}

void ImpEngineModule::Entity::Update(float dt)
{
	for (auto& iter : _components)
	{
		Component* comp = iter.second;

		if (comp->IsActive())
		{
			comp->Update(dt);
		}
	}
}

void ImpEngineModule::Entity::OnCollsiionEnter(const Physics::Collision& collision)
{
	for (auto&[index,component] : _components)
	{
		if (component->IsActive())
		{
			component->OnCollsiionEnter(collision);
		}
	}
}

void ImpEngineModule::Entity::OnCollsiionExit(const Physics::Collision& collision)
{

	for (auto& [index, component] : _components)
	{
		if (component->IsActive())
		{
			component->OnCollsiionExit(collision);
		}
	}
}

void ImpEngineModule::Entity::OnCollsiionStay(const Physics::Collision& collision)
{

	for (auto& [index, component] : _components)
	{
		if (component->IsActive())
		{
			component->OnCollsiionStay(collision);
		}
	}
}

void ImpEngineModule::Entity::OnTriggerEnter(const Physics::Collision& collision)
{
	for (auto& [index, component] : _components)
	{
		if (component->IsActive())
		{
			component->OnTriggerEnter(collision);
		}
	}
}

void ImpEngineModule::Entity::OnTriggerExit(const Physics::Collision& collision)
{

	for (auto& [index, component] : _components)
	{
		if (component->IsActive())
		{
			component->OnTriggerExit(collision);
		}
	}
}

void ImpEngineModule::Entity::OnTriggerStay(const Physics::Collision& collision)
{
	for (auto& [index, component] : _components)
	{
		if (component->IsActive())
		{
			component->OnTriggerStay(collision);
		}
	}
}

void ImpEngineModule::Entity::SetWorld(World* world)
{
	assert(world);

	_world = world;
	_id = _world->AssignNewID();

	// 컴포넌트들 연결.
	for (auto& iter : _components)
	{
		Component* comp = iter.second;
		comp->Set(_world, this);
	}
}

size_t ImpEngineModule::Entity::GetID() const
{
    return _id;
}

ImpEngineModule::Component* ImpEngineModule::Entity::GetComponent(TypeIndex index)
{
	auto iter = _components.find(index);

	if (iter != _components.end())
	{
		return iter->second;
	}

	return nullptr;
}

ImpEngineModule::Component* ImpEngineModule::Entity::AddComponent(void* component, TypeIndex index)
{
	Component* comp = reinterpret_cast<Component*>(component);

	assert(_components.find(index) == _components.end() && "중복된 컴포넌트입니다.");

	comp->Set(_world, this);

	_components.insert({ index,comp });

	return comp;
}

bool ImpEngineModule::Entity::HasComponent(TypeIndex index)
{
	return _components.find(index) != _components.end();
}
