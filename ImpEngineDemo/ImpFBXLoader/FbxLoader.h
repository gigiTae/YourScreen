/// FbxLoader.h
/// - FbxScene �� FbxManager ��ü�� �����մϴ�.
/// - Scene ���� ��Ÿ ������ �����ϴ� ��ü���� �����մϴ�
///
/// 2023-12-29


#pragma once

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

#include "AnimationLoader.h"
#include "MeshLoader.h"
#include "PoseLoader.h"
#include "SkeletonLoader.h"

#include <fbxsdk.h>
#include <memory>

namespace FBXLoad
{
class FbxLoader: public IFBXLoader
{
public:
    FbxLoader();
    ~FbxLoader();

public:
    // �Ž�
    ImpMesh* GetMesh(const char* file, size_t i);
    std::vector<ImpMesh*> GetMeshAll(const char* file) override;

    // �����丮 �ʿ� **
    size_t GetMeshSize(const char* file) override;

    // �ִϸ��̼� �ε� �̿ϼ� ***
    ImpStructure::ImpAnimationObject* GetAnimationObject(const char* file) override;
    std::vector<ImpStructure::Bone*> GetBoneList() override;

public:
    // ������. ��� ������ �ε�
    void LoadAll(const char* file);

private: 
    /// </summary>
    /// Secne ����.
    /// ������ �ε� �� ������ ȣ��ȴ�.
    /// </summary>
    bool CreateFbxScene(const char* file);

    //////////////////////////////////////////////////////////////////////
    
    // ���� �������� ���� ������
    void DisplayContent(FbxNode* node);
    void DisplayContent(); // �޽�, ���̷��� ��
    void DisplayTarget(FbxNode* pNode);
    void DisplayTransformPropagation(FbxNode* pNode);
    void DisplayGeometricTransform(FbxNode* pNode);

    // Scene ���� ���� ������
    void DisplayPose();   // ���� �ε�
    void DisplayAnimation(); // �ִϸ��̼� �ε�

    //////////////////////////////////////////////////////////////////////

    // �޽� �ε�
    void LoadMesh(const char* file);
    void LoadMesh(FbxNode* node);
    void LoadMesh();

    // �ִϸ��̼� �ε� �̿ϼ� **
    ImpStructure::ImpAnimationObject* LoadAnimationObject(const char* file);
    // void LoadAnimation(FbxNode* node);
    // void LoadAnimation();

private:
    // FBX ��ü

    FbxScene* _scene{ nullptr };
    FbxManager* _manager{ nullptr };

    // Loader ��ü

    std::unique_ptr<PoseLoader> _posLoader{ nullptr };
    std::unique_ptr<AnimationLoader> _animationLoader{ nullptr };

    std::unique_ptr<SkeletonLoader> _skeletonLoader{ nullptr };
    std::unique_ptr<MeshLoader> _meshLoader{ nullptr };
};

} // namespace FBXLoad