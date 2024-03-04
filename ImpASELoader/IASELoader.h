#pragma once
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include "../ImpStructure/ImpMesh.h"

//#pragma comment(lib, "../x64/Debug/ImpStructure.lib")

namespace ASELoader
{
	class IASELoader
	{
	public:
		virtual ~IASELoader() {};

		virtual ImpStructure::ImpMesh* GetMesh(const char* file, size_t i) abstract;
		virtual std::vector<ImpStructure::ImpMesh*> GetMeshAll(const char* file) abstract;

		virtual ImpStructure::ImpAnimationObject* GetAnimationObject(const char* file) abstract;

		virtual size_t GetMeshSize(const char* file) abstract;
		virtual std::vector<ImpStructure::Bone*> GetBoneList() abstract;
	};

	class ASELoaderExpoter
	{
	public:
		static IASELoader* GetLoader();
		static void DeleteLoader();

	private:
		static IASELoader* _loader;
	};
}