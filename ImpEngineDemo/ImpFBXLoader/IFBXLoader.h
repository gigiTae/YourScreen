/// IFBXLoader.h : Fbx Loader 가 다른 라이브러리에게 제공할 함수를 정의한 
/// 인터페이스 클레스 입니다.
///
/// 2023-12-27 김주영
/// 

#pragma once
#include "../ImpStructure/ImpMesh.h"

#pragma comment(lib, "../x64/Debug/ImpStructure.lib")

using namespace ImpStructure;

namespace FBXLoad
{
	class IFBXLoader
	{
	public:
		virtual ~IFBXLoader() {};

		virtual ImpMesh* GetMesh(const char* file, size_t i) abstract;
		virtual std::vector<ImpMesh*> GetMeshAll(const char* file) abstract;

		virtual size_t GetMeshSize(const char* file) abstract;

		// node 목록이 담긴 애니메이션오브젝트를 return 해야함
		virtual ImpStructure::ImpAnimationObject* GetAnimationObject(const char* file) abstract;
		// BoneIndex에 맞춰서 순서대로 Bone의 목록이 필요함. Index순서대로 넣는 게 어렵다면 맵으로 변경해도 문제없음
		virtual std::vector<ImpStructure::Bone*> GetBoneList() abstract;
	};

} // FBXLoad