#include "FbxLoader.h"
#include "SceneLoader.h"

namespace FBXLoad
{

FbxLoader::FbxLoader()
    : _impAnimaObjLoader(std::make_unique<ImpAnimationObjLoader>())
{
}

FbxLoader::~FbxLoader()
{
}

void FbxLoader::Release()
{
    _impAnimaObjLoader->Release();
}

void FbxLoader::SetPath(const char* file)
{
    _impAnimaObjLoader->CreateAnimationObj(file);
}

ImpMesh* FbxLoader::GetMesh(size_t i)
{
    return _impAnimaObjLoader->GetMesh(i);
}

ImpStructure::ImpAnimationObject* FbxLoader::GetAnimationObject()
{
    return _impAnimaObjLoader->GetAnimationObject();
}

std::vector<ImpStructure::Bone*> FbxLoader::GetBoneList()
{
    return _impAnimaObjLoader->LoadBoneList();
}

//////////////////////////////////////////////////////////////////////

void FbxLoader::DisplayScene(const char* file)
{
    FbxManager* lSdkManager = FbxManager::Create();

    FbxImporter* lImporter{ FbxImporter::Create(lSdkManager, "Importer") };

    FbxScene* lScene = FbxScene::Create(lSdkManager, file);

    // y ,z, x
    //FbxAxisSystem now = lScene->GetGlobalSettings().GetAxisSystem();

    lScene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::eDirectX);

    // y ,z, -x
    //now = lScene->GetGlobalSettings().GetAxisSystem();

    lImporter->Initialize(file, -1, lSdkManager->GetIOSettings());

    lImporter->Import(lScene);

    FBXSDK_printf("파일 \"%s\"의 scene 생성\n", lScene->GetName());

    // Display the scene.

    // DisplayMetaData(lScene);

    /// 　빗금　세　개　주석은　없는　함수란　뜻

    /// FBXSDK_printf(
    ///    "\n\n---------------------\nGlobal Light "
    ///    "Settings\n---------------------\n\n");
    /// DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

    /// FBXSDK_printf(
    ///    "\n\n----------------------\nGlobal Camera "
    ///    "Settings\n----------------------\n\n");
    /// DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

    FBXSDK_printf(
        "\n\n--------------------\nGlobal Time "
        "Settings\n--------------------\n\n");
    DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

    // FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");
    // DisplayHierarchy(lScene);

    FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");
    // DisplayContent(lScene->GetRootNode());

    // FBXSDK_printf("\n\n----\nPose\n----\n\n");
    DisplayPose(lScene);

    FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");
    DisplayAnimation(lScene);

    // now display generic information

    /// FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
    /// DisplayGenericInfo(lScene);

    lImporter->Destroy();

    lScene->Destroy();

    lSdkManager->Destroy();
}

void FbxLoader::DisplayTransformations(const char* file)
{
    FbxManager* lSdkManager = FbxManager::Create();

    FbxImporter* lImporter{ FbxImporter::Create(lSdkManager, "Importer") };

    FbxScene* lScene = FbxScene::Create(lSdkManager, file);

    lImporter->Initialize(file, -1, lSdkManager->GetIOSettings());

    lImporter->Import(lScene);

    FBXSDK_printf("파일 \"%s\"의 scene 생성\n", lScene->GetName());

    CompareTransformations(lScene->GetRootNode(), lScene);

    lImporter->Destroy();

    lScene->Destroy();

    lSdkManager->Destroy();
}

void FbxLoader::DisplayMetaData(FbxScene* pScene)
{
    FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        FBXSDK_printf(
            "\n\n--------------------\nMeta-Data\n--------------------\n\n");
        FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());
        FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            FBXSDK_printf("    Thumbnail:\n");
            switch (thumbnail->GetDataFormat())
            {
                case FbxThumbnail::eRGB_24:
                    FBXSDK_printf("        Format: RGB\n");
                    break;
                case FbxThumbnail::eRGBA_32:
                    FBXSDK_printf("        Format: RGBA\n");
                    break;
            }
            switch (thumbnail->GetSize())
            {
                default:
                    break;
                case FbxThumbnail::eNotSet:
                    FBXSDK_printf(
                        "        Size: no dimensions specified (%ld bytes)\n",
                        thumbnail->GetSizeInBytes());
                    break;
                case FbxThumbnail::e64x64:
                    FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n",
                        thumbnail->GetSizeInBytes());
                    break;
                case FbxThumbnail::e128x128:
                    FBXSDK_printf(
                        "        Size: 128 x 128 pixels (%ld bytes)\n",
                        thumbnail->GetSizeInBytes());
            }
        }
    }
}

void FbxLoader::DisplayTransformPropagation(FbxNode* pNode)
{
    FBXSDK_printf("    Transformation Propagation\n");
    //
    // Rotation Space
    //
    EFbxRotationOrder lRotationOrder;
    pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);
    FBXSDK_printf("        Rotation Space: ");
    switch (lRotationOrder)
    {
        case eEulerXYZ:
            FBXSDK_printf("Euler XYZ\n");
            break;
        case eEulerXZY:
            FBXSDK_printf("Euler XZY\n");
            break;
        case eEulerYZX:
            FBXSDK_printf("Euler YZX\n");
            break;
        case eEulerYXZ:
            FBXSDK_printf("Euler YXZ\n");
            break;
        case eEulerZXY:
            FBXSDK_printf("Euler ZXY\n");
            break;
        case eEulerZYX:
            FBXSDK_printf("Euler ZYX\n");
            break;
        case eSphericXYZ:
            FBXSDK_printf("Spheric XYZ\n");
            break;
    }
    //
    // Use the Rotation space only for the limits
    // (keep using eEulerXYZ for the rest)
    //
    FBXSDK_printf(
        "        Use the Rotation Space for Limit specification only: %s\n",
        pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");
    //
    // Inherit Type
    //
    FbxTransform::EInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);
    FBXSDK_printf("        Transformation Inheritance: ");
    switch (lInheritType)
    {
        case FbxTransform::eInheritRrSs:
            FBXSDK_printf("RrSs\n");
            break;
        case FbxTransform::eInheritRSrs:
            FBXSDK_printf("RSrs\n");
            break;
        case FbxTransform::eInheritRrs:
            FBXSDK_printf("Rrs\n");
            break;
    }

    FBXSDK_printf("\n");
}

