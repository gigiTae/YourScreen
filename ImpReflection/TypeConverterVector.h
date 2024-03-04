#pragma once

#include "TypeConverter.h"
#include "VectorType.h"
#include "TypeManager.h"

namespace ImpReflection
{
	class Type;

	/// 탬플릿 특수화를 사용한 std::vector 특수화
	template <typename T>
	class TypeConverter<std::vector<T>> : public ITypeConverter
	{
	public:
		static Type* Get()
		{
			static VectorType<T> type;
			return &type;
		}

	};


}
