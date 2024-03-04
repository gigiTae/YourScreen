#pragma once

#include "ExportDefine.h"
#include "WindowInfomation.h"

namespace ImpGameEngine
{
/// ���ӿ����� �������̽�
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
    // ���ӿ��� ����
    static IMP_ENGNIE_API IImpEngine* GetGameEngine();

    // Tool���� ����
    static IMP_ENGNIE_API IImpEngine* GetToolEngine();

    // ���� �Ҹ�
    static IMP_ENGNIE_API void DeleteEngine();

private:
    static IImpEngine* _engine;
};

} // namespace ImpGameEngine
