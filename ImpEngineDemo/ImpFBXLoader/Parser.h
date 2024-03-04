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
		/// FBX 데이터를 읽은 뒤 생성할 때 받았던 
		/// Secene 자료형에 저장하는 클래스
		/// </summary>
		class Parser
		{
		public:
			Parser(const char* fileName);
			~Parser();

			void GetScene(FbxNode* root = nullptr);

			// fbx 객체가 존재하는지 확인
			constexpr bool IsValid() const;

		private:
			// 초기화
			bool InitilazeFbx();

			// 파일 로드
			void LoadFbxFile(const char* fileName);

			// 노드 가져오기
			void GetNode(FbxNode* node);
			// 버텍스 가져오기
			void GetMesh(FbxNode* node, std::vector<Mesh>& meshes);
			bool GetMeshData(FbxMesh* fbxMesh, Mesh& myMesh);
			bool GetControlPoints(FbxMesh* fbxMesh, Mesh& myMesh);
			bool GetMaterial(FbxMesh* fbxMesh, Mesh& myMesh);
			void GetVertexNormals(FbxMesh* fbxMesh, Mesh& myMesh);
			void GetTangents(FbxMesh* fbxMesh, Mesh& myMesh);
			void GetUVSet(FbxMesh* fbxMesh, Mesh& myMesh);

			// 변환 데이터 가져오기
			// 애니메이션 데이터 가져오기

			FbxManager* _fbxManager{ nullptr };
			FbxScene* _fbxScene{ nullptr };
			Scene* _scene{ nullptr };
		};
	}
}