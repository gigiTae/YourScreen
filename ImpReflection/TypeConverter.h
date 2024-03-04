#pragma once
#include "ITypeConverter.h"
#include "PrimitiveFunciton.h"
#include "Meta.h"

namespace ImpReflection
{
	class Type;

	/// <summary>
	/// T Ÿ�Կ� �ش��ϴ� Type�� ��ȯ�ϴ� HelperŬ����
	/// </summary>
	template <typename T>
	class TypeConverter :public ITypeConverter
	{
	public:

		/// <summary>
		/// T Ÿ�Կ� �ش��ϴ� type �� ��´�.
		/// Meta�� static ������ ���ؼ� ���÷��� ���θ� �Ǵ��ϰ�
		/// �ش��ϴ� Type�� ��´�. ���� ���÷��ǵ��� ���� Ÿ����
		/// GetPrimitiveType�� ȣ���Ѵ�. ���� GetPrimitiveType��
		/// TŸ���� ������ ���°�� ��ũ ������ �߻��Ѵ�. 
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