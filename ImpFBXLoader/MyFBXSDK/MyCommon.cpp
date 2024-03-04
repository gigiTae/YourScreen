#include "MyCommon.h"

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(manager->GetIOSettings()))
#endif

void InitializeSdkObjects(FbxManager*& manager, FbxScene*& scene)
{
	// FBX �Ŵ����� �ʱ�ȭ
	manager = FbxManager::Create();
	if (!manager)
	{
		FBXSDK_printf("����: FBX �Ŵ����� ���� �� ����!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", manager->GetVersion());

	// IOSettings ��ü ����. �ҷ�����/�������� ������ ����Ѵ�.
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	manager->LoadPluginsDirectory(lPath.Buffer());

	// FBX �� ����. ��κ� ��ü���� ��������/�������� �� ����Ѵ�.
	scene = FbxScene::Create(manager, "My Scene");
	if (!scene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}

void DestroySdkObjects(FbxManager* manager, bool exitStatus)
{
	// FBX �Ŵ��� ����. FBX �Ŵ����� ������ ��� ��ü�� ����
	if (manager) manager->Destroy();
	if (exitStatus) FBXSDK_printf("���α׷� ����!\n");
}

bool SaveScene(FbxManager* manager, FbxDocument* scene, const char* filename, int fileFormat, bool embedMedia)
{
	int lMajor, lMinor, lRevision;
	bool lStatus = true;

	// exporter ����
	FbxExporter* lExporter = FbxExporter::Create(manager, "");
	if (fileFormat < 0 || fileFormat >= manager->GetIOPluginRegistry()->GetWriterFormatCount())
	{
		// ASCII ������ �ƴ϶�� ��ü �������� ����
		fileFormat = manager->GetIOPluginRegistry()->GetNativeWriterFormat();

		// �����ϸ� ASCII�� ���������� �õ��ϱ�
		int lFormatIndex, lFormatCount = manager->GetIOPluginRegistry()->GetWriterFormatCount();
		for (lFormatIndex = 0; lFormatIndex < lFormatCount; lFormatIndex++)
		{
			if (manager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
			{
				FbxString lDesc = manager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
				const char* lASCII = "ascii";
				if (lDesc.Find(lASCII) >= 0)
				{
					fileFormat = lFormatIndex;
					break;
				}
			}
		}
	}

	// �������� ����. eEXPORT_TEXTURE_AS_EMBEDDED �ɼ��� ���� �� �⺻������ ��� �� ����
	IOS_REF.SetBoolProp(EXP_FBX_MATERIAL, true);
	IOS_REF.SetBoolProp(EXP_FBX_TEXTURE, true);
	IOS_REF.SetBoolProp(EXP_FBX_EMBEDDED, embedMedia);
	IOS_REF.SetBoolProp(EXP_FBX_SHAPE, true);
	IOS_REF.SetBoolProp(EXP_FBX_GOBO, true);
	IOS_REF.SetBoolProp(EXP_FBX_ANIMATION, true);
	IOS_REF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	// filname ������ ���� exporter �ʱ�ȭ.
	if (lExporter->Initialize(filename, fileFormat, manager->GetIOSettings()) == false)
	{
		FBXSDK_printf("FbxExporter::Initialize() ȣ�� ����.\n");
		FBXSDK_printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
		return false;
	}
	FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
	FBXSDK_printf("FBX file format version %d.%d.%d\n\n", lMajor, lMinor, lRevision);

	// �� ��������
	lStatus = lExporter->Export(scene);

	// exporter �Ҹ�
	lExporter->Destroy();
	return lStatus;
}
bool LoadScene(FbxManager* manager, FbxDocument* scene, const char* filename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// ���� ���� ��ȣ ����
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// importer ����
	FbxImporter* lImporter = FbxImporter::Create(manager, "");
	
	// filename ������ ���� importer �ʱ�ȭ
	const bool lImportStatus = lImporter->Initialize(filename, -1, manager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());
		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", filename, lFileMajor, lFileMinor, lFileRevision);
		}
		return false;
	}
	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
	if (lImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", filename, lFileMajor, lFileMinor, lFileRevision);
		
		// ���⼭ ��� ������ �ε����� �ʰ� ��� animation stack ������ ������ �� �ִ�.
		FBXSDK_printf("Animation Stack Information\n");
		lAnimStackCount = lImporter->GetAnimStackCount();
		FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");
		for (int i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);
			FBXSDK_printf("    Animation Stack %d\n", i);
			FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());
			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());
			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}
		
		// �������� ����. �⺻������ �� ����
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}
	// Import the scene.
	lStatus = lImporter->Import(scene);
	if (lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError)
	{
		FBXSDK_printf("Please enter password: ");
		lPassword[0] = '\0';
		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END
			FbxString lString(lPassword);
		IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);
		lStatus = lImporter->Import(scene);
		if (lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("\nPassword is wrong, import aborted.\n");
		}
	}
	if (!lStatus || (lImporter->GetStatus() != FbxStatus::eSuccess))
	{
		FBXSDK_printf("********************************************************************************\n");
		if (lStatus)
		{
			FBXSDK_printf("WARNING:\n");
			FBXSDK_printf("   The importer was able to read the file but with errors.\n");
			FBXSDK_printf("   Loaded scene may be incomplete.\n\n");
		}
		else
		{
			FBXSDK_printf("Importer failed to load the file!\n\n");
		}
		if (lImporter->GetStatus() != FbxStatus::eSuccess)
			FBXSDK_printf("   Last error message: %s\n", lImporter->GetStatus().GetErrorString());
		FbxArray<FbxString*> history;
		lImporter->GetStatus().GetErrorStringHistory(history);
		if (history.GetCount() > 1)
		{
			FBXSDK_printf("   Error history stack:\n");
			for (int i = 0; i < history.GetCount(); i++)
			{
				FBXSDK_printf("      %s\n", history[i]->Buffer());
			}
		}
		FbxArrayDelete<FbxString*>(history);
		FBXSDK_printf("********************************************************************************\n");
	}
	// importer �Ҹ�
	lImporter->Destroy();
	return lStatus;
}