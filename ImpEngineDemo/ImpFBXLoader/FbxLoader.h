/// FbxLoader.h
/// - FbxScene 과 FbxManager 객체를 관리합니다.
/// - Scene 안의 기타 정보를 관리하는 객체들을 관리합니다
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
    // 매시
    ImpMesh* GetMesh(const char* file, size_t i);
    std::vector<ImpMesh*> GetMeshAll(const char* file) override;

    // 리펙토리 필요 **
    size_t GetMeshSize(const char* file) override;

    // 애니메이션 로드 미완성 ***
    ImpStructure::ImpAnimationObject* GetAnimationObject(const char* file) override;
    std::vector<ImpStructure::Bone*> GetBoneList() override;

public:
    // 디버깅용. 모든 데이터 로드
    void LoadAll(const char* file);

private: 
    /// </summary>
    /// Secne 생성.
    /// 파일을 로드 할 때마다 호출된다.
    /// </summary>
    bool CreateFbxScene(const char* file);

    //////////////////////////////////////////////////////////////////////
    
    // 계층 구조마다 갖는 데이터
    void DisplayContent(FbxNode* node);
    void DisplayContent(); // 메시, 스켈레톤 등
    void DisplayTarget(FbxNode* pNode);
    void DisplayTransformPropagation(FbxNode* pNode);
    void DisplayGeometricTransform(FbxNode* pNode);

    // Scene 마다 갖는 데이터
    void DisplayPose();   // 포즈 로드
    void DisplayAnimation(); // 애니메이션 로드

    //////////////////////////////////////////////////////////////////////

    // 메시 로드
    void LoadMesh(const char* file);
    void LoadMesh(FbxNode* node);
    void LoadMesh();

    // 애니메이션 로드 미완성 **
    ImpStructure::ImpAnimationObject* LoadAnimationObject(const char* file);
    // void LoadAnimation(FbxNode* node);
    // void LoadAnimation();

private:
    // FBX 객체

    FbxScene* _scene{ nullptr };
    FbxManager* _manager{ nullptr };

    // Loader 객체

    std::unique_ptr<PoseLoader> _posLoader{ nullptr };
    std::unique_ptr<AnimationLoader> _animationLoader{ nullptr };

    std::unique_ptr<SkeletonLoader> _skeletonLoader{ nullptr };
    std::unique_ptr<MeshLoader> _meshLoader{ nullptr };
};

} // namespace FBXLoad