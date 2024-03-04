#include "EngineModulePCH.h"
#include "EntityIterator.h"
#include "World.h"
#include "Entity.h"

ImpEngineModule::Internal::EntityIterator::EntityIterator(ImpEngineModule::World* world,
	size_t index, bool isEnd, bool isIncludeToBeDestroyed)
	:_world(world),_index(index),_isEnd(isEnd),
	_isIncludeToBeDestroyed(isIncludeToBeDestroyed)
{
	if (_index >= world->GetSize())
	{
		_isEnd = true;
	}
}

ImpEngineModule::Entity* ImpEngineModule::Internal::EntityIterator::Get() const
{
	if (IsEnd())
		return nullptr;

	return _world->GetEntity(_index);
}

bool ImpEngineModule::Internal::EntityIterator::IsEnd() const
{
	return _isEnd || _index >= _world->GetSize();
}

ImpEngineModule::Internal::EntityIterator& ImpEngineModule::Internal::EntityIterator::operator++()
{
	++_index;
	while (_index < _world->GetSize() && (Get() == nullptr ||
		(Get()->GetState() != Entity::EntityState::Active && !_isIncludeToBeDestroyed)))
	{
		++_index;
	}

	if (_index >= _world->GetSize())
	{
		_isEnd = true;
	}

	return *this;
}

bool ImpEngineModule::Internal::EntityIterator::operator==(const EntityIterator& other) const
{
	if (_world != other._world)
	{
		return false;
	}

	if (IsEnd())
	{
		return other.IsEnd();
	}

	return _index == other._index;
}

bool ImpEngineModule::Internal::EntityIterator::operator!=(const EntityIterator& other) const
{
	if (_world != other._world)
	{
		return true;
	}

	if (IsEnd())
	{
		return !other.IsEnd();
	}

	return _index == other._index;
}
