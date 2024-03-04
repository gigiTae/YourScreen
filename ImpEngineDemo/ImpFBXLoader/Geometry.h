///
/// �ܺ� �ڷ����� �������� �ʰ� 
/// ������ ����ü
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

			/// *** ������ ������ *** ///
			std::vector<int> indices;
			std::vector<Vertex> vertices;

			/// *** ID ������ *** ///
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