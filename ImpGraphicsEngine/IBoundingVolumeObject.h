#pragma once

namespace ImpMath { struct Vector4; }

namespace ImpGraphics
{
	class ImpDevice;
	class ImpCamera;
	class ImpRenderTarget;
	class ImpRenderState;

	class IBoundingVolumeObject
	{
	public:
		IBoundingVolumeObject() {};
		virtual ~IBoundingVolumeObject() {};

		virtual void Update(ImpCamera* camera) abstract;
		virtual void Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget) abstract;

		virtual void SetTransformMatrix(DirectX::XMMATRIX transformMatrix) abstract;
		virtual void SetColor(const ImpMath::Vector4& color) abstract;
	};
}