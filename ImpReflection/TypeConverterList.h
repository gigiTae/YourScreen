#pragma once

#include "TypeConverter.h"
#include "ListType.h"

namespace ImpReflection
{
	class Type;

	template <typename T>
	class TypeConverter<std::list<T>> : public ITypeConverter
	{
	public:
		static Type* Get()
		{
			static ListType<T> type;
			return &type;
		}
	};
}