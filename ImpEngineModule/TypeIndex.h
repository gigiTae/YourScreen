#pragma once



namespace ImpEngineModule 
{
	using TypeIndex = std::type_index;

	/// <summary>
	/// 타입에 해당하는 TypeIndex를 반환한다.
	/// </summary>
	template <typename T>
	TypeIndex GetTypeIndex()
	{
		static TypeIndex index = std::type_index(typeid(T));
		return index;
	}
	
	static TypeIndex EXCEPTION = std::type_index(typeid(ImpReflection::Exception));
}
