#include "IFbxLoader.h"
#include "FbxLoader.h"
#include "GetLoader.h"

#include <string>
#include <Windows.h>
#include <stdlib.h>
//#include <crtdbg.h>

// crt 라이브러리
//#define _CRTDBG_MAP_ALLOC

int main()
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //_CrtSetBreakAlloc(244);

    //COORD bufferSize = { 40, (SHORT)1800000 };
    //SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);

    std::string path{ "../ImpD3DDemo/Resources/Mesh/fbx/" };
    std::string fileName;

    // fileName = "idle_251087.fbx";
    // fileName = "Slime.fbx";
    // fileName = "autorig_actor.fbx";
    // fileName = "chara_ani_test_2.fbx";
    // fileName = "walking.fbx";
    fileName = "running0.fbx";

    std::string filePath{ path + fileName };
    FBXLoad::FbxLoader* loader1{ nullptr };
    FBXLoad::IFBXLoader* loader2{ nullptr };
    FBXLoad::GetLoader loaderCreator;

    ////////////////////////////////////////////////////////////////////

    loader1 = new FBXLoad::FbxLoader();

    //loader1->DisplayScene(filePath.c_str());
    //loader1->DisplayTransformations(filePath.c_str());

    loader1->SetPath(filePath.c_str());
    auto animStack = loader1->GetAnimationObject();
    auto test = loader1->GetBoneList();
    auto test2     = loader1->GetMesh();

    loader1->Release();

    ////////////////////////////////////////////////////////////////////

    loader2 = loaderCreator.GetFBXLoader();

    //loader2->SetPath(filePath.c_str());
    //auto animObj = loader2->GetAnimationObject();
    //delete animObj;

    //auto bones = loader2->GetBoneList();
    //auto mesh = loader2->GetMesh();
    // delete mesh;
    //auto meshs = loader2->GetMeshAll();

    ////////////////////////////////////////////////////////////////////

    if (loader1 != nullptr) delete loader1;
    if (loader2 != nullptr) delete loader2;
}

///
/// 테스트 후 lib로 전환 시 설정에서 빌드 후 이벤트 명령줄에 다음 추가
///    copy "$(TargetDir)ImpFBXLoader.lib" "$(SolutionDir)ImpLib"
///
/// 안해도 됨
///
///    if "$(ConfigurationType)" == "StaticLibrary" (
///       copy "$(TargetDir)ImpFBXLoader.lib" "$(SolutionDir)ImpLib")
///