#pragma once
#include "IObject.h"
#include <memory>

#include "LightBase.h"

namespace ImpGraphics
{
	class GeometryBuffer;
	class Shader;
	class Texture;

	class QuadObject : public IObject
	{
	public:
		QuadObject(std::shared_ptr<GeometryBuffer> vbib, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> deferredTexture);
		virtual ~QuadObject();

		virtual void Update(ImpCamera* camera) override;
		virtual void Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget) override;

		virtual DirectX::XMMATRIX GetTransformMatrix() override { return DirectX::XMMatrixIdentity(); };
		virtual void SetTransformMatrix(DirectX::XMMATRIX transformMatrix) override;

		virtual std::shared_ptr<GeometryBuffer> GetGeometryBuffer() override;
        virtual void SetGeometryBuffer(std::shared_ptr<GeometryBuffer> geometryBuffer) override
        {
            _geometryBuffer = geometryBuffer;
        }
		void SetLight(std::unordered_map<size_t, LightBase*> _lights, float globalEmissionCoefficient);
		virtual void SetEmissionCoefficient(float emissionCoefficient) override { _emissionCoefficient = emissionCoefficient; }

	private:
		std::shared_ptr<GeometryBuffer> _geometryBuffer;
		std::shared_ptr<Shader> _shader;

		//temp
		std::shared_ptr<Texture> _irr;

		DirectX::XMFLOAT4X4 _viewTM;
		DirectX::XMFLOAT4X4 _projTM;

		float _emissionCoefficient;
	};
}