///
/// 외부 자료형에 의존하지 않고 
/// 정의한 구조체
///

#pragma once

#include "Common.h"

namespace FBXLoad
{
	inline namespace Test
	{ 
		struct Vertex
		{
			FbxVector4 position;
			FbxVector2 uv;
			FbxVector4 normal;
			//binormal
			FbxVector4 tangent;
		};

		struct Mesh
		{
			std::vector<int> materialIndices;
			std::vector<int> materialUsed;

			/// *** 가공된 데이터 *** ///
			std::vector<int> indices;
			std::vector<Vertex> vertices;

			/// *** ID 데이터 *** ///
			int id;
			const char* name;
			//VertexStatic
		};

		struct Node
		{
			FbxAMatrix GlobalTransform;
			FbxAMatrix LocalTransform;
		};

		struct Scene
		{
			std::vector<Mesh> meshes;
			std::vector<Node> nodes;
			const char* name;
		};
	}
}