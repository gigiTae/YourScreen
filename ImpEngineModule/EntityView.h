#pragma once

#include "EntityIterator.h"

namespace ImpEngineModule
{
	namespace Internal
	{
		/// <summary>
		/// range-for를 위한 EntityView 클래스
		/// </summary>
		class EntityView
		{
		public:
			EntityView(const EntityIterator& first, const EntityIterator& last);

			/// firstIterator 반환
			const EntityIterator& begin() const { return _firstIterator; }
			/// lastIterator 반환
			const EntityIterator& end() const { return _lastIterator; }

		private:
			EntityIterator _firstIterator;
			EntityIterator _lastIterator;
		};

	}
}
