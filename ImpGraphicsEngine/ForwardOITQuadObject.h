#pragma once
#include "IObject.h"
#include <memory>

namespace ImpGraphics
{
class GeometryBuffer;
class Shader;

class ForwardOITQuadObject : public IObject
{
public:
    ForwardOITQuadObject(std::shared_ptr<GeometryBuffer> vbib,
        std::shared_ptr<Shader> shader);
    virtual ~ForwardOITQuadObject();

    virtual void Update(ImpCamera* camera) override;
    virtual void Render(ImpDevice* device,
        ImpRenderState* renderState,
        ImpRenderTarget* rendertarget) override;

    virtual DirectX::XMMATRIX GetTransformMatrix() override
    {
        return DirectX::XMMatrixIdentity();
    };
    virtual void SetTransformMatrix(DirectX::XMMATRIX transformMatrix) override;

    virtual std::shared_ptr<GeometryBuffer> GetGeometryBuffer() override;
    virtual void SetGeometryBuffer(
        std::shared_ptr<GeometryBuffer> geometryBuffer) override
    {
        _geometryBuffer = geometryBuffer;
    }
    virtual void SetEmissionCoefficient(float emissionCoefficient) override { }

private:
    std::shared_ptr<GeometryBuffer> _geometryBuffer;
    std::shared_ptr<Shader> _shader;

    DirectX::XMFLOAT4X4 _viewTM;
    DirectX::XMFLOAT4X4 _projTM;
};
}