#pragma once
#include "IBoundingVolumeObject.h"

namespace ImpGraphics
{
	class GeometryBuffer;
	class Shader;
	class Texture;

	class BoundingVolumeObject : public IBoundingVolumeObject
	{
	public:
		BoundingVolumeObject(std::shared_ptr<GeometryBuffer> vbib, std::shared_ptr<Shader> shader);
		virtual ~BoundingVolumeObject();

		virtual void Update(ImpCamera* camera) override;
		virtual void Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget) override;

		virtual void SetTransformMatrix(DirectX::XMMATRIX transformMatrix) override;
		virtual void SetColor(const ImpMath::Vector4& color) override;

	private:
		std::shared_ptr<GeometryBuffer> _geometryBuffer;
		std::shared_ptr<Shader> _shader;

		DirectX::XMFLOAT4 _color;

		DirectX::XMFLOAT4X4 _worldTM;
		DirectX::XMFLOAT4X4 _viewTM;
		DirectX::XMFLOAT4X4 _projTM;
	};
}