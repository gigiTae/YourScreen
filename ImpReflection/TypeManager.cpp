#include "ImpReflectionPCH.h"
#include "TypeManager.h"

std::type_index ImpReflection::TypeManager::GetTypeIndex(const std::string& name) const
{
	auto iter = _infomations.find(name);
	
	if (iter == _infomations.end())
	{
		return std::type_index(typeid(Exception));
	}

	return iter->second.index;
}

ImpReflection::Type* ImpReflection::TypeManager::GetType(const std::string& name) const
{
	auto iter = _infomations.find(name);

	if (iter == _infomations.end())
	{
		return nullptr;
	}

	return iter->second.type;
}

const std::string& ImpReflection::TypeManager::GetName(std::type_index index) const
{
	for (auto& iter : _infomations)
	{
		if (iter.second.index == index)
		{
			return iter.first;
		}
	}

	return ImpStringHelper::empty;
}

void ImpReflection::TypeManager::AddTypeInfomation(const std::string& name, const TypeInfomation& info)
{
	auto iter = _infomations.find(name);

	// 이미 추가한 타입을 추가함
	if (iter != _infomations.end())
	{
		assert(!"추가한 타입중복으로 추가합니다. 확인이 필요합니다");
		return;
	}

	_infomations.insert({ name,info });
}

ImpReflection::Type* ImpReflection::TypeManager::GetType(std::type_index index) const
{
	for (auto& iter : _infomations)
	{
		if (iter.second.index == index)
		{
			return iter.second.type;
		}
	}

	return nullptr;
}
