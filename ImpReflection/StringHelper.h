#pragma once


/// <summary>
/// String ���� �Լ�
/// </summary>
namespace ImpStringHelper
{
	static const std::string empty = {""};

	/// { �� ¦�� �̷�� } �� ã�´�
	size_t FindClosingBrace(const std::string& data, size_t begin);

	/// { �� ã�� �Լ�
	size_t FindOpeningBrace(const std::string& data, size_t begin);
	
	/// Vector [index]�� ã�� �Լ�
	size_t FindVectorIndex(const std::string& data, size_t begin);

	/// name�� ���ӽ����̽��� �����Ѵ�.
	std::string EraseNamespace(const std::string& name);

	/// Type�� �̸��� ã�Ƽ� ��ȯ�Ѵ�.
	std::string GetTypeName(const std::string& data, size_t begin);

	/// [KeyType{value}] ���� Ű������ ��Ʈ������ �����´�
	std::string GetKeySubstr(const std::string& data, size_t begin);

	/// ���ڿ��� Ư�� ���ڿ��� �����
	void RemoveSubstring(std::string& main, const std::string eraseStr);

	/// ��������� _�� �����.
	std::string EraseUnderbar(const std::string& name);
}