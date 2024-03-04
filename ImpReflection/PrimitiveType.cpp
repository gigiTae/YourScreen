#include "ImpReflectionPCH.h"
#include "PrimitiveType.h"

/// 템플릿 함수 명시적을 선언한다.

/// <summary>
/// int
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<int>()
{
	static IntType type;

	return &type;
}

/// <summary>
/// int
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<unsigned int>()
{
	static UnsignedIntType type;
	
	return &type;
}

/// <summary>
/// float
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<float>()
{
	static FloatType type;

	return &type;
}
/// <summary>
/// double
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<double>()
{
	static DoubleType type;

	return &type;
}

/// <summary>
/// bool
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<bool>()
{
	static BoolType type;

	return &type;
}

/// <summary>
/// long
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<long>()
{
	static LongType type;

	return &type;
}

/// <summary>
/// long long
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<long long>()
{
	static LongLongType type;

	return &type;
}

/// <summary>
/// usigned long long
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<unsigned long long>()
{
	static UnsignedLongLongType type;

	return &type;
}

