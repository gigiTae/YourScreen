#pragma once


namespace ImpReflection
{
	/// <summary>
	/// 리플렉션 여부를 판별하고
	/// 리플렉션한 구조체의 경우에는 재정의하여 사용한다.
	/// 솔직히 클래스명이 애매하다....
	/// </summary>
	template <typename T>
	class Meta
	{
	public:
		constexpr static bool IsReflect = false;
	};

}
