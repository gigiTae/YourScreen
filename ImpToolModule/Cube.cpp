#include "ImpToolModulePCH.h"
#include "Cube.h"

ImpToolModule::Debug::Cube::Cube(ImpGraphics::IImpGraphicsEngine* graphics, ImpEngineModule::Entity* ent)
	:DebugObject(graphics,ent->GetID())
{
	_ent = ent;

	_transform = _ent->GetComponent<ImpEngineModule::Transform>();
	_boxCollider = _ent->GetComponent<ImpEngineModule::BoxCollider>();

	graphics->AddCubeBoundingVolume(_id, Vector3(0.5f, 0.5f, 0.5f));
	//graphics->AddCubeBoundingVolume(_id + 5000000, Vector3(1.f, 1.f, 1.f));
}


void ImpToolModule::Debug::Cube::Update()
{
	Matrix transitionMat = Matrix::MakeTranslationMatrix(_boxCollider->GetCenterPosition());
	Matrix scaleMat = Matrix::MakeScaleMatrix(_boxCollider->GetSize());

	Matrix boxMatix = scaleMat *transitionMat;

	if (_boxCollider->IsRotatable())
	{
		Matrix rotM = Matrix::MakeFromQuaternion(_transform->GetRotation());
		boxMatix = scaleMat * rotM * transitionMat;
	}

	if (_boxCollider->GetSize().x < 0 
		|| _boxCollider->GetSize().z < 0 
		|| _boxCollider->GetSize().y < 0)
	{
		const ImpMath::Vector4 gray(0.3f, 0.3f, 0.3f, 1.f);
		_graphicsEngine->UpdateBoundingVolume(_id, boxMatix, gray);
	}
	else if (_boxCollider->GetCurrentCollisionCollider() == 0)
	{
		const ImpMath::Vector4 green(0.f, 1.f, 0.f, 1.f);
		_graphicsEngine->UpdateBoundingVolume(_id, boxMatix, green);
	}
	else
	{
		const ImpMath::Vector4 red(1.f, 0.f, 0.f, 1.f);
		_graphicsEngine->UpdateBoundingVolume(_id, boxMatix, red);
	}

	//Vector3 min, max;

	//_boxCollider->GetMinMaxPoint(min, max);

	//Vector3 center = (max + min) * 0.5f;
	//Matrix boundingTMat = Matrix::MakeTranslationMatrix(center);
	//Matrix boundingSMat = Matrix::MakeScaleMatrix(Vector3(max - min));

	//Matrix boundingMat = boundingSMat * boundingTMat;

	//_graphicsEngine->UpdateBoundingVolume(_id + 5000000, boundingMat, Vector4(0.f, 0.f, 1.f));

}


ImpToolModule::Debug::Cube::~Cube()
{
	_graphicsEngine->DeleteBoundingVolume(_id);

	// B
	//_graphicsEngine->DeleteBoundingVolume(_id + 5000000);
}
