#pragma once

/// Texutre UV 좌표와 UV Texutre index 를 가져온다. 
/// texture 가 하나만 있다고 가정했을 때 작성된 함수다

#if _DEBUG
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/libfbxsdk-md.lib")
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/libxml2-md.lib")
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/zlib-md.lib")
#else
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/libfbxsdk-md.lib")
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/libxml2-md.lib")
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/zlib-md.lib")
#endif

#include "IFBXLoader.h"
#include <fbxsdk.h>

namespace ImpStructure
{
	struct ImpMesh;
}

namespace FBXLoad
{
	class Loader : public IFBXLoader
	{
	public:
		Loader();
		~Loader();

		ImpMesh* GetMesh(const char* file, size_t i) override;
		std::vector<ImpMesh*> GetMeshAll(const char* file) override;

		virtual size_t GetMeshSize(const char* file) override;

		// 파일 로드 
		void LoadFbxFile(const char* file);

		// fbx 객체가 존재하는지 확인
		constexpr bool IsValid() const;

	private:
		void InitilazeFbx();
		void LoadScene(FbxNode* root = nullptr);
		void LoadMesh(FbxNode* node);

		bool LoadMeshData(FbxMesh* fbxMesh, ImpMesh& myMesh);

		// FBX 객체
		FbxScene* _fbxScene{ nullptr };
		FbxManager* _fbxManager{ nullptr };

		std::vector<ImpMesh*> _meshes;
	};
}
