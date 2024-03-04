#include "GUIManager.h"

ImpGraphics::IImpGraphicsTool* ImpGraphics::ToolExporter::GetEngine()
{
	_tool = new GUIManager;
}

void ImpGraphics::ToolExporter::DeleteEngine()
{
	delete _tool;
}