#pragma once
#include "ITypeConverter.h"
#include "PrimitiveFunciton.h"
#include "Meta.h"

namespace ImpReflection
{
	class Type;

	/// <summary>
	/// T 타입에 해당하는 Type을 반환하는 Helper클래스
	/// </summary>
	template <typename T>
	class TypeConverter :public ITypeConverter
	{
	public:

		/// <summary>
		/// T 타입에 해당하는 type 을 얻는다.
		/// Meta의 static 변수를 통해서 리플렉션 여부를 판단하고
		/// 해당하는 Type을 얻는다. 만약 리플렉션되지 않은 타입은
		/// GetPrimitiveType을 호출한다. 만약 GetPrimitiveType에
		/// T타입의 구현이 없는경우 링크 오류가 발생한다. 
		/// </summary>
		static Type* Get()
		{
			if constexpr (Meta<T>::IsReflect)
			{
				return &Meta<T>::_type;
			}
			else
			{
				return GetPrimitiveType<T>();
			}
		}
	};


	
}