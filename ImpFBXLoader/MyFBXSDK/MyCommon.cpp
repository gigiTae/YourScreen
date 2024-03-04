#include "MyCommon.h"

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(manager->GetIOSettings()))
#endif

void InitializeSdkObjects(FbxManager*& manager, FbxScene*& scene)
{
	// FBX 매니저를 초기화
	manager = FbxManager::Create();
	if (!manager)
	{
		FBXSDK_printf("오류: FBX 매니저를 만들 수 없음!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", manager->GetVersion());

	// IOSettings 객체 생성. 불러오기/내보내기 설정을 담당한다.
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	manager->LoadPluginsDirectory(lPath.Buffer());

	// FBX 씬 생성. 대부분 객체들의 가져오기/내보내기 를 담당한다.
	scene = FbxScene::Create(manager, "My Scene");
	if (!scene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}

void DestroySdkObjects(FbxManager* manager, bool exitStatus)
{
	// FBX 매니저 삭제. FBX 매니저가 생성한 모든 객체도 삭제
	if (manager) manager->Destroy();
	if (exitStatus) FBXSDK_printf("프로그램 성공!\n");
}

bool SaveScene(FbxManager* manager, FbxDocument* scene, const char* filename, int fileFormat, bool embedMedia)
{
	int lMajor, lMinor, lRevision;
	bool lStatus = true;

	// exporter 생성
	FbxExporter* lExporter = FbxExporter::Create(manager, "");
	if (fileFormat < 0 || fileFormat >= manager->GetIOPluginRegistry()->GetWriterFormatCount())
	{
		// ASCII 형식이 아니라면 대체 형식으로 쓰기
		fileFormat = manager->GetIOPluginRegistry()->GetNativeWriterFormat();

		// 가능하면 ASCII로 내보내도록 시도하기
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

	// 내보내기 설정. eEXPORT_TEXTURE_AS_EMBEDDED 옵션이 없는 한 기본값으로 모두 참 값임
	IOS_REF.SetBoolProp(EXP_FBX_MATERIAL, true);
	IOS_REF.SetBoolProp(EXP_FBX_TEXTURE, true);
	IOS_REF.SetBoolProp(EXP_FBX_EMBEDDED, embedMedia);
	IOS_REF.SetBoolProp(EXP_FBX_SHAPE, true);
	IOS_REF.SetBoolProp(EXP_FBX_GOBO, true);
	IOS_REF.SetBoolProp(EXP_FBX_ANIMATION, true);
	IOS_REF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	// filname 변수를 통해 exporter 초기화.
	if (lExporter->Initialize(filename, fileFormat, manager->GetIOSettings()) == false)
	{
		FBXSDK_printf("FbxExporter::Initialize() 호출 실패.\n");
		FBXSDK_printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
		return false;
	}
	FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
	FBXSDK_printf("FBX file format version %d.%d.%d\n\n", lMajor, lMinor, lRevision);

	// 씬 내보내기
	lStatus = lExporter->Export(scene);

	// exporter 소멸
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

	// 파일 버전 번호 생성
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// importer 생성
	FbxImporter* lImporter = FbxImporter::Create(manager, "");
	
	// filename 변수를 통해 importer 초기화
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
		
		// 여기서 모든 파일을 로딩하지 않고도 모든 animation stack 정보를 접근할 수 있다.
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
		
		// 내보내기 설정. 기본적으로 다 참값
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
	// importer 소멸
	lImporter->Destroy();
	return lStatus;
}