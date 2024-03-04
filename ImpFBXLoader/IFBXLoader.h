/// IFBXLoader.h : Fbx Loader 가 다른 라이브러리에게 제공할 함수를 정의한 
/// 인터페이스 클레스 입니다.
///
/// 2023-12-27 김주영
/// 

#pragma once
#include "../ImpStructure/ImpMesh.h"

using namespace ImpStructure;

namespace FBXLoad
{
	class IFBXLoader
	{
	public:
		virtual ~IFBXLoader() {};

		virtual void SetPath(const char* file) abstract; // SetPath를 하고 밑의 3가지 함수를 사용해야 함.

		virtual ImpStructure::ImpAnimationObject* GetAnimationObject() abstract;
		virtual ImpMesh* GetMesh(size_t i = 0) abstract;
		virtual std::vector<ImpStructure::Bone*> GetBoneList() abstract;
	};

} // FBXLoad