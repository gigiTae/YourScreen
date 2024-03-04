#pragma once


namespace ImpEngineModule
{
/// <summary>
///  ���带 �����ϴ� �Ŵ���
/// </summary>
class SoundManager
{

public:
    static constexpr unsigned int maxChannel = 32;

    // �̱���
    static SoundManager* GetInstance()
    {
        static SoundManager mgr;
        return &mgr;
    }

public:
    void Initialize();

    void Update();

    void UnloadAllSound();

    void LoadSound(const std::wstring& path);

    void LoadSoundList(const std::wstring& worldName);

    void PlaySound(const std::wstring& path, bool isLoop, unsigned int channelndex);

    bool isPlaying(size_t index);
    void StopChannel(unsigned int channnelIndex);

    void Finalize();

private:
    SoundManager();
    ~SoundManager();

private:
    FMOD_RESULT _fmodResult;
    FMOD::System* _fmodSystem;
    std::unordered_map<std::wstring, FMOD::Sound*> _soundList;
    unsigned int _version;
    FMOD::Channel* _channel[maxChannel];
};

} // namespace ImpEngineModule