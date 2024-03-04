#pragma once
#include "ITypeConverter.h"
#include "UnorderedMapType.h"

namespace ImpReflection
{
	template <typename T1, typename T2>
	class UnorderedMapTypeConverter : public ITypeConverter
	{
	public:
		static Type* Get()
		{
			static UnorderedMapType<T1, T2> type;
			return &type;
		}


	};

}

