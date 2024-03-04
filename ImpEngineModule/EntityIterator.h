#pragma once

namespace ImpEngineModule
{
	class Entity;
	class World;

	namespace Internal
	{
		/// <summary>
		/// Entity Iteraotor
		/// </summary>
		class EntityIterator
		{
		public:
			EntityIterator(ImpEngineModule::World* world, size_t index
				, bool isEnd, bool isIncludeToBeDestroyed);

			Entity* Get()const;

			/// EndIterator Ȯ��
			bool IsEnd() const;

			/// ���������� Entity ���� ����
			bool IsIncludeToBeDestroyed()const { return _isIncludeToBeDestroyed; }

#pragma region operator
			/// * ������
			Entity* operator*() const { return Get(); }

			/// ���� ������
			EntityIterator& operator++();

			/// �� ������
			bool operator==(const EntityIterator& other) const;

			/// �� ������
			bool operator!=(const EntityIterator& other) const;

#pragma endregion operator

		private:
			size_t _index;
			class ImpEngineModule::World* _world;

			bool _isEnd = false;
			bool _isIncludeToBeDestroyed;
		};
	}
}

