#pragma once
#include "Resource.h"

namespace ImpEngineModule
{
/// <summary>
/// ���� ���ҽ�
/// </summary>
class Sound: public Resource
{
public:
    Sound(const std::wstring& path);
    ~Sound();

    // ���� �ε�
    void Load() override;

};
} // namespace ImpEngineModule