void FbxLoader::DisplayGeometricTransform(FbxNode* pNode)
{
    FbxVector4 lTmpVector;
    FBXSDK_printf("    Geometric Transformations\n");
    //
    // Translation
    //
    lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Translation: %f %f %f\n",
        lTmpVector[0],
        lTmpVector[1],
        lTmpVector[2]);
    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Rotation:    %f %f %f\n",
        lTmpVector[0],
        lTmpVector[1],
        lTmpVector[2]);
    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
    FBXSDK_printf("        Scaling:     %f %f %f\n",
        lTmpVector[0],
        lTmpVector[1],
        lTmpVector[2]);

    FBXSDK_printf("\n");
}
void DisplayMetaData(FbxScene* pScene)
{
    FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        FBXSDK_printf(
            "\n\n--------------------\nMeta-Data\n--------------------\n\n");
        FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());
        FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            FBXSDK_printf("    Thumbnail:\n");
            switch (thumbnail->GetDataFormat())
            {
                case FbxThumbnail::eRGB_24:
                    FBXSDK_printf("        Format: RGB\n");
                    break;
                case FbxThumbnail::eRGBA_32:
                    FBXSDK_printf("        Format: RGBA\n");
                    break;
            }
            switch (thumbnail->GetSize())
            {
                default:
                    break;
                case FbxThumbnail::eNotSet:
                    FBXSDK_printf(
                        "        Size: no dimensions specified (%ld bytes)\n",
                        thumbnail->GetSizeInBytes());
                    break;
                case FbxThumbnail::e64x64:
                    FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n",
                        thumbnail->GetSizeInBytes());
                    break;
                case FbxThumbnail::e128x128:
                    FBXSDK_printf(
                        "        Size: 128 x 128 pixels (%ld bytes)\n",
                        thumbnail->GetSizeInBytes());
            }
        }
    }
}

void FbxLoader::DisplayGlobalTimeSettings(FbxGlobalSettings* pGlobalSettings)
{
    char lTimeString[256];
    FBXSDK_printf(
        "Time Mode : %s\n", FbxGetTimeModeName(pGlobalSettings->GetTimeMode()));
    FbxTimeSpan lTs;
    FbxTime lStart, lEnd;
    pGlobalSettings->GetTimelineDefaultTimeSpan(lTs);
    lStart = lTs.GetStart();
    lEnd   = lTs.GetStop();
    FBXSDK_printf("Timeline default timespan: \n");
    FBXSDK_printf(
        "     Start: %s\n", lStart.GetTimeString(lTimeString, FbxUShort(256)));
    FBXSDK_printf(
        "     Stop : %s\n", lEnd.GetTimeString(lTimeString, FbxUShort(256)));
    FBXSDK_printf("");
}

void FbxLoader::DisplayHierarchy(FbxScene* pScene)
{
    int i;
    FbxNode* lRootNode = pScene->GetRootNode();
    for (i = 0; i < lRootNode->GetChildCount(); i++)
    {
        DisplayHierarchy(lRootNode->GetChild(i), 0);
    }
}

void FbxLoader::DisplayHierarchy(FbxNode* pNode, int pDepth)
{
    FbxString lString;
    int i;
    for (i = 0; i < pDepth; i++)
    {
        lString += "     ";
    }
    lString += pNode->GetName();
    FBXSDK_printf("%s\n", lString.Buffer());
    for (i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayHierarchy(pNode->GetChild(i), pDepth + 1);
    }
}

void FbxLoader::DisplayContent(FbxNode* pNode)
{
    FbxNodeAttribute::EType lAttributeType;
    if (pNode->GetNodeAttribute() == NULL)
    {
        FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());
        switch (lAttributeType)
        {
            default:
                break;
            case FbxNodeAttribute::eMarker:
                /// DisplayMarker(pNode);
                break;
            case FbxNodeAttribute::eSkeleton:
                DisplaySkeleton(pNode);
                break;
            case FbxNodeAttribute::eMesh:
                DisplayMesh(pNode);
                break;
            case FbxNodeAttribute::eNurbs:
                /// DisplayNurb(pNode);
                break;
            case FbxNodeAttribute::ePatch:
                /// DisplayPatch(pNode);
                break;
            case FbxNodeAttribute::eCamera:
                /// DisplayCamera(pNode);
                break;
            case FbxNodeAttribute::eLight:
                /// DisplayLight(pNode);
                break;
            case FbxNodeAttribute::eLODGroup:
                /// DisplayLodGroup(pNode);
                break;
        }
    }
    /// DisplayUserProperties(pNode);
    /// DisplayTarget(pNode);
    /// DisplayPivotsAndLimits(pNode);
    // DisplayTransformPropagation(pNode); // 도움 안됨
    // DisplayGeometricTransform(pNode); // 도움 안됨

    for (int i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayContent(pNode->GetChild(i));
    }
}

