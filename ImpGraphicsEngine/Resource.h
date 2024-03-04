#pragma once

#include <string>

// temp
#include "LightBase.h"
#include "GeometryMaker.h"

struct ID3D11Buffer;
namespace ImpMath { struct Matrix; }
namespace ImpMath { struct Vector3; }
namespace ImpStructure { struct ImpMesh; }
namespace ImpStructure { struct ImpAnimationNode; }

namespace ImpGraphics
{
	enum class VertexType
	{
		PosColor,
		PosColorTex,
		QuadVertex,
		PosNormal,
		MeshVertex,
		SkinnedVertex
	};

	enum class ConstantBufferType
	{
		Mesh,
		Quad
	};

	class ResourceManager;

	class Resource
	{
	public:
		Resource(ResourceManager* resourceManager) 
			: _resourceManager(resourceManager) {};
		virtual ~Resource() {};

	protected:
		ResourceManager* _resourceManager;
	};

	class GeometryBuffer : public Resource
	{
	public:
		GeometryBuffer(ResourceManager* resourceManager);
		virtual ~GeometryBuffer();

		void Load(const std::wstring& path);
		void Load(const std::wstring& path, std::wstring& anmationName);

		void Load(const std::wstring& path, Vector3 center, Vector3 length);
		void Load(const std::wstring& path, Vector3 center, float radius);
		void Load(const std::wstring& path, Vector4 uiPos, DirectX::XMFLOAT2 screenSize);

		void AddAniamtion(const std::wstring& path, const std::wstring& anmationName);

		UINT GetSize() { return _VBs.size(); }

		ID3D11Buffer** GetVB(UINT index = 0) { return &_VBs[index]; }
		ID3D11Buffer* GetIB(UINT index = 0) { return _IBs[index]; }

		UINT GetVertexSize(UINT index = 0) { return _vertexSizes[index]; }
		UINT GetIndexSize(UINT index = 0) { return _indexSizes[index]; }

		//temp
		std::vector<ImpMath::Vector3> GetVertexPositions() { return _positions[0]; }
		std::vector<UINT>* GetIndices() { return &_indices[0]; }
		std::vector<ImpStructure::Bone*> GetBoneList() { return _bones; }
		std::vector<ImpStructure::Bone*> GetBoneList(const std::wstring& animationName) { return _animationsBones[animationName]; }

		ImpMath::Matrix& GetWorldTransform(UINT index);

	private:
		void CreateVBIB(ImpStructure::ImpMesh* mesh);

	private:
		std::vector<UINT> _vertexSizes; // Vertex::xxx 의 sizeof 값
		std::vector<UINT> _indexSizes; // 인덱스 버퍼 안의 인덱스 수 

		// 메시 개수
		UINT _size;
		std::vector<ID3D11Buffer*> _VBs;
		std::vector<ID3D11Buffer*> _IBs;

		std::vector<ImpStructure::ImpAnimationNode*> _nodes; // 행렬을 저장할 벡터. 값이 아닌 이유는 애니메이션으로 바뀌었을 때 가져오려고
		std::vector<ImpStructure::Bone*> _bones;
        std::unordered_map<std::wstring, std::vector<ImpStructure::Bone*>> _animationsBones;

		VertexType _vertexType;

		// 원본(e.g) vector3 array)을 저장할 까 말까 저장해야지...
		std::vector<std::vector<ImpMath::Vector3> > _positions;
		std::vector<std::vector<UINT> > _indices;
	};

	// temp
	class Shader : public Resource
	{
	public:
		struct MeshConstantBuffer
		{
			DirectX::XMFLOAT4X4 transform;
			DirectX::XMFLOAT4X4 worldInvTranspose;
			DirectX::XMFLOAT4X4 world;
		};

		struct QuadConstantBuffer
		{
            DirectX::XMFLOAT4 one;
			DirectionalLightData directionalData[3];
			PointLightData pointData[30];
			SpotLightData spotData[10];
			DirectX::XMFLOAT3 eyePos;
            float globalEmissionCoefficient;
            unsigned int numOfDirectionalLight;
            unsigned int numOfSpotLight;
            unsigned int numOfPointLight;
            float objectEmissionCoefficient;
		};

