#include "EngineModulePCH.h"
#include "ImpColorType.h"

/// <summary>
/// ImpColor
/// </summary>
template<>
ImpReflection::Type* ImpReflection::GetPrimitiveType<ImpStructure::ImpColor>()
{
	static ImpReflection::ImpColorType type;

	return &type;
}