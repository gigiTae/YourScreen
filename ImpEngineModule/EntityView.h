#pragma once

#include "EntityIterator.h"

namespace ImpEngineModule
{
	namespace Internal
	{
		/// <summary>
		/// range-for�� ���� EntityView Ŭ����
		/// </summary>
		class EntityView
		{
		public:
			EntityView(const EntityIterator& first, const EntityIterator& last);

			/// firstIterator ��ȯ
			const EntityIterator& begin() const { return _firstIterator; }
			/// lastIterator ��ȯ
			const EntityIterator& end() const { return _lastIterator; }

		private:
			EntityIterator _firstIterator;
			EntityIterator _lastIterator;
		};

	}
}
