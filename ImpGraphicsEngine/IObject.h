#pragma once
#include <memory>

namespace ImpGraphics
{
	class ImpDevice;
	class ImpCamera;
	class ImpRenderTarget;
	class ImpRenderState;

	class GeometryBuffer;

	class IObject
	{
	public:
		IObject() {};
		virtual ~IObject() {};

		virtual void Update(ImpCamera* camera) abstract;
		virtual void Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget) abstract;

		virtual void SetTransformMatrix(DirectX::XMMATRIX transformMatrix) abstract;
		virtual DirectX::XMMATRIX GetTransformMatrix() abstract;

		virtual std::shared_ptr<GeometryBuffer> GetGeometryBuffer() abstract;
        virtual void SetGeometryBuffer(std::shared_ptr<GeometryBuffer> geometryBuffer) abstract;

		virtual void SetEmissionCoefficient(float emissionCoefficient) abstract;
	};
}