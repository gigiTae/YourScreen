#pragma once
#include "DebugObject.h"

namespace ImpToolModule::Debug
{

/// <summary>
/// Mesh BoundingVolume
/// </summary>
class Mesh: public DebugObject
{
public:
    Mesh(ImpGraphics::IImpGraphicsEngine* graphics, ImpEngineModule::Entity* ent);
    ~Mesh();

    void Update() override;
    ImpMath::Vector3 GetBoundingVolume() const
    {
        return _boundingVolume;
    }

private:
    ImpEngineModule::Entity* _ent;
    ImpEngineModule::Transform* _transform;
    ImpEngineModule::Mesh* _mesh;
    ImpMath::Vector3 _boundingVolume;
    ImpMath::Vector3 _centerOffset;

    const size_t _meshID = 999999999999999999;
};

} // namespace ImpToolModule::Debug