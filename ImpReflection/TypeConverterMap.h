#pragma once

#include "TypeConverter.h"
#include "MapType.h"

namespace ImpReflection
{
	template <typename T1, typename T2>
	class MapTypeConverter : public ITypeConverter
	{
	public:
		static Type* Get()
		{
			static MapType<T1, T2> type;
			return &type;
		}
	};


	
}
