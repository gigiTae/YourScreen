#include "SceneLoader.h"

#include <assert.h>

namespace FBXLoad
{

using MyNodeMap = std::unordered_map<std::string, std::unique_ptr<MyFbxNode>>;

SceneLoader::SceneLoader(const char* file)
    : _manager{ FbxManager::Create() }
    , _geometyConverter{ new FbxGeometryConverter(_manager) }
    , _scene{ FbxScene::Create(_manager, file) }
{

#ifdef _DEBUG
    FBXSDK_printf("파일 \"%s\"의 scene 생성\n", _scene->GetName());
#endif

    assert(_manager);
    assert(_scene);
    assert(_geometyConverter);

    LoadScene(file);
}

SceneLoader::~SceneLoader()
{
    delete _geometyConverter;
    if (_scene) _scene->Destroy();
    if (_manager) _manager->Destroy();
}

void SceneLoader::LoadScene(const char* file)
{
    //
    // 블랜더 의 좌표계(오른손)
    //    UpVector = +Z, FrontVector = +Y, CoordSystem = +X (RightHanded)
    //
    // DirectX 좌표계(왼손)
    //    UpVector = +Y, FrontVector = +Z, CoordSystem = -X (LeftHanded)
    //

    /*
    // up :Y, fornt: Z, coorSys: X
    FbxAxisSystem defualt;

    // up :Y, fornt: X, coorSys: -Z
    FbxAxisSystem directXAxisSys(FbxAxisSystem::EUpVector::eYAxis,
        FbxAxisSystem::EFrontVector::eParityEven,
        FbxAxisSystem::eRightHanded);

    // 하고 싶은 것(블랜더 설정없이 내보내기 -> DirectX로)
    // up :Z, fornt: Y, coorSys: -X
    FbxAxisSystem custemSetting(FbxAxisSystem::EUpVector::eZAxis,
        FbxAxisSystem::EFrontVector::eParityEven,
        FbxAxisSystem::eLeftHanded);

    custemSetting.ConvertScene(_scene);
    */

    // FbxAxisSystem sceneAxisSystem = _scene->GetGlobalSettings().GetAxisSystem();
    _scene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::eDirectX);


    // importer 생성
    FbxImporter* importer{ FbxImporter::Create(_manager, "Importer") };
    bool status = importer->Initialize(file, -1, _manager->GetIOSettings());
    assert(status && "파일을 열 수 없음");

    status = importer->Import(_scene);
    assert(status && "scene 을 못만들었음");

    ProcessScene();

    importer->Destroy();
}

/// <summary>
/// FbxScene 객체를 이용해 얻는 데이터
/// </summary>
void SceneLoader::ProcessScene()
{
    // ProcessMaterial(); // 지금 시점에서 안 사용

    ProcessNode(_scene->GetRootNode());

    ProcessAnimations();
}

void SceneLoader::ProcessMaterial()
{
    for (int i = 0; i < _scene->GetMaterialCount(); i++)
    {
        FbxSurfaceMaterial* fbxMaterial = _scene->GetMaterial(i);
        _fbxMaterials.emplace_back(fbxMaterial);
    }
}


/// <summary>
/// 트리구조인 FbxNode 를 순회하면서 얻을 수 있는 데이터를 모은다.
/// </summary>
/// <param name="node"></param>
void SceneLoader::ProcessNode(FbxNode* node)
{
    // 방문한 노드 업데이트
    _fbxNodeMap.insert(std::pair<std::string, FbxNode*>(node->GetName(), node));
    _myFbxNodesList.emplace_back(std::make_unique<MyFbxNode>(node));

    // 노드 속성에 따라 각 속성 파밍
    // 여기는 Mesh 와 Skeleton 정보만 필요하다
    FbxNodeAttribute* nodeAttri = node->GetNodeAttribute();
    if (nodeAttri != nullptr)
    {
        switch (nodeAttri->GetAttributeType())
        {
            case fbxsdk::FbxNodeAttribute::eSkeleton:
                ProcessSkeleton(node);
                break;
            case fbxsdk::FbxNodeAttribute::eMesh:
                ProcessMesh(node);
                break;
            case fbxsdk::FbxNodeAttribute::eNurbs:
            case fbxsdk::FbxNodeAttribute::ePatch:
            case fbxsdk::FbxNodeAttribute::eCamera:
            case fbxsdk::FbxNodeAttribute::eLight:
            case fbxsdk::FbxNodeAttribute::eLODGroup:
            default:
                break;
        }
    }

    // 재귀적으로 자식 노드 탐색
    for (int i = 0; i < node->GetChildCount(); i++)
    {
        ProcessNode(node->GetChild(i));
    }
}

