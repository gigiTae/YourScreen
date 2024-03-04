#include "EngineModulePCH.h"
#include "QuaternionType.h"

/// <summary>
/// quaternion
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<ImpMath::Quaternion>()
{
	static QuaternionType type;

	return &type;
}