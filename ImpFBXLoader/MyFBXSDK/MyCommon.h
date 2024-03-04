///
/// ���� ��� �� ���� �Լ� ����
/// 
/// 2023.12.15 ���ֿ�
/// 
/// (URL: https://help.autodesk.com/view/FBX/2020/ENU/?guid=FBX_Developer_Help_cpp_ref_common_2_common_8h_example_html)
/// 

#pragma once

/// �޸� ���� Ȯ�ο�
/// TODO: ���� �Լ��� �ִ� ���� �ű� ��
/******************************************************************************************************************************/
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
/******************************************************************************************************************************/

/// ���̺귯�� �߰�

// �ش� ���̺귯�� ���� ������ �߻��� ��� ��ó��
// 1) ������Ʈ �Ӽ� ������ ������ Release/Debuge����, �÷����� x64 ���� Ȯ���Ѵ� 
// 2) ���䵥��ũ ����Ʈ���� FBX SDK 2020.3.4 vs2022 ������ �ٿ� �޴´� (URL: https://aps.autodesk.com/developer/overview/fbx-sdk)
// 3) FBX SDK �� ��ġ�� ���͸� ��θ� Ȯ���Ѵ� (�⺻�� : C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022)
// 4) "�߰� ���� ���͸�" �Ӽ� ���� fbxsdk.h �� ���Ե� ��θ� �Է��Ѵ�

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