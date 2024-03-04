/// SceneLoader.h: FbxSecne ���� �ʿ��� �����͸� Fbx �ڷ��� ���� �������� Ŭ����
/// ���⼭ ������ �а� ���� �Ŵ����� �����. 
/// �� �� �ʿ��� Fbx �ڷḦ ��������
/// 2024-02-04

#pragma once

#include <fbxsdk.h>
#include <string>
#include <map>
#include <set>

#include <vector>
#include <unordered_map>
#include <memory>
#include <array>

namespace FBXLoad
{

struct MyFbxNode
{
    MyFbxNode(FbxNode* node)
        : name{ node->GetName() }
        , node{ node }
        , skeleton{ nullptr }
        , mesh{ nullptr }
    {
    }

    std::string name;

    FbxNode* node;                    // ���־�� �Ѵ�.
    FbxSkeleton* skeleton{ nullptr }; // ���� ���� ���� ���� �ִ�
    FbxMesh* mesh{ nullptr };         // ���� ���� ���� ���� �ִ�
};

struct AnimationInfo
{
    std::string name;

    unsigned int start;
    unsigned int duration;
    unsigned int end;
};

struct MyFbxAnimTransform
{

    struct MyFbxAnimTransform(std::map<unsigned int, FbxVector4> t,
        std::map<unsigned int, FbxVector4> r,
                              std::map<unsigned int, FbxVector4> s)
        : translation(t)
        , rotation(r)
        , scale(s){};

    std::map<unsigned int, FbxVector4> translation;
    std::map<unsigned int, FbxVector4> rotation;
    std::map<unsigned int, FbxVector4> scale;
};

class SceneLoader
{
public:
    SceneLoader(const char* file);
    ~SceneLoader();

    using AnimTransMap = std::unordered_map<std::string, std::unique_ptr<MyFbxAnimTransform>>;

private:
    void LoadScene(const char* file);

    void ProcessScene();
    void ProcessMaterial();
    void ProcessNode(FbxNode* node);
    void ProcessAnimations();
    void ProcessAnimations(FbxNode* node, FbxAnimLayer* animLayer);
    void ProcessAnimations(FbxNode* node,
        FbxAnimLayer* animLayer,
        std::unique_ptr<AnimTransMap>& fbxAnimationTrans,
        unsigned int frameCount);

    void ProcessSkeleton(FbxNode* node);
    void ProcessMesh(FbxNode* node);

    void LoadTimeSet(FbxAnimCurve* animCurve, std::set<unsigned int>& timeSet);

    void LoadXMap(FbxAnimCurve* animCurve, std::map<unsigned int, FbxVector4>& map, unsigned int frameCount);
    void LoadYMap(FbxAnimCurve* animCurve, std::map<unsigned int, FbxVector4>& map);
    void LoadZMap(FbxAnimCurve* animCurve, std::map<unsigned int, FbxVector4>& map);

private:
    FbxManager* _manager{ nullptr };
    FbxScene* _scene{ nullptr };
    FbxGeometryConverter* _geometyConverter{ nullptr };

public:
    std::vector<std::unique_ptr<MyFbxNode>> _myFbxNodesList;
    std::unordered_map<std::string, FbxNode*> _fbxNodeMap;

    std::vector<std::pair<std::string, FbxMesh*>> _fbxMeshs;

    // �Ʒ� ������ 
    AnimationInfo _animationInfo;
    std::unordered_map<std::string, std::unique_ptr<MyFbxAnimTransform>> _fbxAnimationTrans;

    // �Ʒ������� ��ü �Ǿ����
    std::unordered_map<std::string, std::unique_ptr<AnimTransMap>> _fbxAnimationStack;


private:
    std::vector<FbxSurfaceMaterial*> _fbxMaterials;
    std::vector<std::pair<std::string, FbxSkeleton*>> _fbxSkeletons;
};

} // namespace FBXLoad