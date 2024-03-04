#include "ImpToolModulePCH.h"
#include "Sphere.h"


ImpToolModule::Debug::Sphere::Sphere(ImpGraphics::IImpGraphicsEngine* graphics, size_t id, ImpMath::Vector3 center, float radius)
	:DebugObject(graphics,id), _radius(radius), _center(center)
{
	_graphicsEngine->AddSphereBoundingVolume(_id, _center, _radius);
}

ImpToolModule::Debug::Sphere::~Sphere()
{
	_graphicsEngine->DeleteBoundingVolume(_id);
}

