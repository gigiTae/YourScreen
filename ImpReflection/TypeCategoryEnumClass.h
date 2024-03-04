#pragma once

namespace ImpReflection
{
	enum class TypeCategory
	{
		Class, // Struct, Class 
		Primitive, // int, float, bool ���
		Map, // std::map
		UnorderedMap, // std::unordered_map
		Vector, // std::vector
		List, // std::list
		EnumClass, // enum class , enum(x)
		None // error
	};

	/// Tag�� ����ؼ� ����
	enum class Tag
	{
		Component, // 
		Default,
	};

}