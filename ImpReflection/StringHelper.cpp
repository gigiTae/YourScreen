#include "ImpReflectionPCH.h"
#include "StringHelper.h"



size_t ImpStringHelper::FindClosingBrace(const std::string& data, size_t begin)
{
	size_t openingBrace = data.find('{', begin);

	size_t close = openingBrace + 1;

	int openCount = 1;

	while (data.size() > close)
	{
		if (data[close] == '{')
		{
			openCount++;
		}
		else if (data[close] == '}')
		{
			openCount--;
		}

		if (openCount == 0)
		{
			return close;
		}

		++close;
	}

	return std::string::npos;
}

size_t ImpStringHelper::FindOpeningBrace(const std::string& data, size_t begin)
{
	return data.find('{', begin);
}

size_t ImpStringHelper::FindVectorIndex(const std::string& data, size_t begin)
{
	size_t closeBrace = data.find('}', begin);
	
	size_t open = data.find('[', begin);
	size_t close = data.find(']', begin);

	if (closeBrace <= open)
	{
		return std::string::npos;
	}

	std::string strIndex = data.substr(open + 1, close - open - 1);
	size_t index = std::stoull(strIndex);

	return index;
}

std::string ImpStringHelper::EraseNamespace(const std::string& name)
{
	size_t spaceBegin = name.find_last_of(":", name.size());

	if (spaceBegin == std::string::npos)
	{
		return name;
	}

	return name.substr(spaceBegin + 1, name.size() - spaceBegin);
}

std::string ImpStringHelper::GetTypeName(const std::string& data, size_t begin)
{
	size_t openIndex = FindOpeningBrace(data, begin);

	/// { 를 찾지 못핫경우 
	if (openIndex == std::string::npos)
	{
		return  data.substr(begin);
	}
	
	return data.substr(begin, openIndex - begin );
}

std::string ImpStringHelper::GetKeySubstr(const std::string& data, size_t begin)
{
	// [string{22}] -> string{22}
	size_t open = data.find('{', begin);
	size_t close = data.find('}', begin);

	if (open == std::string::npos || open >= close)
	{
		return ImpStringHelper::empty;
	}

	return data.substr(open, close - open);
}

void ImpStringHelper::RemoveSubstring(std::string& main, const std::string eraseStr)
{
	size_t pos = main.find(eraseStr);
	if (pos != std::string::npos)
	{
		main.erase(pos, eraseStr.length());
	}
}

std::string ImpStringHelper::EraseUnderbar(const std::string& name)
{
	if (name[0] == '_')
	{
		return name.substr(1, name.size() - 1);
	}

	return name;
}
