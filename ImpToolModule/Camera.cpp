#include "ImpToolModulePCH.h"
#include "Camera.h"

ImpToolModule::Debug::Camera::Camera(ImpGraphics::IImpGraphicsEngine* graphics, ImpEngineModule::Entity* ent)
	:DebugObject(graphics, ent->GetID())
{
	_ent = ent;
	_camera = ent->GetComponent<ImpEngineModule::Camera>();
	_transform = ent->GetComponent<ImpEngineModule::Transform>();

	_graphicsEngine->AddCubeBoundingVolume(cameraID1 + _id, Vector3(0.2f, 0.2f, 1.f));

}

ImpToolModule::Debug::Camera::~Camera()
{
	_graphicsEngine->DeleteBoundingVolume(cameraID1 + _id);
}

void ImpToolModule::Debug::Camera::Update()
{
	_graphicsEngine->UpdateBoundingVolume(cameraID1 + _id, _transform->GetWorldMatrix(), Vector4(0.f, 0.f, 0.f, 1.f));
}


