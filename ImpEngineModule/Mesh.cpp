#include "EngineModulePCH.h"
#include "Mesh.h"

ImpEngineModule::Mesh::Mesh()
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

ImpEngineModule::Mesh::~Mesh()
{
}

ImpEngineModule::Component* ImpEngineModule::Mesh::Clone(
    Component* clone /*=nullptr*/) const
{
    Mesh* mesh = static_cast<Mesh*>(clone);

    if (mesh == nullptr)
    {
        mesh = new Mesh(*this);
    }
    else
    {
        *mesh = *this;
    }

    mesh->_isBindGraphics = false;

    return mesh;
}

void ImpEngineModule::Mesh::SetAlpha(float alpha)
{
    _isChangedAlpha = true;
    _alpha = alpha;
}

void ImpEngineModule::Mesh::SetEmissionIntensity(float intensity)
{
    _emissionIntensity = intensity;
    _isChangedEmissionIntensity = true;
}
