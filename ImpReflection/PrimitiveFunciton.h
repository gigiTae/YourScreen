#pragma once


namespace ImpReflection
{
	class Type;

	/// GetPrimitiveType 선언
	/// 만약 T타입에 대한 구현이 없으면 링크 오류 발생 
	template<typename T>
	Type* GetPrimitiveType();
}

