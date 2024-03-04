#include "ImpToolModulePCH.h"
#include "Light.h"

ImpToolModule::Debug::Light::Light(ImpGraphics::IImpGraphicsEngine* graphics,
                                   ImpEngineModule::Entity* ent)
    : DebugObject(graphics, ent->GetID())
    , _ent(nullptr)
{
    _transform        = ent->GetComponent<ImpEngineModule::Transform>();
    _light            = ent->GetComponent<ImpEngineModule::Light>();
    _currentLightType = _light->GetLightType();

    if (_currentLightType == ImpEngineModule::Light::LightType::Point)
    {
        _graphicsEngine->AddSphereBoundingVolume(lightID1 + _id, 1.f);
        _graphicsEngine->AddSphereBoundingVolume(lightID2 + _id, 1.f);
    }
    else if (_currentLightType == ImpEngineModule::Light::LightType::Directional)
    {
        _graphicsEngine->AddSphereBoundingVolume(lightID1 + _id, 1.f);
        _graphicsEngine->AddCubeBoundingVolume(lightID2 + _id,
                                               Vector3(1.f, 1.f, 1.f));

        //		_graphicsEngine->AddCubeBoundingVolume(lightID2 + _id,Vector3(0.1f,0.1f,2.f));
    }
    else if (_currentLightType == ImpEngineModule::Light::LightType::Spot)
    {

        _graphicsEngine->AddSphereBoundingVolume(lightID1 + _id, 1.f);
        _graphicsEngine->AddCubeBoundingVolume(lightID2 + _id,
                                               Vector3(1.f, 1.f, 1.f));
    }
}

void ImpToolModule::Debug::Light::Update()
{
    // 라이트 타입이 변경된 경우
    if (_currentLightType != _light->GetLightType())
    {
    }
    auto color  = _light->GetColor();
    Vector3 pos = _transform->GetWorldPostiion();

    static const Matrix center = Matrix::MakeScaleMatrix(
        Vector3(0.05f, 0.05f, 0.05f));
    static const Matrix zAxis = Matrix::MakeScaleMatrix(Vector3(0.1f, 0.1f, 2.f));

    // PointLight인 경우
    if (_currentLightType == ImpEngineModule::Light::LightType::Point)
    {
        Matrix tM   = Matrix::MakeTranslationMatrix(pos);
        float range = _light->GetRange();

        Matrix sM = Matrix::MakeScaleMatrix(Vector3(range, range, range));

        Matrix lightCenterM = center * tM;
        Matrix lightRangeM  = sM * tM;

        _graphicsEngine->UpdateBoundingVolume(lightID1 + _id,
                                              lightCenterM,
                                              Vector4(color.r, color.g, color.b, 1.f));
        _graphicsEngine->UpdateBoundingVolume(lightID2 + _id,
                                              lightRangeM,
                                              Vector4(color.r, color.g, color.b, 1.f));
    }
    else if (_currentLightType == ImpEngineModule::Light::LightType::Directional)
    {
        Matrix rotationM = Matrix::MakeFromQuaternion(_transform->GetRotation());

        Vector3 lightDirPos(rotationM._31 * 1.f,
                            rotationM._32 * 1.f,
                            rotationM._33 * 1.f);

        Matrix lightDirTransformM = Matrix::MakeTranslationMatrix(pos + lightDirPos);
        Matrix tM = Matrix::MakeTranslationMatrix(pos);

        Matrix lightDir = zAxis * rotationM * lightDirTransformM;

        Matrix lightCenterM = center * tM;

        _graphicsEngine->UpdateBoundingVolume(lightID1 + _id,
                                              lightCenterM,
                                              Vector4(color.r, color.g, color.b, 1.f));
        _graphicsEngine->UpdateBoundingVolume(lightID2 + _id,
                                              lightDir,
                                              Vector4(color.r, color.g, color.b, 1.f));
    }
    else
    {
        Matrix rotationM = Matrix::MakeFromQuaternion(_transform->GetRotation());
        float range = _light->GetRange();

        Matrix sM = Matrix::MakeScaleMatrix(Vector3(range, range, range));

        Vector3 lightDirPos(rotationM._31 * 1.f,
                            rotationM._32 * 1.f,
                            rotationM._33 * 1.f);

        Matrix lightDirTransformM = Matrix::MakeTranslationMatrix(pos + lightDirPos);
        Matrix tM = Matrix::MakeTranslationMatrix(pos);

        Matrix lightDir = zAxis * rotationM * lightDirTransformM;

        Matrix lightCenterM = sM* tM;

        _graphicsEngine->UpdateBoundingVolume(lightID1 + _id,
                                              lightCenterM,
                                              Vector4(color.r, color.g, color.b, 1.f));
        _graphicsEngine->UpdateBoundingVolume(lightID2 + _id,
                                              lightDir,
                                              Vector4(color.r, color.g, color.b, 1.f));
    }
}

ImpToolModule::Debug::Light::~Light()
{
    if (_currentLightType == ImpEngineModule::Light::LightType::Point)
    {
        _graphicsEngine->DeleteBoundingVolume(lightID1 + _id);
        _graphicsEngine->DeleteBoundingVolume(lightID2 + _id);
    }
    else if (_currentLightType == ImpEngineModule::Light::LightType::Directional)
    {
        _graphicsEngine->DeleteBoundingVolume(lightID1 + _id);
        _graphicsEngine->DeleteBoundingVolume(lightID2 + _id);
    }
    else
    {
        _graphicsEngine->DeleteBoundingVolume(lightID1 + _id);
        _graphicsEngine->DeleteBoundingVolume(lightID2 + _id);
    }
}
