#pragma once

namespace ImpClient
{
/// <summary>
/// 게임시작하자마 BGM을 반복재생한다.
/// </summary>
class SoundStore: public ImpEngineModule::Component
{
public:
    SoundStore();
    ~SoundStore();

    Component* Clone(Component* clone = nullptr) const override;

    void Start() override;
    virtual void Update(float dt) override;

public:
    std::wstring _soundPath;
    std::wstring _soundPath2;

    float _time;
    bool _isSetSoundPath2;

    REFLECTION_FRIEND(SoundStore);
};

} // namespace ImpClient