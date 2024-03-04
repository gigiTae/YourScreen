#pragma once
#include "Resource.h"

namespace ImpEngineModule
{
/// <summary>
/// 사운드 리소스
/// </summary>
class Sound: public Resource
{
public:
    Sound(const std::wstring& path);
    ~Sound();

    // 사운드 로드
    void Load() override;

};
} // namespace ImpEngineModule
