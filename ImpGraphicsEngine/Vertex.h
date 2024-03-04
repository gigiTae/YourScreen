#pragma once
#include "pch.h"

using namespace DirectX;

namespace ImpGraphics
{
	struct Vertex
	{
		struct PosColor
		{
			XMFLOAT3 Pos;
			XMFLOAT4 Color;
		};

		struct PosColorTex
		{
			XMFLOAT3 Pos;
			XMFLOAT4 Color;
			XMFLOAT2 Tex;
		};

		struct QuadVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT2 Tex;
            XMFLOAT3 ToFarPlaneIndex;
		};

		struct PosNormal
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
		};

		struct MeshVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT2 Tex;
			XMFLOAT3 Tangent;
		};

		struct SkinnedVertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT2 Tex;
            XMFLOAT3 Tangent;

			XMFLOAT3 weight; 
			UINT32 BoneIndices[4];
		};
	};

	class InputLayoutDesc
	{
	public:
		static const D3D11_INPUT_ELEMENT_DESC PosColor[2];
		static const D3D11_INPUT_ELEMENT_DESC PosColorTex[3];
		static const D3D11_INPUT_ELEMENT_DESC QuadVertex[3];
		static const D3D11_INPUT_ELEMENT_DESC PosNormal[2];
		static const D3D11_INPUT_ELEMENT_DESC MeshVertex[4];
		static const D3D11_INPUT_ELEMENT_DESC SkinnedVertex[6];
	};
}