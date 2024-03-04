#include "EngineModulePCH.h"
#include "SoundManager.h"
#include "PathManager.h"
#include "WorldManager.h"

ImpEngineModule::SoundManager::SoundManager()
    : _fmodResult()
    , _fmodSystem(nullptr)
    , _channel{}
    , _version{}
    , _soundList{}
{
}

ImpEngineModule::SoundManager::~SoundManager()
{
}

void ImpEngineModule::SoundManager::Initialize()
{
    _fmodResult = FMOD::System_Create(&_fmodSystem);
    assert(_fmodResult == FMOD_OK);

    _fmodResult = _fmodSystem->getVersion(&_version);
    assert(_fmodResult == FMOD_OK);

    _fmodResult = _fmodSystem->init(maxChannel, FMOD_INIT_NORMAL, nullptr);
    assert(_fmodResult == FMOD_OK);
}

void ImpEngineModule::SoundManager::Update()
{
    _fmodSystem->update();
}

void ImpEngineModule::SoundManager::UnloadAllSound()
{
    for (unsigned int i = 0; i < maxChannel; ++i)
    {
        StopChannel(i);
    }

    for (auto& sound : _soundList)
    {
        sound.second->release();
    }

    _soundList.clear();
}

void ImpEngineModule::SoundManager::LoadSound(const std::wstring& path)
{
    FMOD::Sound* sound = nullptr;

    // wstring->string 불안하다.
    std::filesystem::path filePath(path);
    std::string sPath = filePath.string();

    _fmodResult = _fmodSystem->createSound(sPath.c_str(), FMOD_DEFAULT, 0, &sound);

    if (_fmodResult != FMOD_OK)
    {
        sound->release();
        return;
    }

    _soundList.insert({ path, sound });
}


void ImpEngineModule::SoundManager::LoadSoundList(const std::wstring& worldName)
{
    std::wstring path = PathManager::GetWorldPath(worldName);

    path += L"/SoundList.txt";

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

            LoadSound(line);
        }

        entityList.close();
    }
    else
    {
        assert(!"파일오픈 실패");
    }
}

void ImpEngineModule::SoundManager::PlaySound(const std::wstring& path,
    bool isLoop,
    unsigned int channelIndex)
{
    auto iter = _soundList.find(path);

    if (iter == _soundList.end())
    {
        return;
    }

    _fmodResult = _fmodSystem->playSound(
        iter->second, nullptr, false, &_channel[channelIndex]);

    if (_fmodResult == FMOD_OK)
    {
        if (isLoop)
        {
            _channel[channelIndex]->setMode(FMOD_LOOP_NORMAL);
        }
        else
        {
            _channel[channelIndex]->setMode(FMOD_LOOP_OFF);
        }
    }
}

bool ImpEngineModule::SoundManager::isPlaying(size_t index)
{
    bool temp;

    _channel[index]->isPlaying(&temp);

    return temp;
}

void ImpEngineModule::SoundManager::StopChannel(unsigned int channnelIndex)
{
    if (_channel[channnelIndex])
    {
        _channel[channnelIndex]->stop();
    }
}
void ImpEngineModule::SoundManager::Finalize()
{
    UnloadAllSound();

    _fmodSystem->close();
    _fmodSystem->release();
}
