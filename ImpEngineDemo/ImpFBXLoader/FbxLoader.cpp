#include "FbxLoader.h"

#include <assert.h>

FBXLoad::FbxLoader::FbxLoader()
    : _manager(FbxManager::Create())
    , _meshLoader(std::make_unique<MeshLoader>(_manager))
    , _animationLoader(std::make_unique<AnimationLoader>(_manager))
    , _posLoader{ std::make_unique<PoseLoader>() }
    , _skeletonLoader{ std::make_unique<SkeletonLoader>() }
{
    if (!_manager)
    {
        FBXSDK_printf("오류: 《FbxManager》를 만들 수 없음.\n\n");
        exit(1);
    }
    else
        FBXSDK_printf("\n>> Autodesk FBX SDK 버전: \"%s\"\n",
                      _manager->GetVersion());

    // IO 설정 객체 생성 및 초기화
    FbxIOSettings* ios{ FbxIOSettings::Create(_manager, IOSROOT) };
    _manager->SetIOSettings(ios);
}

FBXLoad::FbxLoader::~FbxLoader()
{
    if (_scene)
        _scene->Destroy();
    if (_manager)
        _manager->Destroy();
}

#pragma region Interface

ImpMesh* FBXLoad::FbxLoader::GetMesh(const char* file, size_t i)
{
    LoadMesh(file);
    return _meshLoader->GetMesh(i);
}

std::vector<ImpMesh*> FBXLoad::FbxLoader::GetMeshAll(const char* file)
{
    LoadMesh(file);
    return _meshLoader->GetMeshAll();
}

size_t FBXLoad::FbxLoader::GetMeshSize(const char* file)
{
    LoadMesh(file);
    return _meshLoader->GetMeshSize();
}

ImpStructure::ImpAnimationObject* FBXLoad::FbxLoader::GetAnimationObject(const char* file)
{
    return LoadAnimationObject(file);
}

std::vector<ImpStructure::Bone*> FBXLoad::FbxLoader::GetBoneList()
{
    // LoadAnimation(file);
    return _animationLoader->GetBoneList();
}

#pragma endregion Interface

//////////////////////////////////////////////////////////////////////

#pragma region CreateScene

bool FBXLoad::FbxLoader::CreateFbxScene(const char* file)
{
    // importer 생성
    FbxImporter* fbxImporter{ FbxImporter::Create(_manager, "Importer") };

    // 해당 파일이 없으면 다음과 같은 오류 발생
    if (!fbxImporter->Initialize(file, -1, _manager->GetIOSettings()))
    {
        FBXSDK_printf("%s 파일을 여는데 실패했습니다.\n\n", file);
        FBXSDK_printf("오류 보고: %s\n\n",
                      fbxImporter->GetStatus().GetErrorString());
        exit(-1);
        return false;
    }

    if (_scene)
    {
        _scene->Destroy();
    }

    _scene = FbxScene::Create(_manager, "Importer Scene");
    FBXSDK_printf("Create Scene: \" %s \"\n\n", _scene->GetName());

    if (!_scene)
    {
        FBXSDK_printf("오류: 《FbxScene》을 만들 수 없음!\n\n");
        exit(1);
        return false;
    }

    // importer 에게 씬 전달
    if (!fbxImporter->Import(_scene))
    {
        printf("오류 보고: %s\n\n", fbxImporter->GetStatus().GetErrorString());
        exit(-1);
        return false;
    }

    // importer 삭제
    fbxImporter->Destroy();

    return true;
}

#pragma endregion CreateScene

//////////////////////////////////////////////////////////////////////

#pragma region Load By Node

void FBXLoad::FbxLoader::LoadAll(const char* file)
{
    FBXSDK_printf("file name: \" %s \" \n", file);

    // 파일을 불러올 수 없다면 얼리 리턴
    if (!CreateFbxScene(file)) return;

    // 씬에서 얻을 수 있는 것들

    // MetaData;
    // GlobalLightSettings
    // GlobalCameraSettings
    // GlobalTimeSettings
    // Hierarchy
    
    //DisplayContent();
    //DisplayPose();
    DisplayAnimation();

    // yGenericInfo

}

