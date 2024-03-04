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
    FBXSDK_printf("���� \"%s\"�� scene ����\n", _scene->GetName());
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
    // ���� �� ��ǥ��(������)
    //    UpVector = +Z, FrontVector = +Y, CoordSystem = +X (RightHanded)
    //
    // DirectX ��ǥ��(�޼�)
    //    UpVector = +Y, FrontVector = +Z, CoordSystem = -X (LeftHanded)
    //

    /*
    // up :Y, fornt: Z, coorSys: X
    FbxAxisSystem defualt;

    // up :Y, fornt: X, coorSys: -Z
    FbxAxisSystem directXAxisSys(FbxAxisSystem::EUpVector::eYAxis,
        FbxAxisSystem::EFrontVector::eParityEven,
        FbxAxisSystem::eRightHanded);

    // �ϰ� ���� ��(���� �������� �������� -> DirectX��)
    // up :Z, fornt: Y, coorSys: -X
    FbxAxisSystem custemSetting(FbxAxisSystem::EUpVector::eZAxis,
        FbxAxisSystem::EFrontVector::eParityEven,
        FbxAxisSystem::eLeftHanded);

    custemSetting.ConvertScene(_scene);
    */

    // FbxAxisSystem sceneAxisSystem = _scene->GetGlobalSettings().GetAxisSystem();
    _scene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::eDirectX);


    // importer ����
    FbxImporter* importer{ FbxImporter::Create(_manager, "Importer") };
    bool status = importer->Initialize(file, -1, _manager->GetIOSettings());
    assert(status && "������ �� �� ����");

    status = importer->Import(_scene);
    assert(status && "scene �� ���������");

    ProcessScene();

    importer->Destroy();
}

/// <summary>
/// FbxScene ��ü�� �̿��� ��� ������
/// </summary>
void SceneLoader::ProcessScene()
{
    // ProcessMaterial(); // ���� �������� �� ���

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
/// Ʈ�������� FbxNode �� ��ȸ�ϸ鼭 ���� �� �ִ� �����͸� ������.
/// </summary>
/// <param name="node"></param>
void SceneLoader::ProcessNode(FbxNode* node)
{
    // �湮�� ��� ������Ʈ
    _fbxNodeMap.insert(std::pair<std::string, FbxNode*>(node->GetName(), node));
    _myFbxNodesList.emplace_back(std::make_unique<MyFbxNode>(node));

    // ��� �Ӽ��� ���� �� �Ӽ� �Ĺ�
    // ����� Mesh �� Skeleton ������ �ʿ��ϴ�
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

    // ��������� �ڽ� ��� Ž��
    for (int i = 0; i < node->GetChildCount(); i++)
    {
        ProcessNode(node->GetChild(i));
    }
}

/// <summary>
/// �ִϸ��̼��� ��� Source Object(FbxAnimStack) ���� �����;� �ϹǷ� ���� ó���Ѵ�.
/// </summary>
void SceneLoader::ProcessAnimations()
{
    if (_scene == nullptr) return;

    // ��Ʈ ��尡 ���ٴ� ���� ���̷����� ���ٴ� �ǹ�
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


            // ���������� ��� ���� �ִϸ��̼� ���� ��������


            // ���� �ִϸ��̼� ��������
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

            // ���� ��
            ProcessAnimations(_scene->GetRootNode(), animLayer);
        }

        // ��... �󰡷� ��ü�ؾ���
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

    // Rotation �� ���� ó��
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

    // fbxAnimationTrans �ȿ� �ִ´�
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
    // ��� ���̽��� �ﰢ�� ���� �ٲ�
    FbxNodeAttribute* attribute = node->GetNodeAttribute();
    _geometyConverter->Triangulate(attribute, true, true);

    FbxMesh* fbxMesh = node->GetMesh();
    assert(fbxMesh && "FbxMesh �� ������ �� ����");

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
/// ��� ȸ���ִϸ��̼��� x, y, z�� �����ϰ�
/// ��� �����ӿ� Ű�� �ִٰ� �����ϰ� �Ľ�
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

        // x�� �� ù��°�Ƿ� ���� �ְ� ����.
        FbxVector4 val{ keyValue, 0.f, 0.f, 0.f };

        // �׸��� �� ù��° ���� �ִϸ��̼� ������ �� ��ŭ ���� ������ �ʱ�ȭ
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

        // �� ù��° ������ ���� �ʱ�ȭ
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

        // �� ù��° ������ ���� �ʱ�ȭ
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