#include "EngineModulePCH.h"
#include "Resource.h"


ImpEngineModule::Resource::Resource(ResourceCategory category, const std::wstring& path)
	:_path(path), _category(category)
{}
