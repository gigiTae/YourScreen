#include "ImpToolModulePCH.h"
#include "SphereCollider.h"


ImpToolModule::Debug::SphereCollider::SphereCollider(
    ImpGraphics::IImpGraphicsEngine* graphics,
    ImpEngineModule::Entity* ent)
    : DebugObject(graphics, ent->GetID())
    , _ent(ent)
{
    _transform = ent->GetComponent<ImpEngineModule::Transform>();
    _sphere    = ent->GetComponent<ImpEngineModule::SphereCollider>();
    _radius    = _sphere->GetRadius();

    _graphicsEngine->AddSphereBoundingVolume( sphereID1 +_id, 1.f);
}

void ImpToolModule::Debug::SphereCollider::Update()
{
    Matrix tM = Matrix::MakeTranslationMatrix(_sphere->GetCenterPosition());
    Matrix sM = Matrix::MakeScaleMatrix(
        Vector3(_sphere->GetRadius(), _sphere->GetRadius(), _sphere->GetRadius()));

    tM = sM * tM;

    if (_sphere->GetRadius() < 0)
    {
        const ImpMath::Vector4 gray(0.3f, 0.3f, 0.3f, 1.f);
        _graphicsEngine->UpdateBoundingVolume(sphereID1 + _id, tM, gray);
    }
    else if (_sphere->GetCurrentCollisionCollider() == 0)
    {
        const ImpMath::Vector4 green(0.f, 1.f, 0.f, 1.f);
        _graphicsEngine->UpdateBoundingVolume(sphereID1 + _id, tM, green);
    }
    else
    {
        const ImpMath::Vector4 red(1.f, 0.f, 0.f, 1.f);
        _graphicsEngine->UpdateBoundingVolume(sphereID1 + _id, tM, red);
    }
}

ImpToolModule::Debug::SphereCollider::~SphereCollider()
{
    _graphicsEngine->DeleteBoundingVolume(sphereID1 + _id);
}
