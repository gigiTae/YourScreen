#include "EngineModulePCH.h"
#include "AudioClip.h"
#include "SoundManager.h"

ImpEngineModule::AudioClip::AudioClip()
{
}

ImpEngineModule::AudioClip::~AudioClip()
{
}

ImpEngineModule::Component* ImpEngineModule::AudioClip::Clone(
    Component* clone /*= nullptr*/) const
{
    AudioClip* audioClip = static_cast<AudioClip*>(clone);

    if (audioClip == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        audioClip = new AudioClip(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *audioClip = *this;
    }

    return audioClip;
}

void ImpEngineModule::AudioClip::Start()
{
    for (const auto& soundPath : _sounds)
    {
        SoundManager::GetInstance()->LoadSound(soundPath);
    }
}

const std::vector<std::wstring>& ImpEngineModule::AudioClip::GetSoundList() const
{
    return _sounds;
}

void ImpEngineModule::AudioClip::PlaySound(const std::wstring& sound,
    bool isLoof,
    unsigned int channel)
{
    if (!HasSound(sound)) return;

    SoundManager::GetInstance()->PlaySound(sound, isLoof, channel);
}

void ImpEngineModule::AudioClip::PlaySound(size_t index,
    bool isLoof,
    unsigned int channel)
{
    PlaySound(_sounds[index], isLoof, channel);
}

size_t ImpEngineModule::AudioClip::GetAudioIndex(const std::wstring& name) const
{
    auto isSameName = [name](const std::wstring& path) 
        {  
            size_t lastPoint = path.find_last_of(L".");
            size_t lastSlash = path.find_last_of(L"\\");

            auto fileName = path.substr(lastSlash + 1, lastPoint - lastSlash - 1);
        if (fileName == name) return true;
        return false;
        };
    auto soundIndex = std::find_if(_sounds.begin(), _sounds.end(), isSameName);
    return soundIndex - _sounds.begin();
}

bool ImpEngineModule::AudioClip::HasSound(const std::wstring& sound) const
{
    for (const auto& inSound : _sounds)
    {
        if (inSound == sound)
        {
            return true;
        }
    }

    return false;
}
