#include "pch.h"
#include "Factory.h"

#include "ObjectManager.h"
#include "ResourceManager.h"

#include "Builder.h"

using namespace ImpGraphics;

Factory::Factory(ResourceManager* resourceManager)
	:_resourceManager(resourceManager)
{
	_objectManager = new ObjectManager;
	_builder = new Builder(resourceManager);
}

Factory::~Factory()
{
	delete _builder;
	delete _objectManager;
}
