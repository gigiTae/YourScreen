#pragma once
#include "IObject.h"
#include <memory>

namespace ImpStructure { class Bone; }

namespace ImpGraphics
{
	class GeometryBuffer;
	class Shader;
	class Texture;
    class Material;

	class SkinnedMeshObject : public IObject
	{
	public:
		SkinnedMeshObject(std::shared_ptr<GeometryBuffer> vbib, std::shared_ptr<Shader> shader, Material* material);
		virtual ~SkinnedMeshObject();

		virtual void Update(ImpCamera* camera) override;
		virtual void Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget) override;

		virtual DirectX::XMMATRIX GetTransformMatrix() override { return XMLoadFloat4x4(&_worldTM); };
		virtual void SetTransformMatrix(DirectX::XMMATRIX transformMatrix) override;

		virtual std::shared_ptr<GeometryBuffer> GetGeometryBuffer() override { return _geometryBuffer; };
        virtual void SetGeometryBuffer(std::shared_ptr<GeometryBuffer> geometryBuffer) override
        {
            _geometryBuffer = geometryBuffer;
        }

		virtual void SetEmissionCoefficient(float emissionCoefficient) override {  }

		void SetAnimation(const std::wstring& animationName);
		void SetPreAnimation(const std::wstring& animationName);
		//void SetChangingCount(int curCount);
        int _changingCount;
        int _isOnInterpolation;
	private:
		std::shared_ptr<GeometryBuffer> _geometryBuffer;
		std::shared_ptr<Shader> _shader;
		//std::shared_ptr<Texture> _texture;
        Material* _material;

		DirectX::XMFLOAT4X4 _worldTM;
		DirectX::XMFLOAT4X4 _viewTM;
		DirectX::XMFLOAT4X4 _projTM;

		// temp - skinning
		std::vector<DirectX::XMFLOAT4X4> _finalBoneTMList; // 상수 버퍼로 전달할 값
		std::vector<DirectX::XMFLOAT4X4> _preFinalBoneTMList; // 상수 버퍼로 전달할 값
		std::vector<ImpStructure::Bone*> _BoneList;
		std::vector<ImpStructure::Bone*> _preBoneList;
        //int _changingCount;
	};
}