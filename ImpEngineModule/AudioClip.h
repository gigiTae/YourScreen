#pragma once
#include "Component.h"

namespace ImpEngineModule
{
/// <summary>
/// 소리 컴포넌트
/// </summary>
class AudioClip: public Component
{
public:
    AudioClip();
    ~AudioClip();

    Component* Clone(Component* clone = nullptr) const override;

    virtual void Start() override;

    const std::vector<std::wstring>& GetSoundList() const;

    void PlaySound(size_t index, bool isLoof, unsigned int channel);
    void PlaySound(const std::wstring& sound, bool isLoof, unsigned int channel);

    size_t GetAudioIndex(const std::wstring& name) const;

    bool HasSound(const std::wstring& sound) const;

private:
    std::vector<std::wstring> _sounds;

    REFLECTION_FRIEND(AudioClip);
};
} // namespace ImpEngineModule
