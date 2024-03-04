/// IFBXLoader.h : Fbx Loader �� �ٸ� ���̺귯������ ������ �Լ��� ������ 
/// �������̽� Ŭ���� �Դϴ�.
///
/// 2023-12-27 ���ֿ�
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

		virtual void SetPath(const char* file) abstract; // SetPath�� �ϰ� ���� 3���� �Լ��� ����ؾ� ��.

		virtual ImpStructure::ImpAnimationObject* GetAnimationObject() abstract;
		virtual ImpMesh* GetMesh(size_t i = 0) abstract;
		virtual std::vector<ImpStructure::Bone*> GetBoneList() abstract;
	};

} // FBXLoad