void FbxLoader::DisplayPose(FbxScene* pScene)
{
    int i, j, k, lPoseCount;
    FbxString lName;

    lPoseCount = pScene->GetPoseCount();

    FBXSDK_printf("Pose Count: %d\n", lPoseCount);

    FBXSDK_printf("\n");

    for (i = 0; i < lPoseCount; i++)
    {
        FbxPose* lPose = pScene->GetPose(i);

        lName = lPose->GetName();
        FBXSDK_printf("Pose Name: %s\n", lName.Buffer());

        FBXSDK_printf(
            "    Is a bind pose: %s\n", lPose->IsBindPose() ? "true" : "flase");

        FBXSDK_printf("    Number of items in the pose: %d\n", lPose->GetCount());

        FBXSDK_printf("\n");

        for (j = 0; j < lPose->GetCount(); j++)
        {
            lName = lPose->GetNodeName(j).GetCurrentName();
            FBXSDK_printf("    Item name: %s\n", lName.Buffer());

            if (!lPose->IsBindPose())
            {
                // Rest pose can have local matrix
                FBXSDK_printf("    Is local space matrix: %s\n",
                    lPose->IsLocalMatrix(j) ? "true" : "false");
            }
            FBXSDK_printf("    Is local space matrix: %s\n",
                lPose->IsLocalMatrix(j) ? "true" : "false");

            FBXSDK_printf("    Matrix value: \n");

            FbxString lMatrixValue;

            for (k = 0; k < 4; k++)
            {
                FbxMatrix lMatrix = lPose->GetMatrix(j);
                FbxVector4 lRow   = lMatrix.GetRow(k);
                char lRowValue[1024];

                FBXSDK_sprintf(lRowValue,
                    1024,
                    "%9.4f %9.4f %9.4f %9.4f\n",
                    lRow[0],
                    lRow[1],
                    lRow[2],
                    lRow[3]);
                lMatrixValue += FbxString("        ") + FbxString(lRowValue);
            }

            FBXSDK_printf(lMatrixValue.Buffer());

            FBXSDK_printf("\n");
        }
    }

    lPoseCount = pScene->GetCharacterPoseCount();

    FBXSDK_printf("Character Pose Count: %d\n", lPoseCount);

    for (i = 0; i < lPoseCount; i++)
    {
        FbxCharacterPose* lPose  = pScene->GetCharacterPose(i);
        FbxCharacter* lCharacter = lPose->GetCharacter();

        if (!lCharacter) break;

        FBXSDK_printf("Character Pose Name: %s", lCharacter->GetName());

        FbxCharacterLink lCharacterLink;
        FbxCharacter::ENodeId lNodeId = FbxCharacter::eHips;

        while (lCharacter->GetCharacterLink(lNodeId, &lCharacterLink))
        {
            FbxAMatrix& lGlobalPosition = lCharacterLink.mNode->EvaluateGlobalTransform(
                FBXSDK_TIME_ZERO);

            FBXSDK_printf("    Matrix value: \n");

            FbxString lMatrixValue;

            for (k = 0; k < 4; k++)
            {
                FbxVector4 lRow = lGlobalPosition.GetRow(k);
                char lRowValue[1024];

                FBXSDK_sprintf(lRowValue,
                    1024,
                    "%9.4f %9.4f %9.4f %9.4f\n",
                    lRow[0],
                    lRow[1],
                    lRow[2],
                    lRow[3]);
                lMatrixValue += FbxString("        ") + FbxString(lRowValue);
            }

            FBXSDK_printf(lMatrixValue.Buffer());

            lNodeId = FbxCharacter::ENodeId(int(lNodeId) + 1);
        }
    }
}

void FbxLoader::DisplayAnimation(FbxScene* pScene)
{
    unsigned int objectCount = pScene->GetSrcObjectCount<FbxAnimStack>();
    FBXSDK_printf("Anim Stack Count: %d\n", objectCount);

    for (unsigned int i = 0; i < objectCount; i++)
    {
        FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

        FBXSDK_printf(
            "Animation Stack %d Name: %s\n", i, (char*)lAnimStack->GetName());

        int nbAnimLayers  = lAnimStack->GetMemberCount<FbxAnimLayer>();
        int nbAudioLayers = lAnimStack->GetMemberCount<FbxAudioLayer>();
        FbxString lOutputString;

        lOutputString = "   contains ";
        if (nbAnimLayers == 0 && nbAudioLayers == 0)
            lOutputString += "no layers";

        if (nbAnimLayers)
        {
            lOutputString += nbAnimLayers;
            lOutputString += " Animation Layer";
            if (nbAnimLayers > 1) lOutputString += "s";
        }

        if (nbAudioLayers)
        {
            if (nbAnimLayers) lOutputString += " and ";

            lOutputString += nbAudioLayers;
            lOutputString += " Audio Layer";
            if (nbAudioLayers > 1) lOutputString += "s";
        }
        lOutputString += "\n\n";
        FBXSDK_printf(lOutputString);

        for (int l = 0; l < lAnimStack->GetMemberCount<FbxAnimLayer>(); l++)
        {
            lOutputString = "AnimLayer ";
            lOutputString += l;
            lOutputString += "\n";
            FBXSDK_printf(lOutputString);

            FbxAnimLayer* pAnimLayer = lAnimStack->GetMember<FbxAnimLayer>(l);



            if (pAnimLayer == nullptr) continue;
            DisplayAnimation(pScene->GetRootNode(), pAnimLayer);
        }
    }
}

