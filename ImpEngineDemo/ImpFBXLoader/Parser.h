#pragma once

//#include <fbxsdk.h>
//#include <vector>
#include "Common.h"

namespace FBXLoad
{
	inline namespace Test
	{

		struct Scene;
		//struct SceneData;
		struct Mesh;
		struct GeometryImportSettings;

		/// <summary>
		/// FBX �����͸� ���� �� ������ �� �޾Ҵ� 
		/// Secene �ڷ����� �����ϴ� Ŭ����
		/// </summary>
		class Parser
		{
		public:
			Parser(const char* fileName);
			~Parser();

			void GetScene(FbxNode* root = nullptr);

			// fbx ��ü�� �����ϴ��� Ȯ��
			constexpr bool IsValid() const;

		private:
			// �ʱ�ȭ
			bool InitilazeFbx();

			// ���� �ε�
			void LoadFbxFile(const char* fileName);

			// ��� ��������
			void GetNode(FbxNode* node);
			// ���ؽ� ��������
			void GetMesh(FbxNode* node, std::vector<Mesh>& meshes);
			bool GetMeshData(FbxMesh* fbxMesh, Mesh& myMesh);
			bool GetControlPoints(FbxMesh* fbxMesh, Mesh& myMesh);
			bool GetMaterial(FbxMesh* fbxMesh, Mesh& myMesh);
			void GetVertexNormals(FbxMesh* fbxMesh, Mesh& myMesh);
			void GetTangents(FbxMesh* fbxMesh, Mesh& myMesh);
			void GetUVSet(FbxMesh* fbxMesh, Mesh& myMesh);

			// ��ȯ ������ ��������
			// �ִϸ��̼� ������ ��������

			FbxManager* _fbxManager{ nullptr };
			FbxScene* _fbxScene{ nullptr };
			Scene* _scene{ nullptr };
		};
	}
}