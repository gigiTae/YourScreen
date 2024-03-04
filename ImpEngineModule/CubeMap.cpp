#include "EngineModulePCH.h"
#include "CubeMap.h"



ImpEngineModule::CubeMap::CubeMap(const std::wstring& path)
:Resource(ResourceCategory::CubeMap,path)
{

}

ImpEngineModule::CubeMap::~CubeMap()
{

}

void ImpEngineModule::CubeMap::Load()
{
	//아무것도 안한다
}
