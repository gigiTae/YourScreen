#include "EngineModulePCH.h"
#include "TagMapResource.h"

ImpEngineModule::TagMapResource::TagMapResource(const std::wstring& path)
	:Resource(ResourceCategory::TagMap,path)
{

}

ImpEngineModule::TagMapResource::~TagMapResource()
{

}

void ImpEngineModule::TagMapResource::Load()
{
	const std::wstring& path = GetPath();

	std::ifstream readData(GetPath());

	if (readData.is_open())
	{
		std::string line{};

		size_t index = -1;


		while (std::getline(readData, line))
		{
			++index;
			assert(index <= 31);


			_tagMap.tagCheck[index] = std::bitset<32>(line);
		}

		readData.close();
	}
}
