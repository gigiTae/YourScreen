#pragma once

namespace ImpReflection
{
	enum class TypeCategory
	{
		Class, // Struct, Class 
		Primitive, // int, float, bool 등등
		Map, // std::map
		UnorderedMap, // std::unordered_map
		Vector, // std::vector
		List, // std::list
		EnumClass, // enum class , enum(x)
		None // error
	};

	/// Tag를 사용해서 쿼리
	enum class Tag
	{
		Component, // 
		Default,
	};

}