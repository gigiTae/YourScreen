#include "EngineModulePCH.h"
#include "ResourceManager.h"
#include "EntityResource.h"
#include "PathManager.h"
#include "CubeMap.h"
#include "EventManager.h"
#include "CallBackEvent.h"
#include "TagMapResource.h"

ImpEngineModule::ResourceManager::ResourceManager()
	:_entityResources{}, _worldDataList{}, _eventManager(nullptr),
	_cubeMap(nullptr), _tagMapResource(nullptr)
{

}

ImpEngineModule::ResourceManager::~ResourceManager()
{

}

void ImpEngineModule::ResourceManager::Initailize(const std::wstring startWorld, EventManager* eventManager)
{
	/// 월드Data 내부의 월드들 경로를 담아온다.
	_worldDataList = PathManager::GetWorldDataList();

	_eventManager = eventManager;
}

void ImpEngineModule::ResourceManager::Finalize()
{
	UnloadAllEntityResources();

	if (_cubeMap != nullptr)
	{
		delete _cubeMap;
	}

	if (_tagMapResource != nullptr)
	{
		delete _tagMapResource;
	}
}

ImpEngineModule::EntityResource* ImpEngineModule::ResourceManager::GetEntityResource(const std::wstring& path)
{
	auto iter = _entityResources.find(path);

	if (iter == _entityResources.end())
	{
		return nullptr;
	}

	return iter->second.get();
}

void ImpEngineModule::ResourceManager::LoadResource(const std::wstring& path)
{


}

void ImpEngineModule::ResourceManager::LoadEntityList(const std::wstring& path)
{
	// 1. txt 파일을 읽는다.
	std::wifstream entityList(path);

	if (entityList.is_open())
	{
		std::wstring line;
		std::wstring relativePath = PathManager::GetResourcesPath();

		while (std::getline(entityList, line))
		{
			// 2. EntityResource들을 상대경로를 더해서 로드한다. 
			line = relativePath + line;

			LoadEntityResource(line);
		}

		entityList.close();
	}
	else
	{
		assert(!"파일오픈 실패");
	}
}

void ImpEngineModule::ResourceManager::LoadEntityResource(const std::wstring& path)
{
	_entityResources.insert({path,std::make_unique<EntityResource>(path)});
	_entityResources[path]->Load();
}

void ImpEngineModule::ResourceManager::UnloadAllEntityResources()
{
	_entityResources.clear();
}

void ImpEngineModule::ResourceManager::LoadCubeMap(const std::wstring& path)
{
	if (_cubeMap != nullptr)
	{
		delete _cubeMap;
	}
	_cubeMap = new CubeMap(path);
	_cubeMap->Load();
	_eventManager->Emit<Event::LoadCubeMap>({ path });
}

void ImpEngineModule::ResourceManager::LoadTagMapResource(const std::wstring& path)
{
	if (_tagMapResource != nullptr)
	{
		delete _tagMapResource;
	}

	_tagMapResource = new TagMapResource(path);
	_tagMapResource->Load();
}

const std::unordered_map<std::wstring, std::unique_ptr<ImpEngineModule::EntityResource>>&
    ImpEngineModule::ResourceManager::GetEntityResources() const
{
    return _entityResources;
}
