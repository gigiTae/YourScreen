#include "pch.h"

#include "ImpGraphicsEngine.h"

namespace ImpGraphics
{
	IImpGraphicsEngine* EngineExporter::GetEngine()
	{
		if (_engine == nullptr)
		{
			_engine = new ImpGraphicsEngine;
		}
		return _engine;
	}

	void EngineExporter::DeleteEngine()
	{
		delete _engine;
	}

	ImpGraphics::IImpGraphicsEngine* EngineExporter::_engine = nullptr;
}