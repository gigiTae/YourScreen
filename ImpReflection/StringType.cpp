#include "ImpReflectionPCH.h"
#include "StringType.h"
#include "PrimitiveFunciton.h"

template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<std::string>()
{
	static StringType type;

	return &type;
}

template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<std::wstring>()
{
	static WstringType type;

	return &type;
}