		struct WorldViewProjConstantBuffer
		{
			DirectX::XMFLOAT4X4 worldViewProj;
			DirectX::XMFLOAT4 color;
		};

		struct SkinnedMeshConstantBuffer
		{
			DirectX::XMFLOAT4X4 transform;
			DirectX::XMFLOAT4X4 worldInvTranspose;
			DirectX::XMFLOAT4X4 world;

			DirectX::XMFLOAT4X4 boneTransform[100];
			DirectX::XMFLOAT4X4 preBoneTransform[100];

			int changingCount;
			int maxChanging;
			int isOnInterpolation;
            float pad;
			// skinning TM 넘기기 위한 상수버ㅓㅍ
			// 셰이더 코드에서 VS 에서 적용
		};

		struct DownSamplingVSConstantBuffer
        {
			// x: widht
			// y: height
			// z: 1920 * 1080에서 몇 배수로 나눈 건지
            DirectX::XMFLOAT4 screenSize;
        };

		struct bloomQuadConstantBuffer
        {
            float fStregth;
            float fTime0_X;
            DirectX::XMFLOAT2 uvDir;
        };

		struct UIConstantBuffer
        {
            float x;
            float y;
            float ratio;
            float pad;
        };

		struct UIAlphaConstantBuffer
		{
            float alpha;
            float fStregth;
            DirectX::XMFLOAT2 uvDir;

			int isNoise;
            int isPlus;
            float xRatio;
            int pad3;
		};

		struct AlphaConstantBuffer
        {
            float alpha;
            DirectX::XMFLOAT3 pad;
        };

		struct EmissionCoefficientConstantBuffer
		{
            float objectEmissionCoefficient;
            DirectX::XMFLOAT3 pad;
		};

	public:
		Shader(ResourceManager* resourceManager);
		virtual ~Shader();

		void Load(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, VertexType vertexType);

		void SetConstantBuffer(MeshConstantBuffer constantBufferData);
		void SetConstantBuffer(QuadConstantBuffer constantBufferData);
		void SetConstantBuffer(WorldViewProjConstantBuffer constantBufferData);
		void SetConstantBuffer(SkinnedMeshConstantBuffer constantBufferData);
        void SetConstantBuffer(DownSamplingVSConstantBuffer constantBufferData);
        void SetConstantBuffer(UIConstantBuffer constantBufferData);
        void SetConstantBuffer(UIAlphaConstantBuffer constantBufferData);
        void SetConstantBuffer(AlphaConstantBuffer constantBufferData);
        void SetConstantBuffer(bloomQuadConstantBuffer constantBufferData);
        void SetConstantBuffer(EmissionCoefficientConstantBuffer constantBufferData);

		ID3D11VertexShader* GetVertexShader() { return _vertexShader; }
		ID3D11PixelShader* GetPixelShader() { return _pixelShader; }
		ID3D11InputLayout* GetInputLayout() { return _inputLayout; }

		//temp 
		ResourceManager* GetResourceManager() {return _resourceManager;}
	private:
		ID3D11VertexShader* _vertexShader;
		ID3D11PixelShader* _pixelShader;

        ID3D11Buffer* _meshConstantBuffer;
        ID3D11Buffer* _quadConstantBuffer;
        ID3D11Buffer* _worldViewProjConstantBuffer;
        ID3D11Buffer* _skinnedMeshConstantBuffer;
        ID3D11Buffer* _downSamplingVSConstantBuffer;
        ID3D11Buffer* _UIConstantBuffer;
        ID3D11Buffer* _UIAlphaConstantBuffer;
        ID3D11Buffer* _alphaConstantBuffer;
        ID3D11Buffer* _bloomQuadConstantBuffer;
        ID3D11Buffer* _emssionCoefficientConstantBuffer;

		ID3D11InputLayout* _inputLayout;
	};

	class Texture : public Resource
	{
	public:
		Texture(ResourceManager* resourceManager);
		virtual ~Texture();

		void Load(const std::wstring& texturePath);

		ID3D11ShaderResourceView*& GetTextureSRV() { return _textureSRV; }

	private:
		ID3D11ShaderResourceView* _textureSRV;
	};
}