#include "ImpToolModulePCH.h"
#include "CapsuleCollider.h"

ImpToolModule::Debug::CapsuleCollider::CapsuleCollider(
    ImpGraphics::IImpGraphicsEngine* graphics,
    ImpEngineModule::Entity* ent)
    : DebugObject(graphics, ent->GetID())
    , _ent(ent)
{
    _transform = ent->GetComponent<ImpEngineModule::Transform>();
    _capsule   = ent->GetComponent<ImpEngineModule::CapsuleCollider>();

    _graphicsEngine->AddSphereBoundingVolume(capsuleID1 + _id, 1.f);
    _graphicsEngine->AddSphereBoundingVolume(capsuleID2 + _id, 1.f);
    _graphicsEngine->AddCubeBoundingVolume(capsuleID3 + _id, Vector3(1.f, 1.f, 1.f));
}

ImpToolModule::Debug::CapsuleCollider::~CapsuleCollider()
{
    _graphicsEngine->DeleteBoundingVolume(_id + capsuleID1);
    _graphicsEngine->DeleteBoundingVolume(_id + capsuleID2);
    _graphicsEngine->DeleteBoundingVolume(_id + capsuleID3);
}

void ImpToolModule::Debug::CapsuleCollider::Update()
{
    Matrix boxScaleM = Matrix::MakeScaleMatrix(
        Vector3(_capsule->GetRadius() / std::sqrt(2.f),
                _capsule->GetHeight(),
                _capsule->GetRadius() / std::sqrt(2.f)));

    Matrix sM = Matrix::MakeScaleMatrix(
        Vector3(_capsule->GetRadius(), _capsule->GetRadius(), _capsule->GetRadius()));

    Matrix rotM = (_capsule->IsRotatable())
                      ? Matrix::MakeFromQuaternion(_capsule->GetRotation())
                      : Matrix::Identity();

    float hegiht = _capsule->GetHeight();

    Vector3 up(0.f, hegiht, 0.f);
    Vector3 down(0.f, -hegiht, 0.f);
    Vector3 capsuleCenter = _capsule->GetCenterPosition();

    up   = up.Transform(rotM) + capsuleCenter;
    down = down.Transform(rotM) + capsuleCenter;

    Matrix upTM   = sM * Matrix::MakeTranslationMatrix(up);
    Matrix downTM = sM * Matrix::MakeTranslationMatrix(down);
    Matrix boxTM = boxScaleM * rotM * Matrix::MakeTranslationMatrix(capsuleCenter);

    if (_capsule->GetRadius() < 0.f || _capsule->GetHeight() < 0.f)
    {
        const ImpMath::Vector4 gray(0.3f, 0.3f, 0.3f, 1.f);
        _graphicsEngine->UpdateBoundingVolume(_id + capsuleID1, upTM, gray);
        _graphicsEngine->UpdateBoundingVolume(_id + capsuleID2, downTM, gray);
        _graphicsEngine->UpdateBoundingVolume(_id + capsuleID3, boxTM, gray);
    }
    else if (_capsule->GetCurrentCollisionCollider() == 0)
    {
        const ImpMath::Vector4 green(0.f, 1.f, 0.f, 1.f);
        _graphicsEngine->UpdateBoundingVolume(_id + capsuleID1, upTM, green);
        _graphicsEngine->UpdateBoundingVolume(_id + capsuleID2, downTM, green);
        _graphicsEngine->UpdateBoundingVolume(_id + capsuleID3, boxTM, green);
    }
    else
    {
        const ImpMath::Vector4 red(1.f, 0.f, 0.f, 1.f);
        _graphicsEngine->UpdateBoundingVolume(_id + capsuleID1, upTM, red);
        _graphicsEngine->UpdateBoundingVolume(_id + capsuleID2, downTM, red);
        _graphicsEngine->UpdateBoundingVolume(_id + capsuleID3, boxTM, red);
    }
}
