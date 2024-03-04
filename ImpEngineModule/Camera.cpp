#include "EngineModulePCH.h"
#include "Camera.h"
#include "World.h"
#include "InputManager.h"
#include "Transform.h"

#define IMP_EDITOR_MODE

ImpEngineModule::Camera::Camera()
{

}

ImpEngineModule::Camera::~Camera()
{

}


ImpEngineModule::Component* ImpEngineModule::Camera::Clone(Component* clone /*= nullptr*/) const
{
	Camera* camera = static_cast<Camera*>(clone);

	if (camera == nullptr)
	{
		camera = new Camera(*this);
	}
	else
	{
		*camera = *this;
	}

	return camera;
}


void ImpEngineModule::Camera::OnMainCamera()
{
	World* world = GetWorld();

	// 다른 메인 카메라가 있다면 끈다.
	world->Each<ImpEngineModule::Camera>([](Entity* ent, ComponentHandle<Camera> camera)
		{
			camera->OffMainCamera();
		}, false);

	_isMain = true;
}

void ImpEngineModule::Camera::OnSecondCameara()
{
    World* world = GetWorld();

    // 다른 메인 카메라가 있다면 끈다.
    world->Each<ImpEngineModule::Camera>([](Entity* ent, ComponentHandle<Camera> camera)
                                         { camera->OffSecondCamera(); },
                                         false);

    _isSecond = true;
}

void ImpEngineModule::Camera::Strafe(float distance)
{
	//mPosition = XMFLOAT3(mRight.x * d + mPosition.x, mRight.y * d + mPosition.y, mRight.z * d + mPosition.z);
	Transform* transform = GetComponent<Transform>();

	ImpMath::Vector3 position = transform->GetLocalPosition();
	ImpMath::Quaternion rotation = transform->GetRotation();
	auto rotMatrix = ImpMath::Matrix::MakeFromQuaternion(rotation);

	transform->SetPosition(ImpMath::Vector3(rotMatrix._11 * distance + position.x,
		rotMatrix._12 * distance + position.y, rotMatrix._13 * distance + position.z));
}

void ImpEngineModule::Camera::Walk(float distance)
{
	//mPosition = XMFLOAT3(mLook.x * d + mPosition.x, mLook.y * d + mPosition.y, mLook.z * d + mPosition.z);
	Transform* transform = GetComponent<Transform>();

	ImpMath::Vector3 position = transform->GetLocalPosition();
	ImpMath::Quaternion rotation = transform->GetRotation();
	auto rotMatrix = ImpMath::Matrix::MakeFromQuaternion(rotation);

	transform->SetPosition(ImpMath::Vector3(rotMatrix._31 * distance + position.x,
		rotMatrix._32 * distance + position.y, rotMatrix._33 * distance + position.z));
}

void ImpEngineModule::Camera::WorldUpDown(float distance)
{
	Transform* transform = GetComponent<Transform>();

	ImpMath::Vector3 position = transform->GetLocalPosition();
	ImpMath::Quaternion rotation = transform->GetRotation();
	auto rotMatrix = ImpMath::Matrix::MakeFromQuaternion(rotation);

	transform->SetPosition(ImpMath::Vector3(rotMatrix._21 * distance + position.x,
		rotMatrix._22 * distance + position.y, rotMatrix._23 * distance + position.z));
}

void ImpEngineModule::Camera::Yaw(float degree)
{
	Transform* transform = GetComponent<Transform>();
	transform->AddYawRotation(ImpMath::DegreeToRadian(degree));
}

void ImpEngineModule::Camera::Pitch(float degree)
{
	Transform* transform = GetComponent<Transform>();
	transform->AddPitchRotation(ImpMath::DegreeToRadian(degree));
}

void ImpEngineModule::Camera::Roll(float degree)
{
	Transform* transform = GetComponent<Transform>();
	transform->AddRollRotation(ImpMath::DegreeToRadian(degree));
}

