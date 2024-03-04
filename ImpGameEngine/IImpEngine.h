#pragma once

#include "ExportDefine.h"
#include "WindowInfomation.h"

namespace ImpGameEngine
{
/// 게임엔진의 인터페이스
class IImpEngine
{
public:
    virtual ~IImpEngine()
    {
    }

    virtual IMP_ENGNIE_API void Initialize(WindowInfomation info) abstract;

    virtual IMP_ENGNIE_API void Process() abstract;

    virtual IMP_ENGNIE_API void Finalize() abstract;
};

class EngineExporter
{
public:
    // 게임엔진 생성
    static IMP_ENGNIE_API IImpEngine* GetGameEngine();

    // Tool엔진 생성
    static IMP_ENGNIE_API IImpEngine* GetToolEngine();

    // 엔진 소멸
    static IMP_ENGNIE_API void DeleteEngine();

private:
    static IImpEngine* _engine;
};

} // namespace ImpGameEngine