/// <summary>
/// 애니메이션의 경우 Source Object(FbxAnimStack) 마다 가져와야 하므로 따로 처리한다.
/// </summary>
void SceneLoader::ProcessAnimations()
{
    if (_scene == nullptr) return;

    // 루트 노드가 없다는 것은 스켈레톤이 없다는 의미
    if (_scene->GetRootNode() == nullptr) return;

    int objectCount{ _scene->GetSrcObjectCount<FbxAnimStack>() };

    for (int i = 0; i < objectCount; i++)
    {
        FbxAnimStack* animStack{ _scene->GetSrcObject<FbxAnimStack>(i) };
        FbxTakeInfo* animInfo{ _scene->GetTakeInfo(animStack->GetName()) };

        if (animInfo == nullptr) continue;

        AnimationInfo myAnimInfo;

        myAnimInfo.name = animInfo->mName.Buffer();

        FbxTimeSpan span{ animInfo->mLocalTimeSpan };

        myAnimInfo.start = static_cast<unsigned int>(span.GetStart().GetFrameCount(FbxTime::eFrames30));
        myAnimInfo.duration = static_cast<unsigned int>(span.GetDuration().GetFrameCount(FbxTime::eFrames30));
        myAnimInfo.end = static_cast<unsigned int>(span.GetStop().GetFrameCount(FbxTime::eFrames30));

        unsigned int frameCount = static_cast<unsigned int>(
            span.GetDuration().GetFrameCount(FbxTime::eFrames30));

        for (int i = 0; i < animStack->GetMemberCount<FbxAnimLayer>(); i++)
        {
            FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(i);


            // 본격적으로 노드 따라 애니메이션 정보 가져오기


            // 여러 애니메이션 가져오기
            std::unique_ptr<AnimTransMap> fbxAnimationTrans{ std::make_unique<AnimTransMap>()};
            ProcessAnimations(_scene->GetRootNode(), animLayer, fbxAnimationTrans, frameCount);

            /*
            fbxAnimationTrans.insert(
            std::pair<std::string, std::unique_ptr<MyFbxAnimTransform>>(node->GetName(),
            std::make_unique<MyFbxAnimTransform>(tMap, rMap, sMap)));
            */

            _fbxAnimationStack.insert(
                std::pair<std::string, std::unique_ptr<AnimTransMap>>(myAnimInfo.name, 
                    std::move(fbxAnimationTrans)));

            // 예전 거
            ProcessAnimations(_scene->GetRootNode(), animLayer);
        }

        // 몬가... 몬가로 대체해야함
        // ---------------
        _animationInfo = myAnimInfo;
        _fbxAnimationTrans;
        // ---------------

        /*AnimationStack Animstack;
        Animstack.animInfo          = myAnimInfo;
        Animstack.fbxAnimationTrans = _fbxAnimationTrans;
        _fbxAnimationStack.emplace_back(Animstack);*/
    }
}

