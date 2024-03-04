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

			/// EndIteraotorȮ��
			bool IsEnd() const;

			/// ���������� Entity�� �����ϴ���
			bool IsIncludeToBeDestroyed()const { return _isIncludeToBeDestroyed; }

			/// Get World
			World* GetWorld()const { return _world; }

			/// Entity ��ȯ
			Entity* Get()const;

			/// Entity ��ȯ
			Entity* operator*()const { return Get(); }

			/// �� ������
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

			/// �� ������
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

			/// ���� ������
			EntityComponentIterator<Types...>& operator++();

		private:
			size_t _index;
			class ImpEngineModule::World* _world;
			bool _isEnd = false;
			bool _isIncludeToBeDestroyed;
		};

		template <typename...Types>
		ImpEngineModule::Internal::EntityComponentIterator<Types...>& // ��ȯ�� 
			ImpEngineModule::Internal::EntityComponentIterator<Types...>::operator++()
		{
			++_index;
			while (_index < _world->GetSize() && // EndIterator�� �ƴϴ�
				(Get() == nullptr || !Get()->template HasComponent<Types...>() || // �������ڿ� �ش��ϴ� ������Ʈ�� ���� ���
					(Get()->GetState() != Entity::EntityState::Active && !IsIncludeToBeDestroyed()))) // Ȱ��ȭ Entity�� ������ ���
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
