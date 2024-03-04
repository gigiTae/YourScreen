#pragma once

namespace ImpEngineModule
{
	class Entity;
	
	namespace Internal
	{
		template<typename... Types>
		class EntityComponentIterator;

		/// <summary>
		/// ECS시스템에서 내부적으로 사용하는 View
		/// </summary>
		template<typename... Types>
		class EntityComponentView
		{
		public:
			EntityComponentView(const EntityComponentIterator<Types...>& first,
				const EntityComponentIterator<Types...>& last);

			/// return: firstIterator
			const EntityComponentIterator<Types...> begin()const
			{
				return _firstIterator;
			}

			/// return: lastIterator
			const EntityComponentIterator<Types...> end()const
			{
				return _lastIterator;
			}

		private:
			EntityComponentIterator<Types...> _firstIterator;
			EntityComponentIterator<Types...> _lastIterator;
		};

		template<typename... Types>
		ImpEngineModule::Internal::EntityComponentView<Types ...>::EntityComponentView(
			const EntityComponentIterator<Types...>& first,
			const EntityComponentIterator<Types...>& last)
			:_firstIterator(first),_lastIterator(last)
		{
			if (_firstIterator.Get() == nullptr || 
				(_firstIterator.Get()->GetState() != ImpEngineModule::Entity::EntityState::Active &&
				!_firstIterator.IsIncludeToBeDestroyed()) ||
				!_firstIterator.Get()->template HasComponent<Types...>())
			{
				++_firstIterator;
			}
		}
		
	}
}