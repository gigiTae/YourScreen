#include "ImpToolModulePCH.h"
#include "DebugObject.h"


ImpToolModule::Debug::DebugObject::DebugObject(ImpGraphics::IImpGraphicsEngine* graphics, size_t id)
	:_graphicsEngine(graphics),_id(id)
{}

