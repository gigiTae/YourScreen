#pragma once

#include "TypeConverter.h"
#include "VectorType.h"
#include "TypeManager.h"

namespace ImpReflection
{
	class Type;

	/// ���ø� Ư��ȭ�� ����� std::vector Ư��ȭ
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
