/// ImpAnimationObjLoader.h: FbxSecne 가져온 원본 sdk 객체 데이터를
/// ImpStuct 에 정의된 자료형으로 변환하는 클래스
///
/// 2024-02-08

#pragma once

#include "../ImpStructure/ImpMesh.h"
#include "SceneLoader.h"

#include <fbxsdk.h>
#include <memory>
#include <ImpColor.h>
#include <map>

namespace FBXLoad
{

class ImpAnimationObjLoader
{
public:
    void CreateAnimationObj(const char* file);
    void Release();

    ImpStructure::ImpMesh* GetMesh(size_t i = 0);
    ImpStructure::ImpAnimationObject* GetAnimationObject();
    std::vector<ImpStructure::Bone*> LoadBoneList();
    std::vector<ImpStructure::ImpMesh*> LoadMeshList();

private:
    void LoadImpNodes(std::unique_ptr<MyFbxNode>& myNode,
        std::unordered_map<std::string, ImpStructure::ImpAnimationNode*>& impNodes,
        std::unordered_map<std::string, std::unique_ptr<MyFbxAnimTransform>>& myAnimTrans);

    void LoadImpNode(ImpStructure::ImpAnimationNode* impNode,
        std::unique_ptr<MyFbxNode>& myNode);

    void LoadImpAnimation(ImpStructure::ImpAnimationNode* impNode,
        MyFbxAnimTransform* animTrans);

    void LoadImpBone(ImpStructure::ImpAnimationNode* impNode, FbxSkeleton* skeleton);

    void LoadMeshes(ImpStructure::ImpAnimationNode* impNode, FbxMesh* fbxMesh);

    void LoadVertex(FbxMesh* fbxMesh, ImpStructure::ImpMesh* impMesh);

    void LoadFace(ImpStructure::ImpMesh* impMesh);

    void LoadBoneNodeTM(ImpStructure::Bone* impNode, FbxMesh* fbxMesh);

    void LoadBoneWeight(FbxMesh* fbxMesh, ImpStructure::ImpMesh* impMesh);
    void LoadBoneWeight(FbxSkin* fbxSkin, ImpStructure::ImpMesh* impMesh);

    int FindeBoneIndex(std::string name);

    void Display(ImpStructure::ImpAnimationObject* impAnimObj);
    void Display(ImpStructure::ImpAnimationNode* impAnimNode);
    void DisplayNodeTM(ImpStructure::ImpAnimationNode* impAnimNode);
    void DisplayWeight(ImpStructure::ImpAnimationNode* impAnimNode);
    void DisplayTransform(ImpStructure::ImpAnimationNode* impAnimNode);
    void DisplayAnimation(ImpStructure::ImpAnimationNode* impAnimNode);

    // 변환 행렬
    ImpMath::Matrix LoadGlobalTransform(FbxNode* fbxNode);
    ImpMath::Matrix LoadLocalTransform(FbxNode* fbxNode);
    ImpMath::Matrix ConverImpMatrixFrom(const FbxAMatrix& fbxMatrix);

    // 부모 노드 주소
    void SetParentNodeAddress(const FbxNode* fbxNode,
        ImpStructure::ImpAnimationNode* impAnimNode,
        std::unordered_map<std::string, ImpStructure::ImpAnimationNode*>& impNodes);

private:
    ImpStructure::ImpAnimationObject* _impAnimObj{ nullptr };
    std::vector<ImpStructure::Bone*> _boneList;
    std::vector<ImpStructure::ImpMesh*> _meshList;
};

} // namespace FBXLoad