///
/// 공동 헤더 및 공동 함수 정의
/// 
/// 2023.12.15 김주영
/// 
/// (URL: https://help.autodesk.com/view/FBX/2020/ENU/?guid=FBX_Developer_Help_cpp_ref_common_2_common_8h_example_html)
/// 

#pragma once

/// 메모리 누수 확인용
/// TODO: 메인 함수가 있는 쪽을 옮길 것
/******************************************************************************************************************************/
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
/******************************************************************************************************************************/

/// 라이브러리 추가

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
#include <cassert>

#ifndef _COMMON_H
#define _COMMON_H

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
void DestroySdkObjects(FbxManager* pManager, bool pExitStatus);
void CreateAndFillIOSettings(FbxManager* pManager);

bool SaveScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename, int pFileFormat = -1, bool pEmbedMedia = false);
bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);

#endif // #ifndef _COMMON_H