#pragma once

#ifndef IMP_REFLECTION_HEADER
#define IMP_REFLECTION_HEADER

#include "ImpReflectionPCH.h"

#include "ReflectionMacro.h"
#include "TypeCategoryEnumClass.h"
#include "PrimitiveFunciton.h"

/// Type
#include "Type.h"
#include "PrimitiveType.h"
#include "ClassType.h"
#include "StringType.h"
#include "EnumClassType.h"
#include "MapType.h"
#include "VectorType.h"
#include "ListType.h"
#include "UnorderedMapType.h"

/// Converter
#include "ITypeConverter.h"
#include "TypeConverter.h"
#include "TypeConverterMap.h"
#include "TypeConverterVector.h"
#include "TypeConverterList.h"
#include "UnorderedMapTypeConverter.h"
#include "Meta.h"

#include "TypeManager.h"

#endif // !IMP_REFLECITON_HEADER