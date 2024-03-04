#include "EngineModulePCH.h"
#include "Sound.h"

ImpEngineModule::Sound::Sound(const std::wstring& path)
    : Resource(ResourceCategory::Sound, path)
{
}


ImpEngineModule::Sound::~Sound()
{
}

void ImpEngineModule::Sound::Load()
{
}
