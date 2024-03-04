#pragma once



namespace ImpEngineModule 
{
	using TypeIndex = std::type_index;

	/// <summary>
	/// Ÿ�Կ� �ش��ϴ� TypeIndex�� ��ȯ�Ѵ�.
	/// </summary>
	template <typename T>
	TypeIndex GetTypeIndex()
	{
		static TypeIndex index = std::type_index(typeid(T));
		return index;
	}
	
	static TypeIndex EXCEPTION = std::type_index(typeid(ImpReflection::Exception));
}
