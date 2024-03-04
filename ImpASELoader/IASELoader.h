#pragma once
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

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