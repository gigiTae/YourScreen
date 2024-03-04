#pragma once

#include "../ImpStructure/ImpMesh.h"

#include "ImpMeshLoader.h"
#include "SceneLoader.h"

#include <fbxsdk.h>
#include <memory>
#include <ImpColor.h>
#include <map>

namespace FBXLoad
{

class ImpAnimationObjectLoader2
{
public:
    ImpStructure::ImpAnimationObject* LoadAnimationObject2(const char* file);
    std::vector<ImpStructure::Bone*> LoadBoneList();
    std::vector<ImpStructure::ImpMesh*> LoadMeshList();

private:
    void LoadImpNode(
        std::unique_ptr<MyFbxNode>& myNode,
        ImpAnimationObject* impAnimObject,
        std::unordered_map<std::string, std::unique_ptr<MyFbxAnimTransform>>& myAnimTrans);

    void LoadMeshes(FbxMesh* fbxMesh, ImpMesh* impMesh);

    void LoadVertex(FbxMesh* fbxMesh, ImpMesh* impMesh);
    void LoadFace(ImpMesh* impMesh);

    void LoadBoneWeight(FbxMesh* fbxMesh, ImpMesh* impMesh);
    void LoadBoneWeight(FbxSkin* fbxSkin, ImpMesh* impMesh);

    int FindeBoneIndex(std::string name);

    // 변환 행렬
    ImpMath::Matrix LoadGlobalTransform(FbxNode* fbxNode);
    ImpMath::Matrix LoadLocalTransform(FbxNode* fbxNode);
    ImpMath::Matrix ConverImpMatrixFrom(const FbxAMatrix& fbxMatrix);

    // 부모 노드 주소
    void SetParentNodeAddress(const FbxNode* fbxNode,
                              ImpAnimationNode* impAnimNode,
                              ImpNodeMap& impNode);

    // 본 로드
    void LoadBoneData(FbxNode* fbxNode, Bone* _bone);

private:
    ImpAnimationObject* _impAnimObj{ nullptr };
    std::vector<ImpStructure::Bone*> _boneList;
    std::vector<ImpStructure::ImpMesh*> _meshList;

};

} // namespace FBXLoad