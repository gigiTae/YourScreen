#include "EngineModulePCH.h"
#include "TagMap.h"

void ImpEngineModule::Physics::TagMap::Reset()
{
	for (size_t i = 0; i < static_cast<size_t>(Tag::END); ++i)
	{
		tagCheck[i].reset();
	}
}
