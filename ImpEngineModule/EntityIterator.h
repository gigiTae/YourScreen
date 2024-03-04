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

			/// EndIterator 확인
			bool IsEnd() const;

			/// 삭제예정인 Entity 포함 여부
			bool IsIncludeToBeDestroyed()const { return _isIncludeToBeDestroyed; }

#pragma region operator
			/// * 연산자
			Entity* operator*() const { return Get(); }

			/// 증감 연산자
			EntityIterator& operator++();

			/// 비교 연산자
			bool operator==(const EntityIterator& other) const;

			/// 비교 연산자
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

