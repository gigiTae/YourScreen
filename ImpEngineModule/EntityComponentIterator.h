#pragma once

namespace ImpEngineModule
{
	class World;
	class Entity;

	namespace Internal
	{
		/// <summary>
		/// EntityComponentIterator
		/// </summary>
		template <typename... Types>
		class EntityComponentIterator
		{
		public:
			EntityComponentIterator(World* world, size_t index,
				bool isEnd, bool isIncldueToBeDestroyed);

			/// Iterator Index 
			size_t GetIndex()const { return _index; }

			/// EndIteraotor확인
			bool IsEnd() const;

			/// 삭제예정인 Entity를 포함하는지
			bool IsIncludeToBeDestroyed()const { return _isIncludeToBeDestroyed; }

			/// Get World
			World* GetWorld()const { return _world; }

			/// Entity 반환
			Entity* Get()const;

			/// Entity 반환
			Entity* operator*()const { return Get(); }

			/// 비교 연산자
			bool operator==(const EntityComponentIterator<Types...>& other)const
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

			/// 비교 연산자
			bool operator!=(const EntityComponentIterator<Types...>& other)const
			{
				if (_world != other._world)
				{
					return true;
				}

				if (IsEnd())
				{
					return !other.IsEnd();
				}
					
				return _index != other._index;
			}

			/// 증감 연산자
			EntityComponentIterator<Types...>& operator++();

		private:
			size_t _index;
			class ImpEngineModule::World* _world;
			bool _isEnd = false;
			bool _isIncludeToBeDestroyed;
		};

		template <typename...Types>
		ImpEngineModule::Internal::EntityComponentIterator<Types...>& // 반환값 
			ImpEngineModule::Internal::EntityComponentIterator<Types...>::operator++()
		{
			++_index;
			while (_index < _world->GetSize() && // EndIterator가 아니다
				(Get() == nullptr || !Get()->template HasComponent<Types...>() || // 가변인자에 해당하는 컴포넌트가 없는 경우
					(Get()->GetState() != Entity::EntityState::Active && !IsIncludeToBeDestroyed()))) // 활성화 Entity만 쿼리한 경우
			{
				++_index;
			}

			if (_index >= _world->GetSize())
			{
				_isEnd = true;
			}

			return *this;
		}

		template <typename... Types>
		ImpEngineModule::Internal::EntityComponentIterator<Types...>::EntityComponentIterator(
			World* world, size_t index, bool isEnd, bool isIncldueToBeDestroyed)
			:_world(world), _index(index), _isEnd(isEnd), _isIncludeToBeDestroyed(isIncldueToBeDestroyed)
		{
			if (_index >= _world->GetSize())
			{
				_isEnd = true;
			}
		}

		template <typename...Types>
		Entity* ImpEngineModule::Internal::EntityComponentIterator<Types...>::Get() const
		{
			if (IsEnd())
			{
				return nullptr;
			}

			return _world->GetByIndex(_index);
		}

		template <typename... Types>
		bool ImpEngineModule::Internal::EntityComponentIterator<Types...>::IsEnd() const
		{
			return _index >= _world->GetSize() || _isEnd;
		}

	}
}
