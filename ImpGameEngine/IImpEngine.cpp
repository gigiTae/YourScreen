#include "ImpGameEnginePCH.h"
#include "IImpEngine.h"
#include "ImpGameEngine.h"
#include "ImpToolEngine.h"

IMP_ENGNIE_API ImpGameEngine::IImpEngine* ImpGameEngine::EngineExporter::GetGameEngine()
{
    _engine = new ImpGameEngine();
    return _engine;
}

IMP_ENGNIE_API ImpGameEngine::IImpEngine* ImpGameEngine::EngineExporter::GetToolEngine()
{
    _engine = new ImpToolEngine();
    return _engine;
}

IMP_ENGNIE_API void ImpGameEngine::EngineExporter::DeleteEngine()
{
    delete _engine;
}

ImpGameEngine::IImpEngine* ImpGameEngine::EngineExporter::_engine = nullptr;
