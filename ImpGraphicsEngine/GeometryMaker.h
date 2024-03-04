#pragma once
#include <d3d11.h>
#include "../ImpStructure/ImpMesh.h"

namespace ImpStructure { struct ImpMesh; }

namespace ImpGraphics
{
	class GeometryMaker
	{
	public:
		ImpStructure::ImpMesh* MakeBoxLineListMesh(ImpMath::Vector3 center, ImpMath::Vector3 length);
		ImpStructure::ImpMesh* MakeSphereLineListMesh(ImpMath::Vector3 center, float radius);
        ImpStructure::ImpMesh* MakeGrid();
        ImpStructure::ImpMesh* MakeQuadPolygonMesh(ImpMath::Vector4 uiPos, DirectX::XMFLOAT2 screenSize);
	};
}