void FbxLoader::DisplayAnimation(FbxNode* pNode, FbxAnimLayer* pAnimLayer)
{
    // int lModelCount;
    FbxString lOutputString;

    FBXSDK_printf("     Node Name: %s\n", pNode->GetName());

    FbxAnimCurve* lAnimCurve = NULL;

    lAnimCurve = pNode->LclTranslation.GetCurve(
        pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (lAnimCurve)
    {
        FBXSDK_printf("        TX\n");
        DisplayCurveKeys(lAnimCurve);
    }
    lAnimCurve = pNode->LclTranslation.GetCurve(
        pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (lAnimCurve)
    {
        FBXSDK_printf("        TY\n");
        DisplayCurveKeys(lAnimCurve);
    }
    lAnimCurve = pNode->LclTranslation.GetCurve(
        pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (lAnimCurve)
    {
        FBXSDK_printf("        TZ\n");
        DisplayCurveKeys(lAnimCurve);
    }

    lAnimCurve = pNode->LclRotation.GetCurve(
        pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (lAnimCurve)
    {
        FBXSDK_printf("        RX\n");
        DisplayCurveKeys(lAnimCurve);
    }
    lAnimCurve = pNode->LclRotation.GetCurve(
        pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (lAnimCurve)
    {
        FBXSDK_printf("        RY\n");
        DisplayCurveKeys(lAnimCurve);
    }
    lAnimCurve = pNode->LclRotation.GetCurve(
        pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (lAnimCurve)
    {
        FBXSDK_printf("        RZ\n");
        DisplayCurveKeys(lAnimCurve);
    }

    lAnimCurve = pNode->LclScaling.GetCurve(
        pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (lAnimCurve)
    {
        FBXSDK_printf("        SX\n");
        DisplayCurveKeys(lAnimCurve);
    }
    lAnimCurve = pNode->LclScaling.GetCurve(
        pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (lAnimCurve)
    {
        FBXSDK_printf("        SY\n");
        DisplayCurveKeys(lAnimCurve);
    }
    lAnimCurve = pNode->LclScaling.GetCurve(
        pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (lAnimCurve)
    {
        FBXSDK_printf("        SZ\n");
        DisplayCurveKeys(lAnimCurve);
    }
    FBXSDK_printf("\n");

    auto a = pNode->GetChildCount();
    for (int i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayAnimation(pNode->GetChild(i), pAnimLayer);
    }
}

void FbxLoader::DisplayCurveKeys(FbxAnimCurve* pCurve)
{
    FbxTime lKeyTime;
    float lKeyValue;
    char lTimeString[256];
    FbxString lOutputString;
    int lCount;

    int lKeyCount = pCurve->KeyGetCount();

    for (lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += "\n";
        FBXSDK_printf(lOutputString);
    }
}

void FbxLoader::DisplaySkeleton(FbxNode* pNode)
{
    FbxSkeleton* lSkeleton = (FbxSkeleton*)pNode->GetNodeAttribute();

    FBXSDK_printf("Skeleton Name: %s\n", (char*)pNode->GetName());

    int nbMetaData = lSkeleton->GetSrcObjectCount<FbxObjectMetaData>();
    if (nbMetaData > 0) FBXSDK_printf("    MetaData connections ");

    for (int i = 0; i < nbMetaData; i++)
    {
        FbxObjectMetaData* metaData = lSkeleton->GetSrcObject<FbxObjectMetaData>(i);
        FBXSDK_printf("        Name: %s\n", (char*)metaData->GetName());
    }

    const char* lSkeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };

    FBXSDK_printf("    Type: %s\n", lSkeletonTypes[lSkeleton->GetSkeletonType()]);

    if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimb)
    {
        FBXSDK_printf("    Limb Length: %f\n", lSkeleton->LimbLength.Get());
    }
    else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
    {
        FBXSDK_printf("    Limb Node Size: %f\n", lSkeleton->Size.Get());
    }
    else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eRoot)
    {
        FBXSDK_printf("    Limb Root Size: %f\n", lSkeleton->Size.Get());
    }

    auto color = lSkeleton->GetLimbNodeColor();
    FBXSDK_printf("    Color: %f, %f, %f\n", color[0], color[1], color[2]);

    FBXSDK_printf("\n");
}

void FbxLoader::DisplayMesh(FbxNode* pNode)
{
    FbxMesh* fbxMesh = (FbxMesh*)pNode->GetNodeAttribute();

    FBXSDK_printf("Mesh Name: %s\n", (char*)pNode->GetName());

    // LoadControlsPoints(fbxMesh);
    // DisplayPolygons(fbxMesh);
    // DisplayMaterialMapping(mesh);
    // DisplayTexture(mesh);
    // DisplayMaterial(lMesh);
    // DisplayMaterialConnections(lMesh);
    DisplayLink(fbxMesh);
    // DisplayShape(lMesh);
    // DisplayCache(lMesh);

    // 다 사용한 Fbx Mesh  제거
    fbxMesh->Destroy();
}

void FbxLoader::DisplayPolygons(FbxMesh* pMesh)
{
    int i, j, polygonCount = pMesh->GetPolygonCount();
    FbxVector4* controlPoints = pMesh->GetControlPoints();
    char header[100];

    FBXSDK_printf("    Polygons\n");

    int vertexId{ 0 };

    for (i = 0; i < polygonCount; i++)
    {
        FBXSDK_printf("        Polygon %d\n", i);
        int l;

        for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
        {
            FbxGeometryElementPolygonGroup* ePolgrp = pMesh->GetElementPolygonGroup(
                l);
            switch (ePolgrp->GetMappingMode())
            {
                case FbxGeometryElement::eByPolygon:
                    if (ePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
                    {
                        FBXSDK_sprintf(
                            header, 100, "        Assigned to group: ");
                        int polyGroupId = ePolgrp->GetIndexArray().GetAt(i);
                        FBXSDK_printf("%s %d\n", header, polyGroupId);
                    }
                    break;
                default:
                    // any other mapping modes don't make sense
                    FBXSDK_printf("        \"unsupported group assignment\"\n");
                    break;
            }
        }

        // 폴리곤의 사이즈는 무조건 3 이다.
        int polygonSize = pMesh->GetPolygonSize(i);

        for (j = 0; j < polygonSize; j++)
        {
            //
            // 정점의 ControlPoint index 를 가져온다.
            //

            FBXSDK_printf("            Vertex ID: %d\n", vertexId);

            /// ----------------
            /// Vertex Position
            /// ----------------

            // 포지션을 담는 변수를 준비한다.
            FbxVector4 localPosition;

            int controlPointIndex = pMesh->GetPolygonVertex(i, j);
            if (controlPointIndex < 0)
            {
                FBXSDK_printf(
                    "            Coordinates: Invalid index found!\n");
                continue;
            }
            else
            {
                localPosition = controlPoints[controlPointIndex];
                FBXSDK_printf("            Coordinates: %f, %f, %f\n",
                    localPosition[0],
                    localPosition[1],
                    localPosition[2]);
            }

            /// --------------
            /// Vertex Color
            /// --------------

            // 컬러값을 담는 변수를 준비한다.
            FbxColor vertexColor;

            int eColorCount = pMesh->GetElementVertexColorCount();


            for (l = 0; l < eColorCount; l++)
            {
                FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(
                    l);
                FBXSDK_sprintf(header, 100, "            Color vertex: ");

                switch (leVtxc->GetMappingMode())
                {
                    default:
                        break;
                    case FbxGeometryElement::eByControlPoint:
                        switch (leVtxc->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            {
                                vertexColor = leVtxc->GetDirectArray().GetAt(
                                    controlPointIndex);
                                FBXSDK_printf("%s %f, %f, %f, %f\n",
                                    header,
                                    vertexColor[0],
                                    vertexColor[1],
                                    vertexColor[2],
                                    vertexColor[3]);
                            }
                            break;
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                int id = leVtxc->GetIndexArray().GetAt(
                                    controlPointIndex);
                                vertexColor = leVtxc->GetDirectArray().GetAt(id);
                                FBXSDK_printf("%s %f, %f, %f, %f\n",
                                    header,
                                    vertexColor[0],
                                    vertexColor[1],
                                    vertexColor[2],
                                    vertexColor[3]);
                            }
                            break;
                            default:
                                break; // other reference modes not shown here!
                        }
                        break;

                    case FbxGeometryElement::eByPolygonVertex:
                    {
                        switch (leVtxc->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            {
                                vertexColor = leVtxc->GetDirectArray().GetAt(
                                    vertexId);
                                FBXSDK_printf("%s %f, %f, %f, %f\n",
                                    header,
                                    vertexColor[0],
                                    vertexColor[1],
                                    vertexColor[2],
                                    vertexColor[3]);
                            }
                            break;
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                int id = leVtxc->GetIndexArray().GetAt(vertexId);
                                vertexColor = leVtxc->GetDirectArray().GetAt(id);
                                FBXSDK_printf("%s %f, %f, %f, %f\n",
                                    header,
                                    vertexColor[0],
                                    vertexColor[1],
                                    vertexColor[2],
                                    vertexColor[3]);
                            }
                            break;
                            default:
                                break; // other reference modes not shown here!
                        }
                    }
                    break;

                    case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
                    case FbxGeometryElement::eAllSame: // doesn't make much sense for UVs
                    case FbxGeometryElement::eNone: // doesn't make much sense for UVs
                        break;
                }
            } // for eColorCount

            /// ----
            /// UV
            /// ----

            // UV 데이터를 담는 변수를 준비한다.
            FbxVector2 uv;

            for (l = 0; l < pMesh->GetElementUVCount(); ++l)
            {
                FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
                FBXSDK_sprintf(header, 100, "            Texture UV: ");

                switch (leUV->GetMappingMode())
                {
                    default:
                        break;
                    case FbxGeometryElement::eByControlPoint:
                        switch (leUV->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            {
                                uv = leUV->GetDirectArray().GetAt(
                                    controlPointIndex);
                                FBXSDK_printf("%s %f, %f\n", header, uv[0], uv[1]);
                            }
                            break;
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                int id = leUV->GetIndexArray().GetAt(
                                    controlPointIndex);
                                uv = leUV->GetDirectArray().GetAt(id);
                                FBXSDK_printf("%s %f, %f\n",
                                    header,
                                    leUV->GetDirectArray().GetAt(id)[0],
                                    leUV->GetDirectArray().GetAt(id)[1]);
                            }
                            break;
                            default:
                                break; // other reference modes not shown here!
                        }
                        break;

                    case FbxGeometryElement::eByPolygonVertex:
                    {
                        int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
                        switch (leUV->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                uv = leUV->GetDirectArray().GetAt(lTextureUVIndex);
                                FBXSDK_printf("%s %f, %f\n", header, uv[0], uv[1]);
                            }
                            break;
                            default:
                                break; // other reference modes not shown here!
                        }
                    }
                    break;

                    case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
                    case FbxGeometryElement::eAllSame: // doesn't make much sense for UVs
                    case FbxGeometryElement::eNone: // doesn't make much sense for UVs
                        break;
                }
            } // for PolygonSize

            /// --------------
            /// Vertex Normal
            /// --------------

            // 노말 값을 담는 변수를 미리 준비한다.
            FbxVector4 normal;

            int normalCount = pMesh->GetElementNormalCount();

            for (l = 0; l < normalCount; ++l)
            {
                FbxGeometryElementNormal* eNormal = pMesh->GetElementNormal(l);
                FBXSDK_sprintf(header, 100, "            Normal: ");

                if (eNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                {
                    switch (eNormal->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        {
                            normal = eNormal->GetDirectArray().GetAt(vertexId);
                            FBXSDK_printf("%s %f, %f, %f\n",
                                header,
                                normal[0],
                                normal[1],
                                normal[2]);
                            int a = 3;
                        }
                        break;
                        case FbxGeometryElement::eIndexToDirect:
                        {
                            int id = eNormal->GetIndexArray().GetAt(vertexId);
                            normal = eNormal->GetDirectArray().GetAt(id);
                            FBXSDK_printf("%s %f, %f, %f\n",
                                header,
                                normal[0],
                                normal[1],
                                normal[2]);
                        }
                        break;
                        default:
                            break; // other reference modes not shown here!
                    }
                } // for normalCount
            }

            /// ---------
            /// Tangent
            /// ---------

            // 탄젠트 값을 담는 변수를 준비한다.
            FbxVector4 tangent;

            int tangentCount = pMesh->GetElementTangentCount();

            for (l = 0; l < tangentCount; ++l)
            {
                FbxGeometryElementTangent* eTangent = pMesh->GetElementTangent(l);
                FBXSDK_sprintf(header, 100, "            Tangent: ");

                if (eTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                {
                    switch (eTangent->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        {
                            tangent = eTangent->GetDirectArray().GetAt(vertexId);
                            FBXSDK_printf("%s %f, %f, %f\n",
                                header,
                                tangent[0],
                                tangent[1],
                                tangent[2]);
                        }
                        break;
                        case FbxGeometryElement::eIndexToDirect:
                        {
                            int id  = eTangent->GetIndexArray().GetAt(vertexId);
                            tangent = eTangent->GetDirectArray().GetAt(id);
                            FBXSDK_printf("%s %f, %f, %f\n",
                                header,
                                tangent[0],
                                tangent[1],
                                tangent[2]);
                        }
                        break;
                        default:
                            break; // other reference modes not shown here!
                    }
                }
            } // for tangentCount

            /// ---------
            /// Binormal
            /// ---------

            // 바이노말 값을 담는 변수를 준비한다.

            FbxVector4 bitangent;

            for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
            {
                FbxGeometryElementBinormal* eBinormal = pMesh->GetElementBinormal(
                    l);

                FBXSDK_sprintf(header, 100, "            Binormal: ");
                if (eBinormal->GetMappingMode() ==
                    FbxGeometryElement::eByPolygonVertex)
                {
                    switch (eBinormal->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        {
                            bitangent = eBinormal->GetDirectArray().GetAt(vertexId);
                            FBXSDK_printf("%s %f, %f, %f\n",
                                header,
                                bitangent[0],
                                bitangent[1],
                                bitangent[2]);
                        }
                        break;
                        case FbxGeometryElement::eIndexToDirect:
                        {
                            int id = eBinormal->GetIndexArray().GetAt(vertexId);
                            bitangent = eBinormal->GetDirectArray().GetAt(id);
                            FBXSDK_printf("%s %f, %f, %f\n",
                                header,
                                bitangent[0],
                                bitangent[1],
                                bitangent[2]);
                        }
                        break;
                        default:
                            break; // other reference modes not shown here!
                    }
                }
            } // for Binormal Count

            FBXSDK_printf("\n");
            vertexId++;
        } // for polygonSize
    }     // for polygonCount

    // check visibility for the edges of the mesh
    for (int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
    {
        FbxGeometryElementVisibility* leVisibility = pMesh->GetElementVisibility(l);
        FBXSDK_sprintf(header, 100, "    Edge Visibility : ");
        // DisplayString(header);
        switch (leVisibility->GetMappingMode())
        {
            default:
                break;
                // should be eByEdge
            case FbxGeometryElement::eByEdge:
                // should be eDirect
                for (j = 0; j != pMesh->GetMeshEdgeCount(); ++j)
                {
                    FBXSDK_printf("        Edge %d\n", j);
                    FBXSDK_printf("              Edge visibility: %s\n",
                        leVisibility->GetDirectArray().GetAt(j) ?
                            "tr"
                            "ue" :
                            "fa"
                            "ls"
                            "e");
                }

                break;
        }
    }
}

void FbxLoader::DisplayLink(FbxMesh* pMesh)
{
    int i, j;
    int lSkinCount    = 0;
    int lClusterCount = 0;
    FbxCluster* lCluster;

    lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

    // lLinkCount = pGeometry->GetLinkCount();
    for (i = 0; i != lSkinCount; ++i)
    {
        lClusterCount = ((FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin))
                            ->GetClusterCount();

        FBXSDK_printf("  Skin %d\n", i);

        for (j = 0; j != lClusterCount; ++j)
        {
            FBXSDK_printf("    Cluster %d\n", j);

            lCluster = ((FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin))
                           ->GetCluster(j);
            // lLink = pGeometry->GetLink(i);

            const char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

            FBXSDK_printf("    Mode: %s\n", lClusterModes[lCluster->GetLinkMode()]);

            if (lCluster->GetLink() != NULL)
            {
                FBXSDK_printf("        Name: %s\n",
                    (char*)lCluster->GetLink()->GetName());
            }

            FbxString lString1 = "        Link Indices: ";
            FbxString lString2 = "        Weight Values: ";

            int k, lIndexCount = lCluster->GetControlPointIndicesCount();
            int* lIndices    = lCluster->GetControlPointIndices();
            double* lWeights = lCluster->GetControlPointWeights();

            for (k = 0; k < lIndexCount; k++)
            {
                lString1 += lIndices[k];
                lString2 += (float)lWeights[k];

                // FBXSDK_printf("( %d: %f )   ", lIndices[k], (float)lWeights[k]);

                if (k < lIndexCount - 1)
                {
                    lString1 += ", ";
                    lString2 += ", ";
                }
            }

            lString1 += "\n";
            lString2 += "\n";

            FBXSDK_printf(lString1);
            FBXSDK_printf(lString2);

            FBXSDK_printf("\n");

            FbxAMatrix lMatrix;

            FBXSDK_printf("    Matrix: \n");

            lMatrix = lCluster->GetTransformMatrix(lMatrix);
            auto t  = lMatrix.GetT();
            FBXSDK_printf(
                "        Transform Translation: %f, %f, %f\n", t[0], t[1], t[2]);

            auto r = lMatrix.GetR();
            FBXSDK_printf(
                "        Transform Rotation: %f, %f, %f\n", r[0], r[1], r[2]);

            auto s = lMatrix.GetS();
            FBXSDK_printf(
                "        Transform Scaling: %f, %f, %f\n", s[0], s[1], s[2]);

            FBXSDK_printf("    Link Matrix: \n");

            lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
            auto tt = lMatrix.GetT();
            FBXSDK_printf(
                "        Transform Translation: %f, %f, %f\n", tt[0], tt[1], tt[2]);

            auto rr = lMatrix.GetR();
            FBXSDK_printf(
                "        Transform Rotation: %f, %f, %f\n", rr[0], rr[1], rr[2]);

            auto ss = lMatrix.GetS();
            FBXSDK_printf(
                "        Transform Scaling: %f, %f, %f\n", ss[0], ss[1], ss[2]);

            if (lCluster->GetAssociateModel() != NULL)
            {
                lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
                FBXSDK_printf("        Associate Model: %s\n",
                    (char*)lCluster->GetAssociateModel()->GetName());

                auto t = lMatrix.GetT();
                FBXSDK_printf(
                    "        Associate Model Translation: %f, %f, %f\n",
                    t[0],
                    t[1],
                    t[2]);
                auto r = lMatrix.GetR();
                FBXSDK_printf("        Associate Model Rotation: %f, %f, %f\n",
                    r[0],
                    r[1],
                    r[2]);
                auto s = lMatrix.GetS();
                FBXSDK_printf("        Associate Model Scaling: %f, %f, %f\n",
                    s[0],
                    s[1],
                    s[2]);
            }

            FBXSDK_printf("\n");
        }
    }
}

void FbxLoader::CompareTransformations(FbxNode* pNode, FbxScene* pScene)
{
    if (pNode != pScene->GetRootNode())
    {
        FBXSDK_printf("%s\n", pNode->GetName());
        FbxNode* lParentNode = pNode->GetParent();
        // The first way: calculate global and local transform by
        // EvaluateGlobalTransform() and EvaluateLocalTransform().
        FbxAMatrix lGlobal, lLocal;
        lGlobal = pNode->EvaluateGlobalTransform();
        lLocal  = pNode->EvaluateLocalTransform();
        // The second way: calculate global and local transform from scratch by the node's properties.
        FbxAMatrix lParentTransform, lLocalTransform, lGlobalTransform;
        // temp
        FbxAMatrix linverse;
        lGlobalTransform = CalculateGlobalTransform(pNode);
        if (lParentNode)
        {
            // Get parent global transform.
            lParentTransform = CalculateGlobalTransform(lParentNode);
            // Calculate local transform according to: LocalTransform =
            // ParentGlobalInverse * GlobalTransform.

            linverse        = lParentTransform.Inverse();
            lLocalTransform = lParentTransform.Inverse() * lGlobalTransform;
        }
        else
            lLocalTransform = lGlobalTransform;
        // Compare, the results are the same. Display the global and local transformation of each joint.
        if (lGlobal == lGlobalTransform)
        {
            for (int i = 0; i < 4; ++i)
            {
                FbxString lHeader("GlobalTransform Row_");
                FbxString lIndex(i);
                lHeader += lIndex;
                lHeader += ": ";
                auto row = lGlobal.GetRow(i);
                FBXSDK_printf("%s%9.4f  %9.4f  %9.4f  %9.4f\n",
                    lHeader.Buffer(),
                    row[0],
                    row[1],
                    row[2],
                    row[3]);
            }
            FBXSDK_printf("\n");
        }
        else
        {
            FBXSDK_printf(
                "Error: The two global transform results are not equal!\n");
            for (int i = 0; i < 4; ++i)
            {
                FbxString lHeader("KFbxEvaluatorGlobalTransform Row_");
                FbxString lIndex(i);
                lHeader += lIndex;
                lHeader += ": ";
                auto row = lGlobal.GetRow(i);
                FBXSDK_printf("%s%9.4f  %9.4f  %9.4f  %9.4f\n",
                    lHeader.Buffer(),
                    row[0],
                    row[1],
                    row[2],
                    row[3]);
            }
            FBXSDK_printf("\n");
            for (int i = 0; i < 4; ++i)
            {
                FbxString lHeader("FromScratchGlobalTransform Row_");
                FbxString lIndex(i);
                lHeader += lIndex;
                lHeader += ": ";
                auto row = lGlobalTransform.GetRow(i);
                FBXSDK_printf("%s%9.4f  %9.4f  %9.4f  %9.4f\n",
                    lHeader.Buffer(),
                    row[0],
                    row[1],
                    row[2],
                    row[3]);
            }
            FBXSDK_printf("\n");
        }
        if (lLocal == lLocalTransform)
        {
            for (int i = 0; i < 4; ++i)
            {
                FbxString lHeader("LocalTransform Row_");
                FbxString lIndex(i);
                lHeader += lIndex;
                lHeader += ": ";
                auto row = lLocal.GetRow(i);
                FBXSDK_printf("%s%9.4f  %9.4f  %9.4f  %9.4f\n",
                    lHeader.Buffer(),
                    row[0],
                    row[1],
                    row[2],
                    row[3]);
            }
            FBXSDK_printf("\n");
        }
        else
        {
            FBXSDK_printf(
                "Error: The two local transform results are not equal!\n");
            for (int i = 0; i < 4; ++i)
            {
                FbxString lHeader("KFbxEvaluatorLocalTransform Row_");
                FbxString lIndex(i);
                lHeader += lIndex;
                lHeader += ": ";
                auto row = lLocal.GetRow(i);
                FBXSDK_printf("%s %9.4f  %9.4f  %9.4f  %9.4f\n",
                    lHeader.Buffer(),
                    row[0],
                    row[1],
                    row[2],
                    row[3]);
            }
            FBXSDK_printf("\n");
            for (int i = 0; i < 4; ++i)
            {
                FbxString lHeader("FromScratchLocalTransform Row_");
                FbxString lIndex(i);
                lHeader += lIndex;
                lHeader += ": ";
                auto row = lLocalTransform.GetRow(i);
                FBXSDK_printf("%s %9.4f  %9.4f  %9.4f  %9.4f\n",
                    lHeader.Buffer(),
                    row[0],
                    row[1],
                    row[2],
                    row[3]);
            }
            FBXSDK_printf("\n");
        }
    }
    int lChildCount = pNode->GetChildCount();
    for (int i = 0; i < lChildCount; i++)
    {
        CompareTransformations(pNode->GetChild(i), pScene);
    }
}

FbxAMatrix FbxLoader::CalculateGlobalTransform(FbxNode* pNode)
{
    FbxAMatrix lTranlationM, lScalingM, lScalingPivotM, lScalingOffsetM,
        lRotationOffsetM, lRotationPivotM, lPreRotationM, lRotationM,
        lPostRotationM, lTransform;
    FbxAMatrix lParentGX, lGlobalT, lGlobalRS;
    if (!pNode)
    {
        lTransform.SetIdentity();
        return lTransform;
    }
    // Construct translation matrix
    FbxVector4 lTranslation = pNode->LclTranslation.Get();
    lTranlationM.SetT(lTranslation);
    // Construct rotation matrices
    FbxVector4 lRotation     = pNode->LclRotation.Get();
    FbxVector4 lPreRotation  = pNode->PreRotation.Get();
    FbxVector4 lPostRotation = pNode->PostRotation.Get();
    lRotationM.SetR(lRotation);
    lPreRotationM.SetR(lPreRotation);
    lPostRotationM.SetR(lPostRotation);
    // Construct scaling matrix
    FbxVector4 lScaling = pNode->LclScaling.Get();
    lScalingM.SetS(lScaling);
    // Construct offset and pivot matrices
    FbxVector4 lScalingOffset  = pNode->ScalingOffset.Get();
    FbxVector4 lScalingPivot   = pNode->ScalingPivot.Get();
    FbxVector4 lRotationOffset = pNode->RotationOffset.Get();
    FbxVector4 lRotationPivot  = pNode->RotationPivot.Get();
    lScalingOffsetM.SetT(lScalingOffset);
    lScalingPivotM.SetT(lScalingPivot);
    lRotationOffsetM.SetT(lRotationOffset);
    lRotationPivotM.SetT(lRotationPivot);
    // Calculate the global transform matrix of the parent node
    FbxNode* lParentNode = pNode->GetParent();
    if (lParentNode)
    {
        lParentGX = CalculateGlobalTransform(lParentNode);
    }
    else
    {
        lParentGX.SetIdentity();
    }
    // Construct Global Rotation
    FbxAMatrix lLRM, lParentGRM;
    FbxVector4 lParentGR = lParentGX.GetR();
    lParentGRM.SetR(lParentGR);
    lLRM = lPreRotationM * lRotationM * lPostRotationM;
    // Construct Global Shear*Scaling
    // FBX SDK does not support shear, to patch this, we use:
    // Shear*Scaling = RotationMatrix.Inverse * TranslationMatrix.Inverse * WholeTranformMatrix
    FbxAMatrix lLSM, lParentGSM, lParentGRSM, lParentTM;
    FbxVector4 lParentGT = lParentGX.GetT();
    lParentTM.SetT(lParentGT);
    lParentGRSM = lParentTM.Inverse() * lParentGX;
    lParentGSM  = lParentGRM.Inverse() * lParentGRSM;
    lLSM        = lScalingM;
    // Do not consider translation now
    FbxTransform::EInheritType lInheritType = pNode->InheritType.Get();
    if (lInheritType == FbxTransform::eInheritRrSs)
    {
        lGlobalRS = lParentGRM * lLRM * lParentGSM * lLSM;
    }
    else if (lInheritType == FbxTransform::eInheritRSrs)
    {
        lGlobalRS = lParentGRM * lParentGSM * lLRM * lLSM;
    }
    else if (lInheritType == FbxTransform::eInheritRrs)
    {
        FbxAMatrix lParentLSM;
        FbxVector4 lParentLS = lParentNode->LclScaling.Get();
        lParentLSM.SetS(lParentLS);
        FbxAMatrix lParentGSM_noLocal = lParentGSM * lParentLSM.Inverse();
        lGlobalRS = lParentGRM * lLRM * lParentGSM_noLocal * lLSM;
    }
    else
    {
        FBXSDK_printf("error, unknown inherit type! \n");
    }
    // Construct translation matrix
    // Calculate the local transform matrix
    lTransform = lTranlationM * lRotationOffsetM * lRotationPivotM *
                 lPreRotationM * lRotationM * lPostRotationM *
                 lRotationPivotM.Inverse() * lScalingOffsetM * lScalingPivotM *
                 lScalingM * lScalingPivotM.Inverse();
    FbxVector4 lLocalTWithAllPivotAndOffsetInfo = lTransform.GetT();
    // Calculate global translation vector according to:
    // GlobalTranslation = ParentGlobalTransform * LocalTranslationWithPivotAndOffsetInfo
    FbxVector4 lGlobalTranslation = lParentGX.MultT(
        lLocalTWithAllPivotAndOffsetInfo);
    lGlobalT.SetT(lGlobalTranslation);
    // Construct the whole global transform
    lTransform = lGlobalT * lGlobalRS;
    return lTransform;
}

} // namespace FBXLoad