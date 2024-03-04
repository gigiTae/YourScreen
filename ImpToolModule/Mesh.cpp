#include "ImpToolModulePCH.h"
#include "Mesh.h"

ImpToolModule::Debug::Mesh::Mesh(ImpGraphics::IImpGraphicsEngine* graphics,
    ImpEngineModule::Entity* ent)
    : DebugObject(graphics, ent->GetID())
{
    _ent       = ent;
    _transform = ent->GetComponent<ImpEngineModule::Transform>();
    _mesh      = _ent->GetComponent<ImpEngineModule::Mesh>();

    if (_mesh)
    {
        Vector3 offset;

        _graphicsEngine->GetBoundingBoxLength(_mesh->GetFBXPath(), _centerOffset);
        _boundingVolume = _graphicsEngine->GetBoundingBoxLength(
            _mesh->GetFBXPath(), offset);
        _graphicsEngine->AddCubeBoundingVolume(_meshID, _boundingVolume);
    }
    else
    {
        _graphicsEngine->AddCubeBoundingVolume(_meshID, Vector3(1.f, 1.f, 1.f));
    }
}


ImpToolModule::Debug::Mesh::~Mesh()
{
    _graphicsEngine->DeleteBoundingVolume(_meshID);
}

void ImpToolModule::Debug::Mesh::Update()
{
    if (_mesh)
    {
        Matrix trans = Matrix::MakeTranslationMatrix(_centerOffset);

        Matrix mat = trans * _transform->GetWorldMatrix();
        _graphicsEngine->UpdateBoundingVolume(
            _meshID, mat, Vector4(1.f, 1.f, 0.6f));
    }
    else
    {
        Matrix mat = _transform->GetWorldMatrix();
        _graphicsEngine->UpdateBoundingVolume(
            _meshID, mat, Vector4(1.f, 1.f, 0.6f));
    }
}
