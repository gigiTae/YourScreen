/// FbxLoader.h: IFbxLoader 의 구현부이면서 원본 fbxSdk 객체의 데이터를 출력합니다.
///
/// 
/// 2023-12-29


#pragma once

// #if _DEBUG
// #pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/libfbxsdk-md.lib")
// #pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/libxml2-md.lib")
// #pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/zlib-md.lib")
// #else
// #pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/libfbxsdk-md.lib")
// #pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/libxml2-md.lib")
// #pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/zlib-md.lib")
// #endif

//#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/libfbxsdk.lib")
//#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/libxml2-mt.lib")
//#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/zlib-mt.lib")

#include "IFBXLoader.h"
#include "ImpAnimationObjLoader.h"

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

    void SetPath(const char* file) override; // SetPath를 하고 밑의 3가지 함수를 사용해야 함.
    ImpMesh* GetMesh(size_t i = 0) override;
    ImpStructure::ImpAnimationObject* GetAnimationObject() override;
    std::vector<ImpStructure::Bone*> GetBoneList() override;

    void Release();

public:
    void DisplayScene(const char* file);
    void DisplayTransformations(const char* file);

private:
    // DisplayScene 가 호출
    void DisplayMetaData(FbxScene* pScene);
    void DisplayTransformPropagation(FbxNode* pNode);
    void DisplayGeometricTransform(FbxNode* pNode);
    void DisplayGlobalTimeSettings(FbxGlobalSettings* pGlobalSettings);
    void DisplayHierarchy(FbxScene* pScene);
    void DisplayHierarchy(FbxNode* pNode, int pDepth);
    void DisplayContent(FbxNode* pNode);
    void DisplayPose(FbxScene* pScene);
    void DisplayAnimation(FbxScene* pScene);
    void DisplayAnimation(FbxNode* pNode, FbxAnimLayer* pAnimLayer);
    void DisplayCurveKeys(FbxAnimCurve* pCurve); 
    void DisplaySkeleton(FbxNode* pNode);
    void DisplayMesh(FbxNode* pNode);
    void DisplayPolygons(FbxMesh* pMesh);
    void DisplayLink(FbxMesh* pMesh);

    // DisplayTransformations 가 호출
    void CompareTransformations(FbxNode* pNode, FbxScene* pScene);
    FbxAMatrix CalculateGlobalTransform(FbxNode* pNode);

private:
    std::unique_ptr<ImpAnimationObjLoader> _impAnimaObjLoader{ nullptr };
};

} // namespace FBXLoad