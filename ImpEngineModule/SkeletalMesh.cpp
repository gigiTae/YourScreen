#include "EngineModulePCH.h"
#include "SkeletalMesh.h"


ImpEngineModule::SkeletalMesh::SkeletalMesh()
    : _isBindGraphics(false)
    , _renderType(RenderType::Default)
    , _isOpaque(true)
    , _isCullNone(false)
    , _emissionIntensity(1.f)
    , _alpha(1.f)
    , _isChangedAlpha(false)
    , _isChangedEmissionIntensity(false)
{
}


ImpEngineModule::SkeletalMesh::~SkeletalMesh()
{
}

ImpEngineModule::Component* ImpEngineModule::SkeletalMesh::Clone(
    Component* clone /*= nullptr*/) const
{
    SkeletalMesh* mesh = static_cast<SkeletalMesh*>(clone);

    if (mesh == nullptr)
    {
        mesh = new SkeletalMesh(*this);
    }
    else
    {
        *mesh = *this;
    }

    mesh->_isBindGraphics = false;

    return mesh;
}

void ImpEngineModule::SkeletalMesh::SetAlpha(float alpha)
{
    _isChangedAlpha = true;
    _alpha          = alpha;
}

void ImpEngineModule::SkeletalMesh::SetEmissionIntensity(float intensity)
{
    _emissionIntensity          = intensity;
    _isChangedEmissionIntensity = true;
}
