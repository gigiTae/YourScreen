#pragma once


/// <summary>
/// String 관련 함수
/// </summary>
namespace ImpStringHelper
{
	static const std::string empty = {""};

	/// { 와 짝을 이루는 } 를 찾는다
	size_t FindClosingBrace(const std::string& data, size_t begin);

	/// { 를 찾는 함수
	size_t FindOpeningBrace(const std::string& data, size_t begin);
	
	/// Vector [index]를 찾는 함수
	size_t FindVectorIndex(const std::string& data, size_t begin);

	/// name의 네임스페이스를 제거한다.
	std::string EraseNamespace(const std::string& name);

	/// Type의 이름을 찾아서 반환한다.
	std::string GetTypeName(const std::string& data, size_t begin);

	/// [KeyType{value}] 에서 키정보를 스트링으로 가져온다
	std::string GetKeySubstr(const std::string& data, size_t begin);

	/// 문자열의 특정 문자열을 지운다
	void RemoveSubstring(std::string& main, const std::string eraseStr);

	/// 멤버변수의 _를 지운다.
	std::string EraseUnderbar(const std::string& name);
}