void SceneLoader::ProcessAnimations(FbxNode* node, FbxAnimLayer* animLayer)
{
    FbxAnimCurve* animCurve{ nullptr };
    std::set<unsigned int> trsTimeSet;
    animCurve = node->LclTranslation.GetCurve(
        animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    LoadTimeSet(animCurve, trsTimeSet);
    animCurve = node->LclTranslation.GetCurve(
        animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    LoadTimeSet(animCurve, trsTimeSet);
    animCurve = node->LclTranslation.GetCurve(
        animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    LoadTimeSet(animCurve, trsTimeSet);

    std::set<unsigned int> rotTimeSet;
    animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    LoadTimeSet(animCurve, rotTimeSet);
    animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    LoadTimeSet(animCurve, rotTimeSet);
    animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    LoadTimeSet(animCurve, rotTimeSet);

    std::set<unsigned int> sclTimeSet;
    animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    LoadTimeSet(animCurve, sclTimeSet);
    animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    LoadTimeSet(animCurve, sclTimeSet);
    animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    LoadTimeSet(animCurve, sclTimeSet);

    if (!trsTimeSet.empty() && !rotTimeSet.empty() && !sclTimeSet.empty())
    {
        std::map<unsigned int, FbxVector4> tMap;
        for (const auto& t : trsTimeSet)
        {
            FbxTime time;
            time.Set(t);

            FbxAMatrix currentTransform = node->EvaluateLocalTransform(time);
            FbxVector4 trs              = currentTransform.GetT();
            // FbxVector4 trs = node->EvaluateLocalTranslation(time);
            unsigned frameTime = (unsigned)time.GetFrameCount(FbxTime::eFrames30);
            tMap.insert(std::pair<unsigned int, FbxVector4>(frameTime, trs));
        }

        std::map<unsigned int, FbxVector4> rMap;
        for (const auto& t : rotTimeSet)
        {
            FbxTime time;
            time.Set(t);

            FbxAMatrix currentTransform = node->EvaluateLocalTransform(time);
            // FbxVector4 rott             = currentTransform.GetT();
            FbxQuaternion temp = currentTransform.GetQ();
            FbxVector4 rot;

            rot[0] = temp[0];
            rot[1] = temp[1];
            rot[2] = temp[2];
            rot[3] = temp[3];

            // FbxVector4 rot             = node->EvaluateLocalRotation(time);
            unsigned frameTime = (unsigned)time.GetFrameCount(FbxTime::eFrames30);
            rMap.insert(std::pair<unsigned int, FbxVector4>(frameTime, rot));
        }

        std::map<unsigned int, FbxVector4> sMap;
        for (const auto& t : sclTimeSet)
        {
            FbxTime time;
            time.Set(t);

            // FbxAMatrix currentTransform = node->EvaluateLocalTransform(time);
            // FbxVector4 scl              = currentTransform.GetS();
            FbxVector4 scl = node->EvaluateLocalScaling();
            unsigned frameTime = (unsigned)time.GetFrameCount(FbxTime::eFrames30);
            sMap.insert(std::pair<unsigned int, FbxVector4>(frameTime, scl));
        }

        _fbxAnimationTrans.insert(
            std::pair<std::string, std::unique_ptr<MyFbxAnimTransform>>(
                node->GetName(),
                std::make_unique<MyFbxAnimTransform>(tMap, rMap, sMap)));
    }

    for (int i = 0; i < node->GetChildCount(); i++)
    {
        ProcessAnimations(node->GetChild(i), animLayer);
    }
}

void SceneLoader::ProcessAnimations(FbxNode* node,
    FbxAnimLayer* animLayer,
    std::unique_ptr<AnimTransMap>& fbxAnimationTrans,
    unsigned int frameCount)
{
    FbxAnimCurve* animCurve{ nullptr };
    std::map<unsigned int, FbxVector4> tMap;
    animCurve = node->LclTranslation.GetCurve(
        animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    LoadXMap(animCurve, tMap, frameCount);
    animCurve = node->LclTranslation.GetCurve(
        animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    LoadYMap(animCurve, tMap);
    animCurve = node->LclTranslation.GetCurve(
        animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    LoadZMap(animCurve, tMap);

    std::map<unsigned int, FbxVector4> rMap;
    animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    LoadXMap(animCurve, rMap, frameCount);
    animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    LoadYMap(animCurve, rMap);
    animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    LoadZMap(animCurve, rMap);

    std::map<unsigned int, FbxVector4> sMap;
    animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    LoadXMap(animCurve, sMap, frameCount);
    animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    LoadYMap(animCurve, sMap);
    animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    LoadZMap(animCurve, sMap);

    // Rotation 만 따로 처리
    for (auto& r : rMap)
    {
        auto value = r.second;

        FbxQuaternion quat;
        quat.ComposeSphericalXYZ(value);

        r.second[0] = quat[0];
        r.second[1] = quat[1];
        r.second[2] = quat[2];
        r.second[3] = quat[3];
    }

    // fbxAnimationTrans 안에 넣는다
    fbxAnimationTrans->insert(
        std::pair<std::string, std::unique_ptr<MyFbxAnimTransform>>(node->GetName(),
            std::make_unique<MyFbxAnimTransform>(tMap, rMap, sMap)));

    for (int i = 0; i < node->GetChildCount(); i++)
    {
        ProcessAnimations(
            node->GetChild(i), animLayer, fbxAnimationTrans, frameCount);
    }
}

void SceneLoader::ProcessSkeleton(FbxNode* node)
{
    FbxSkeleton* fbxSkeleton{ (FbxSkeleton*)node->GetNodeAttribute() };
    if (fbxSkeleton == nullptr) return;

    _fbxSkeletons.emplace_back(
        std::pair<std::string, FbxSkeleton*>(node->GetName(), fbxSkeleton));
    _myFbxNodesList.back()->skeleton = fbxSkeleton;
}

void SceneLoader::ProcessMesh(FbxNode* node)
{
    // 모든 페이스를 삼각형 으로 바꿈
    FbxNodeAttribute* attribute = node->GetNodeAttribute();
    _geometyConverter->Triangulate(attribute, true, true);

    FbxMesh* fbxMesh = node->GetMesh();
    assert(fbxMesh && "FbxMesh 를 가져올 수 없음");

    _fbxMeshs.emplace_back(
        std::pair<std::string, FbxMesh*>(node->GetName(), fbxMesh));
    _myFbxNodesList.back()->mesh = fbxMesh;
}

void SceneLoader::LoadTimeSet(FbxAnimCurve* animCurve,
    std::set<unsigned int>& timeSet)
{
    if (!animCurve) return;
    for (int lCount = 0; lCount < animCurve->KeyGetCount(); lCount++)
    {
        unsigned int lKeyTime = (unsigned int)animCurve->KeyGetTime(lCount).Get();
        timeSet.insert(lKeyTime);
    }
}

/// <summary>
/// 모든 회전애니메이션의 x, y, z가 존재하고
/// 모든 프레임에 키가 있다고 가정하고 파싱
/// </summary>
/// <param name="animCurve"></param>
/// <param name="frameCount"></param>
/// <param name="map"></param>

void SceneLoader::LoadXMap(FbxAnimCurve* animCurve,
    std::map<unsigned int, FbxVector4>& map,
    unsigned int frameCount)
{
    if (!animCurve) return;

    for (int lCount = 0; lCount < animCurve->KeyGetCount(); lCount++)
    {
        unsigned int keyTime = (unsigned int)animCurve->KeyGetTime(lCount).GetFrameCount(
            FbxTime::eFrames30);
        float keyValue = static_cast<float>(animCurve->KeyGetValue(lCount));

        // x는 맨 첫번째므로 먼저 넣고 본다.
        FbxVector4 val{ keyValue, 0.f, 0.f, 0.f };

        // 그리고 맨 첫번째 값도 애니메이션 프레임 수 만큼 같은 값으로 초기화
        if (lCount == 0)
        {
            for (unsigned i = 0; i < frameCount; i++)
            {
                map.insert(std::pair<unsigned int, FbxVector4>(i, val));
            }

            continue;
        }

        map[keyTime][0] = keyValue;
    }

    animCurve = nullptr;
}

void SceneLoader::LoadYMap(FbxAnimCurve* animCurve,
    std::map<unsigned int, FbxVector4>& map)
{
    if (!animCurve) return;

    for (int lCount = 0; lCount < animCurve->KeyGetCount(); lCount++)
    {
        unsigned int keyTime = (unsigned int)animCurve->KeyGetTime(lCount).GetFrameCount(
            FbxTime::eFrames30);
        float keyValue = static_cast<float>(animCurve->KeyGetValue(lCount));

        // 맨 첫번째 값으로 전부 초기화
        if (lCount == 0)
        {
            for (auto& m : map)
            {
                m.second[1] = keyValue;
            }

            continue;
        }

        map[keyTime][1] = keyValue;
    }

    animCurve = nullptr;
}

void SceneLoader::LoadZMap(FbxAnimCurve* animCurve,
    std::map<unsigned int, FbxVector4>& map)
{
    if (!animCurve) return;

    for (int lCount = 0; lCount < animCurve->KeyGetCount(); lCount++)
    {
        unsigned int keyTime = (unsigned int)animCurve->KeyGetTime(lCount).GetFrameCount(
            FbxTime::eFrames30);
        float keyValue = static_cast<float>(animCurve->KeyGetValue(lCount));

        // 맨 첫번째 값으로 전부 초기화
        if (lCount == 0)
        {
            for (auto& m : map)
            {
                m.second[2] = keyValue;
            }

            continue;
        }

        map[keyTime][2] = keyValue;
    }

    animCurve = nullptr;
}

} // namespace FBXLoad