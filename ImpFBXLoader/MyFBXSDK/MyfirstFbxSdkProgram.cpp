/// 
///  ������ FBX SDK ���α׷� ����.
/// �ش� ������ ���� ������ ��� �� �ֽ��ϴ�.
/// 
/// 1. FBX SDK �Ÿ� �Ŵ��� ��ü�� �ʱ�ȭ�ϱ�. [`FbxManager`]
/// 2. FBX ���Ͽ��� ��(secene)�� �ҷ� ����. [`FbxIOSettings`, `FbxImporter`, `FbxScene`]
/// 3. �� �ϳ��� ���� ���� ��带 �б�. [`FbxScene`, `FbxNode`, `FbxNodeAttribute`]
/// 4. �� ��� ���� ���ٰ� ����ϱ� [`FbxNode`, `FbxNodeAttribute`, `FbxString`]
/// 
/// 2023.12.15 ���ֿ�
/// 
/// (���� URL: https://help.autodesk.com/view/FBX/2020/ENU/?guid=FBX_Developer_Help_getting_started_your_first_fbx_sdk_program_html)
/// 

#pragma once

// �޸� ���� Ȯ�ο�
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

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
#include <iostream>

/* ���� �� */
int numTabs = 0;

/**
 * �ʿ��� ��ŭ ��
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
 * �Ӽ� ���
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
 * �� ����� �ڽ� ��带 ��������� ���
 */
void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // ��� ������ ���
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
        nodeName,
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // ����� �Ӽ� ���
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // ��������� �ڽ� ��� ���
    for (int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}

/// <summary>
/// �ϵ� �ڵ����� fbx ���� �ε� �� xml �������� ���
/// </summary>
/// <returns></returns>
int main() 
{
    /// 1. ��ü ����

	// fbx ���� �̸�
	const char* lFilename = "file.fbx";

    // FBX SDK �Ŵ��� �ʱ�ȭ
	FbxManager* lSdkManager = FbxManager::Create();


    ///  2. FBX ���� �ҷ�����

	// IO ���� ��ü ���� �� �ʱ�ȭ
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// FBX SDK �Ŵ����� importer ����
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// �ش� ������ ������ ������ ���� ���� �߻�
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) 
    {
		printf("FbxImporter::Initialize() �Լ� ȣ�� ����.\n");
		printf("���� ����: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}


    /// 3. �� ������ �б�

    // ���� ����
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// importer ���� �� ����
	lImporter->Import(lScene);

	// ���� �ҷ������Ƿ� importer ����
	lImporter->Destroy();

    // ��������� ���� ���� ��� �Ӽ��� ���.
    // (����: ��Ʈ ���� � �Ӽ��� ���� �����Ƿ� ������� �ʴ´�)
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) 
    {
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			PrintNode(lRootNode->GetChild(i));
	}
    lRootNode->Destroy();


    // FBX SDK �Ŵ����� �Ҹ��ϸ� �ٸ� FBX SDK ��ü�� �Ҹ�ȴ�. 
    // �� ��� ���������� �޸� ���� ��û �߻���
	lSdkManager->Destroy();

	/*******************************************************************************************************/
	/// �޸� ���� �˻� 
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); // ���� ��ġ�� �̵�
	_CrtDumpMemoryLeaks(); // ���� ���� ���
	/*******************************************************************************************************/
}