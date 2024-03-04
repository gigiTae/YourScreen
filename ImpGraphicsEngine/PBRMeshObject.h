#pragma once
#include "IObject.h"
#include <memory>

namespace ImpGraphics
{
class GeometryBuffer;
class Shader;
class Texture;
class Material;

class PBRMeshObject : public IObject
{
public:
    PBRMeshObject(std::shared_ptr<GeometryBuffer> vbib,
               std::shared_ptr<Shader> shader,
                  Material* material);
    virtual ~PBRMeshObject();

    virtual void Update(ImpCamera* camera) override;
    virtual void Render(ImpDevice* device,
                        ImpRenderState* renderState,
                        ImpRenderTarget* rendertarget) override;

    virtual DirectX::XMMATRIX GetTransformMatrix() override
    {
        return XMLoadFloat4x4(&_worldTM);
    };
    virtual void SetTransformMatrix(DirectX::XMMATRIX transformMatrix) override;

    virtual std::shared_ptr<GeometryBuffer> GetGeometryBuffer() override
    {
        return _geometryBuffer;
    };
    virtual void SetGeometryBuffer(std::shared_ptr<GeometryBuffer> geometryBuffer) override
    {
        _geometryBuffer = geometryBuffer;
    }

    virtual void SetEmissionCoefficient(float emissionCoefficient) override { _emissionCoefficient = emissionCoefficient; }

private:
    std::shared_ptr<GeometryBuffer> _geometryBuffer;
    std::shared_ptr<Shader> _shader;
    Material* _material;

    DirectX::XMFLOAT4X4 _worldTM;
    DirectX::XMFLOAT4X4 _viewTM;
    DirectX::XMFLOAT4X4 _projTM;

    float _emissionCoefficient;
};
} // namespace ImpGraphics