void FBXLoad::FbxLoader::DisplayContent(FbxNode* pNode)
{
    FbxNodeAttribute::EType lAttributeType;
    int i{ 0 };

    if (pNode->GetNodeAttribute() == NULL)
    {
        FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
            case fbxsdk::FbxNodeAttribute::eSkeleton:
                _skeletonLoader->DisplaySkeleton(pNode);
                break;
            case fbxsdk::FbxNodeAttribute::eMesh:
                _meshLoader->LoadMesh(pNode);
                break;
            case fbxsdk::FbxNodeAttribute::eNurbs:
                break;
            case fbxsdk::FbxNodeAttribute::ePatch:
                break;
            case fbxsdk::FbxNodeAttribute::eCamera:
                break;
            case fbxsdk::FbxNodeAttribute::eLight:
                break;
            case fbxsdk::FbxNodeAttribute::eLODGroup:
                break;
            default:
                break;
        }
    }

    // etc
    // DisplayUserProperties(pNode);
    // DisplayTarget(pNode);
    // DisplayPivotsAndLimits(pNode);
    // DisplayTransformPropagation(pNode);
    // DisplayGeometricTransform(pNode);

    for (i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayContent(pNode->GetChild(i));
    }
}

void FBXLoad::FbxLoader::DisplayContent()
{
    int i{ 0 };
    FbxNode* node{ _scene->GetRootNode() };

    // 노드를 불러올 수 없다면 얼리 리턴
    if (!node) return;

    const int numChild{ node->GetChildCount() };

    for (i = 0; i < numChild; i++)
    {
        DisplayContent(node->GetChild(i));
    }
}

#pragma region etc

void FBXLoad::FbxLoader::DisplayTarget(FbxNode* pNode)
{
    if (pNode->GetTarget() != NULL)
    {
        FBXSDK_printf(">>    Target Name: %s\n",
                      (char*)pNode->GetTarget()->GetName());
    }
}

void FBXLoad::FbxLoader::DisplayTransformPropagation(FbxNode* pNode)
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
}

void FBXLoad::FbxLoader::DisplayGeometricTransform(FbxNode* pNode)
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

#pragma endregion etc

#pragma endregion Load By Node

//////////////////////////////////////////////////////////////////////

#pragma region Load By Scene

void FBXLoad::FbxLoader::DisplayPose()
{
    _posLoader->DisplayPose(_scene);
}

void FBXLoad::FbxLoader::DisplayAnimation()
{
    _animationLoader->DisplayAnimation(_scene);
}

#pragma endregion Load By Scene

//////////////////////////////////////////////////////////////////////

#pragma region Mesh

void FBXLoad::FbxLoader::LoadMesh(const char* file)
{
    FBXSDK_printf("file name: \"%s\" \n", file);

    // Scene를 만들지 못하면 얼리 리턴
    if (!CreateFbxScene(file)) return;

    LoadMesh();
}

void FBXLoad::FbxLoader::LoadMesh(FbxNode* pNode)
{
    FbxNodeAttribute::EType lAttributeType;
    int i{ 0 };

    if (pNode->GetNodeAttribute() == NULL)
    {
        FBXSDK_printf("NULL Node Attribute\n\n");
        return;
    }

    lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

    if (lAttributeType != fbxsdk::FbxNodeAttribute::eMesh)
        return;

    _meshLoader->LoadMesh(pNode);

    for (i = 0; i < pNode->GetChildCount(); i++)
    {
        LoadMesh(pNode->GetChild(i));
    }
}

void FBXLoad::FbxLoader::LoadMesh()
{
    int i{ 0 };
    FbxNode* node{ _scene->GetRootNode() };
    const int numChild{ node->GetChildCount() };

    if (node)
    {
        for (i = 0; i < numChild; i++)
        {
            LoadMesh(node->GetChild(i));
        }
    }
}

#pragma endregion Mesh

#pragma region Animation Object

ImpStructure::ImpAnimationObject* FBXLoad::FbxLoader::LoadAnimationObject(const char* file)
{
    FBXSDK_printf("file name: \"%s\" \n", file);

    // Scene를 만들지 못하면 얼리 리턴
    if (!CreateFbxScene(file)) return nullptr;

    // 내보낼 AnimationObject 생성
    ImpStructure::ImpAnimationObject* animationObj{ new ImpStructure::ImpAnimationObject() };

    _posLoader->LoadPose(_scene, *animationObj);
}

#pragma endregion Animation Object