#include "EngineModulePCH.h"
#include "EntityView.h"

ImpEngineModule::Internal::EntityView::EntityView(const EntityIterator& first, const EntityIterator& last)
	:_firstIterator(first), _lastIterator(last)
{

}
