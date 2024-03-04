/// 
///  간단한 FBX SDK 프로그램 예제.
/// 해당 예제를 다음 사항을 배울 수 있습니다.
/// 
/// 1. FBX SDK 매모리 매니저 객체를 초기화하기. [`FbxManager`]
/// 2. FBX 파일에서 씬(secene)을 불러 오기. [`FbxIOSettings`, `FbxImporter`, `FbxScene`]
/// 3. 씬 하나의 계층 구조 노드를 읽기. [`FbxScene`, `FbxNode`, `FbxNodeAttribute`]
/// 4. 씬 노드 정보 접근과 출력하기 [`FbxNode`, `FbxNodeAttribute`, `FbxString`]
/// 
/// 2023.12.15 김주영
/// 
/// (참고 URL: https://help.autodesk.com/view/FBX/2020/ENU/?guid=FBX_Developer_Help_getting_started_your_first_fbx_sdk_program_html)
/// 

#pragma once

// 메모리 누수 확인용
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// 해당 라이브러리 관련 오류가 발생할 경우 대처법
// 1) 프로젝트 속성 설정의 구성이 Release/Debuge인지, 플랫폼이 x64 인지 확인한다 
// 2) 오토데스크 사이트에서 FBX SDK 2020.3.4 vs2022 버전을 다운 받는다 (URL: https://aps.autodesk.com/developer/overview/fbx-sdk)
// 3) FBX SDK 가 설치된 디렉터리 경로를 확인한다 (기본값 : C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022)
// 4) "추가 포함 디렉터리" 속성 에서 fbxsdk.h 가 포함된 경로를 입력한다

#if _DEBUG
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/libfbxsdk-md.lib")
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/libxml2-md.lib")
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/zlib-md.lib")
#else
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/libfbxsdk-md.lib")
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/libxml2-md.lib")
#pragma comment(lib, "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/zlib-md.lib")
#endif


#include <fbxsdk.h>
#include <iostream>

/* 탭할 수 */
int numTabs = 0;

/**
 * 필요한 만큼 탭
 */
void PrintTabs() 
{
    for (int i = 0; i < numTabs; i++)
        printf("\t");
}

/**
 * Return a string-based representation based on the attribute type.
 */
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch (type) {
    case FbxNodeAttribute::eUnknown: return "unidentified";
    case FbxNodeAttribute::eNull: return "null";
    case FbxNodeAttribute::eMarker: return "marker";
    case FbxNodeAttribute::eSkeleton: return "skeleton";
    case FbxNodeAttribute::eMesh: return "mesh";
    case FbxNodeAttribute::eNurbs: return "nurbs";
    case FbxNodeAttribute::ePatch: return "patch";
    case FbxNodeAttribute::eCamera: return "camera";
    case FbxNodeAttribute::eCameraStereo: return "stereo";
    case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
    case FbxNodeAttribute::eLight: return "light";
    case FbxNodeAttribute::eOpticalReference: return "optical reference";
    case FbxNodeAttribute::eOpticalMarker: return "marker";
    case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
    case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
    case FbxNodeAttribute::eBoundary: return "boundary";
    case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
    case FbxNodeAttribute::eShape: return "shape";
    case FbxNodeAttribute::eLODGroup: return "lodgroup";
    case FbxNodeAttribute::eSubDiv: return "subdiv";
    default: return "unknown";
    }
}

/**
 * 속성 출력
 */
void PrintAttribute(FbxNodeAttribute* pAttribute) {
    if (!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

/**
 * 한 노드의 자식 노드를 재귀적으로 출력
 */
void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // 노드 콘텐츠 출력
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
        nodeName,
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // 노드의 속성 출력
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // 재귀적으로 자식 노드 출력
    for (int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}

/// <summary>
/// 하드 코딩으로 fbx 파일 로드 및 xml 형식으로 출력
/// </summary>
/// <returns></returns>
int main() 
{
    /// 1. 객체 생성

	// fbx 파일 이름
	const char* lFilename = "file.fbx";

    // FBX SDK 매니저 초기화
	FbxManager* lSdkManager = FbxManager::Create();


    ///  2. FBX 파일 불러오기

	// IO 설정 객체 생성 및 초기화
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// FBX SDK 매니저로 importer 생성
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// 해당 파일이 없으면 다음과 같은 오류 발생
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) 
    {
		printf("FbxImporter::Initialize() 함수 호출 실패.\n");
		printf("오류 보고: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}


    /// 3. 씬 데이터 읽기

    // 씬을 생성
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// importer 에게 씬 전달
	lImporter->Import(lScene);

	// 씬을 불러왔으므로 importer 삭제
	lImporter->Destroy();

    // 재귀적으로 씬의 노드와 노드 속성을 출력.
    // (참조: 루트 노드는 어떤 속성을 갖지 않으므로 출력하지 않는다)
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) 
    {
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			PrintNode(lRootNode->GetChild(i));
	}
    lRootNode->Destroy();


    // FBX SDK 매니저가 소멸하면 다른 FBX SDK 객체도 소멸된다. 
    // ㄴ 라고 설명하지만 메모리 누수 엄청 발생함
	lSdkManager->Destroy();

	/*******************************************************************************************************/
	/// 메모리 누수 검사 
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); // 누수 위치로 이동
	_CrtDumpMemoryLeaks(); // 누수 정보 출력
	/*******************************************************************************************************/
}