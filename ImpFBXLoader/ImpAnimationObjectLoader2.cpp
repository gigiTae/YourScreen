#include "ImpAnimationObjectLoader.h"
#include <fbxsdk.h>
#include <cassert>
#include <algorithm>
#include "ImpAnimationObjectLoader2.h"
#include <set>

using ImpNodeMap = std::unordered_map<std::string, ImpAnimationNode*>;

namespace FBXLoad
{
ImpStructure::ImpAnimationObject* FBXLoad::ImpAnimationObjectLoader2::LoadAnimationObject2(
    const char* file)
{
    _boneList.clear();
    _meshList.clear();

    SceneLoader scene{ file };

    // 애니메이션 오브젝트 생성
    ImpAnimationObject* impAnimObj = new ImpAnimationObject();

    // 총 애니메이션 시간
    impAnimObj->_maxAnimTime = (float)scene._animationInfo.duration;

    for (auto& myFbxNode : scene._myFbxNodesList)
    {
        /// 먼저 노드 기본 정보 부터 로드

        LoadImpNode(myFbxNode, impAnimObj, scene._fbxAnimationTrans);

        /// 스켈레톤 정보를 로드
        // 메시에서 본 가중치 관련 값을 세팅하기 위해 먼저
        if (myFbxNode->skeleton == nullptr) continue;

        Bone* impBone = new Bone();

        impAnimObj->_nodes[myFbxNode->name]->_bone = impBone;

        impBone->_boneName       = myFbxNode->name;
        impBone->_boneTM_NodeTM  = LoadGlobalTransform(myFbxNode->node);
        impBone->_boneTM_WorldTM = LoadGlobalTransform(myFbxNode->node);

        _boneList.emplace_back(impBone);
    }

    /// 메시 정보 로드
    for (auto& myFbxNode : scene._myFbxNodesList)
    {
        if (myFbxNode->mesh == nullptr) continue;

        impAnimObj->_nodes[myFbxNode->name]->_mesh = new ImpMesh();

        ImpMesh* mesh = impAnimObj->_nodes[myFbxNode->name]->_mesh;

        mesh->_meshName = myFbxNode->name;

        LoadMeshes(myFbxNode->mesh, mesh);
    }

    _impAnimObj = impAnimObj;

    return impAnimObj;
}
std::vector<ImpStructure::Bone*> ImpAnimationObjectLoader2::LoadBoneList()
{
    return _boneList;
}

std::vector<ImpStructure::ImpMesh*> ImpAnimationObjectLoader2::LoadMeshList()
{
    return _meshList;
}

void ImpAnimationObjectLoader2::LoadImpNode(
    std::unique_ptr<MyFbxNode>& myNode,
    ImpAnimationObject* impAnimObject,
    std::unordered_map<std::string, std::unique_ptr<MyFbxAnimTransform>>& myAnimTrans)
{
    ImpAnimationNode* impAnimNode{ new ImpAnimationNode() };

    impAnimNode->_name           = myNode->name;
    impAnimNode->_mesh           = nullptr;
    impAnimNode->_localTransform = LoadLocalTransform(myNode->node);
    impAnimNode->_worldTransform = LoadGlobalTransform(myNode->node);
    impAnimNode->_bone           = nullptr;
    SetParentNodeAddress(myNode->node, impAnimNode, impAnimObject->_nodes);
    impAnimNode->_animation = nullptr;

    MyFbxAnimTransform* animTrans = myAnimTrans[myNode->name].get();
    if (animTrans != nullptr)
    {
        impAnimNode->_animation = new ImpAnimation();

        for (auto& map : animTrans->translation)
        {
            AnimationPosition* animPosition{ new AnimationPosition() };

            animPosition->_tick  = map.first;
            animPosition->_pos.x = (float)map.second[0];
            animPosition->_pos.y = (float)map.second[1];
            animPosition->_pos.z = (float)map.second[2];

            impAnimNode->_animation->_positions.emplace_back(animPosition);
        }

        for (auto& map : animTrans->rotation)
        {
            AnimationRotation* animRotation{ new AnimationRotation() };

            animRotation->_tick               = map.first;
            animRotation->_rotQT_accumulation.x = (float)map.second[0];
            animRotation->_rotQT_accumulation.y = (float)map.second[1];
            animRotation->_rotQT_accumulation.z = (float)map.second[2];
            animRotation->_rotQT_accumulation.w = (float)map.second[3];

            //Vector3 euler = animRotation->_rotQT_accumulation.ToEuler();
            //animRotation->_rotQT_accumulation.MakeFromYawPitchRoll(euler.z, euler.x, euler.y);


            impAnimNode->_animation->_rotations.emplace_back(animRotation);
        }

        for (auto& map : animTrans->scale)
        {
            AnimationScale* animScale{ new AnimationScale() };

            animScale->_tick    = map.first;
            animScale->_scale.x = (float)map.second[0];
            animScale->_scale.y = (float)map.second[1];
            animScale->_scale.z = (float)map.second[2];

            impAnimNode->_animation->_scales.emplace_back(animScale);
        }
    }

    // 애니메이션
    /*MyFbxAnimCurve* animationCurve = myAnimCurve[myNode->name].get();
    if (animationCurve != nullptr)
    {
        if (!animationCurve->translation.empty() &&
            !animationCurve->rotation.empty() && !animationCurve->scale.empty())
        {
            impAnimNode->_animation = new ImpAnimation();

            for (auto& map : animationCurve->translation)
            {
                AnimationPosition* animPosition{ new AnimationPosition() };

                animPosition->_tick  = map.first;
                animPosition->_pos.x = map.second[0];
                animPosition->_pos.y = map.second[1];
                animPosition->_pos.z = map.second[2];

                impAnimNode->_animation->_positions.emplace_back(animPosition);
            }

            for (auto& map : animationCurve->rotation)
            {
                AnimationRotation* animRotation{ new AnimationRotation() };

                Quaternion
                    rotationQuat = Quaternion()
                                       .MakeFromYawPitchRoll(DegreeToRadian(
                                                                 map.second[0]),
                                                             DegreeToRadian(
                                                                 map.second[1]),
                                                             DegreeToRadian(
                                                                 map.second[2]));

                animRotation->_tick               = map.first;
                animRotation->_rotQT_accumulation = rotationQuat;

                impAnimNode->_animation->_rotations.emplace_back(animRotation);
            }

            for (auto& map : animationCurve->scale)
            {
                AnimationScale* animScale{ new AnimationScale() };

                animScale->_tick    = map.first;
                animScale->_scale.x = map.second[0];
                animScale->_scale.y = map.second[1];
                animScale->_scale.z = map.second[2];

                impAnimNode->_animation->_scales.emplace_back(animScale);
            }
        }
    }*/

    // 애니메이션 오브젝트의 노드 맵에 넣기
    impAnimObject->_nodes.insert(
        std::unordered_map<std::string,
                           ImpAnimationNode*>::value_type(impAnimNode->_name,
                                                          impAnimNode));
}

void ImpAnimationObjectLoader2::LoadMeshes(FbxMesh* fbxMesh, ImpMesh* impMesh)
{
    impMesh->_meshName = fbxMesh->GetName();

    LoadVertex(fbxMesh, impMesh);
    LoadFace(impMesh);
    LoadBoneWeight(fbxMesh, impMesh);

    _meshList.emplace_back(impMesh);
}

void ImpAnimationObjectLoader2::LoadVertex(FbxMesh* fbxMesh, ImpMesh* impMesh)
{
    int i, j, polygonCount = fbxMesh->GetPolygonCount();
    FbxVector4* controlPoints = fbxMesh->GetControlPoints();
    std::vector<ImpVertex> myMeshVertex;

    myMeshVertex.resize(polygonCount * 3);

    int vertexId{ 0 };

    for (i = 0; i < polygonCount; i++)
    {
        // 폴리곤의 사이즈는 무조건 3 이다.
        int polygonSize = fbxMesh->GetPolygonSize(i);

        for (j = 0; j < polygonSize; j++)
        {
            // 버텍스의 Control Point 등록
            int controlPointIndex = fbxMesh->GetPolygonVertex(i, j);

            /// ----------------
            /// Vertex Position
            /// ----------------

            // 포지션을 담는 변수를 준비한다.
            FbxVector4 localPosition;

            if (controlPointIndex < 0)
            {
                continue;
            }
            else
            {
                localPosition = controlPoints[controlPointIndex];
                // 위치값을 담는다.
                myMeshVertex[i * 3 + j]._localPos = {
                    static_cast<float>(localPosition[0]),
                    static_cast<float>(localPosition[1]),
                    static_cast<float>(localPosition[2])
                };

                // 그외 필수 정보들도 담는다
                myMeshVertex[i]._controlPoint = controlPointIndex;
                myMeshVertex[i]._vertexId     = vertexId;
            }

            /// --------------
            /// Vertex Color
            /// --------------

            // 컬러값을 담는 변수를 준비한다.
            FbxColor vertexColor;

            int eColorCount = fbxMesh->GetElementVertexColorCount();

            // 컬러값 데이터 유무 표시한다.
            if (eColorCount) impMesh->_hasColor = true;

            for (int l = 0; l < eColorCount; l++)
            {
                impMesh->_hasColor = true;
                FbxGeometryElementVertexColor* leVtxc = fbxMesh->GetElementVertexColor(
                    l);
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
                            }
                            break;
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                int id = leVtxc->GetIndexArray().GetAt(
                                    controlPointIndex);
                                vertexColor = leVtxc->GetDirectArray().GetAt(id);
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
                            }
                            break;
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                int id = leVtxc->GetIndexArray().GetAt(vertexId);
                                vertexColor = leVtxc->GetDirectArray().GetAt(id);
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

                // 컬러 값을 담는다.
                myMeshVertex[i * 3 + j]._color = {
                    static_cast<float>(vertexColor[0]),
                    static_cast<float>(vertexColor[1]),
                    static_cast<float>(vertexColor[2]),
                    static_cast<float>(vertexColor[3])
                };

            } // for eColorCount

            /// ----
            /// UV
            /// ----

            // UV 데이터를 담는 변수를 준비한다.
            FbxVector2 uv;

            int eUVCount = fbxMesh->GetElementUVCount();
            if (eUVCount) impMesh->_hasTexture = true;
            for (int l = 0; l < eUVCount; ++l)
            {
                FbxGeometryElementUV* leUV = fbxMesh->GetElementUV(l);

                switch (leUV->GetMappingMode())
                {
                    default:
                        break;
                    case FbxGeometryElement::eByControlPoint:
                        switch (leUV->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            {
                                uv = leUV->GetDirectArray().GetAt(controlPointIndex);
                            }
                            break;
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                int id = leUV->GetIndexArray().GetAt(
                                    controlPointIndex);
                                uv = leUV->GetDirectArray().GetAt(id);
                            }
                            break;
                            default:
                                break; // other reference modes not shown here!
                        }
                        break;

                    case FbxGeometryElement::eByPolygonVertex:
                    {
                        int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
                        switch (leUV->GetReferenceMode())
                        {
                            case FbxGeometryElement::eDirect:
                            case FbxGeometryElement::eIndexToDirect:
                            {
                                uv = leUV->GetDirectArray().GetAt(lTextureUVIndex);
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

                // uv 좌표값을 담는다.
                myMeshVertex[i * 3 + j]._u = static_cast<float>(uv[0]);
                myMeshVertex[i * 3 + j]._v = static_cast<float>(uv[1]);

            } // for PolygonSize

            /// --------------
            /// Vertex Normal
            /// --------------

            // 노말 값을 담는 변수를 미리 준비한다.
            FbxVector4 normal;

            int normalCount = fbxMesh->GetElementNormalCount();

            // 노말 값의 유무 표시한다.
            if (normalCount) impMesh->_hasNormal = true;

            for (int l = 0; l < normalCount; ++l)
            {
                FbxGeometryElementNormal* eNormal = fbxMesh->GetElementNormal(l);

                if (eNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                {
                    switch (eNormal->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        {
                            normal = eNormal->GetDirectArray().GetAt(vertexId);
                            int a  = 3;
                        }
                        break;
                        case FbxGeometryElement::eIndexToDirect:
                        {
                            int id = eNormal->GetIndexArray().GetAt(vertexId);
                            normal = eNormal->GetDirectArray().GetAt(id);
                        }
                        break;
                        default:
                            break; // other reference modes not shown here!
                    }

                    // 노말 값을 담는다.
                    myMeshVertex[i * 3 + j]._normal = {
                        static_cast<float>(normal[0]),
                        static_cast<float>(normal[1]),
                        static_cast<float>(normal[2])
                    };

                } // for normalCount
            }

            /// ---------
            /// Tangent
            /// ---------

            // 탄젠트 값을 담는 변수를 준비한다.
            FbxVector4 tangent;

            int tangentCount = fbxMesh->GetElementTangentCount();
            if (tangentCount) impMesh->_hasTangent = true;
            for (int l = 0; l < tangentCount; ++l)
            {
                FbxGeometryElementTangent* eTangent = fbxMesh->GetElementTangent(l);

                if (eTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                {
                    switch (eTangent->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        {
                            tangent = eTangent->GetDirectArray().GetAt(vertexId);
                        }
                        break;
                        case FbxGeometryElement::eIndexToDirect:
                        {
                            int id  = eTangent->GetIndexArray().GetAt(vertexId);
                            tangent = eTangent->GetDirectArray().GetAt(id);
                        }
                        break;
                        default:
                            break; // other reference modes not shown here!
                    }
                }

                // 탄젠트 값을 담는다.
                myMeshVertex[i * 3 + j]._tangent = {
                    static_cast<float>(tangent[0]),
                    static_cast<float>(tangent[1]),
                    static_cast<float>(tangent[2])
                };

            } // for tangentCount

            /// ---------
            /// Binormal
            /// ---------

            // 바이노말 값을 담는 변수를 준비한다.

            FbxVector4 bitangent;

            for (int l = 0; l < fbxMesh->GetElementBinormalCount(); ++l)
            {
                FbxGeometryElementBinormal* eBinormal = fbxMesh->GetElementBinormal(
                    l);

                if (eBinormal->GetMappingMode() ==
                    FbxGeometryElement::eByPolygonVertex)
                {
                    switch (eBinormal->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        {
                            bitangent = eBinormal->GetDirectArray().GetAt(vertexId);
                        }
                        break;
                        case FbxGeometryElement::eIndexToDirect:
                        {
                            int id = eBinormal->GetIndexArray().GetAt(vertexId);
                            bitangent = eBinormal->GetDirectArray().GetAt(id);
                        }
                        break;
                        default:
                            break; // other reference modes not shown here!
                    }
                }

                // 바이노말 값을 담는다.
                myMeshVertex[i * 3 + j]._bitangent = {
                    static_cast<float>(bitangent[0]),
                    static_cast<float>(bitangent[1]),
                    static_cast<float>(bitangent[2])
                };

            } // for Binormal Count

            vertexId++;
        } // for polygonSize
    }     // for polygonCount


    // 겹치는 것 없애도록 최적화
    // 최적화 이후에도 버텍스가 3배수야 하는데 거기까지 보장하는지는 모르겟다.
    // std::sort(myMeshVertex.begin(), myMeshVertex.end());
    // auto newEnd = std::unique(myMeshVertex.begin(), myMeshVertex.end());
    // myMeshVertex.resize(std::distance(myMeshVertex.begin(), newEnd));
    // myMeshVertex.erase(std::remove(myMeshVertex.begin(), myMeshVertex.end(), nullptr),
    //                    myMeshVertex.end());


    for (auto& vertex : myMeshVertex)
    {
        impMesh->_meshVertex.emplace_back(new ImpVertex(vertex));
    }
}

void ImpAnimationObjectLoader2::LoadFace(ImpMesh* impMesh)
{

    size_t vertexCount = impMesh->_meshVertex.size();
    size_t faceCount   = vertexCount / 3;

    impMesh->_meshFace.resize(faceCount);

    for (size_t i = 0; i < faceCount; i++)
    {
        impMesh->_meshFace[i] = new ImpFace();

        for (size_t j = 0; j < 3; j++)
        {
            impMesh->_meshFace[i]->vertexIndex[j] = i * 3 + j;
        }
    }
}

void ImpAnimationObjectLoader2::LoadBoneWeight(FbxMesh* fbxMesh, ImpMesh* impMesh)
{
    if (fbxMesh->GetDeformerCount(FbxDeformer::eSkin) <= 0) return;

    impMesh->_hasWeight = true;

    for (int i = 0; i < fbxMesh->GetDeformerCount(); i++)
    {

        FbxDeformer* fbxDeformer{ fbxMesh->GetDeformer(i) };

        if (fbxDeformer == nullptr) continue;

        switch (fbxDeformer->GetDeformerType())
        {
            case FbxDeformer::eSkin:
            {
                if ((FbxSkin*)fbxDeformer == nullptr) continue;
                LoadBoneWeight((FbxSkin*)fbxDeformer, impMesh);
            }
            break;

            default:
                break;
        }
    }
}

void ImpAnimationObjectLoader2::LoadBoneWeight(FbxSkin* fbxSkin, ImpMesh* impMesh)
{
    int clusterCount{ fbxSkin->GetClusterCount() };

    FbxCluster::ELinkMode linkMode{ FbxCluster::ELinkMode::eNormalize };

    for (int i = 0; i < clusterCount; i++)
    {
        FbxCluster* cluster{ fbxSkin->GetCluster(i) };
        if (cluster == nullptr) continue;

        linkMode            = cluster->GetLinkMode();
        FbxNode* linkedNode = cluster->GetLink();
        if (linkedNode == nullptr) continue;

        FbxAMatrix clusterLinkedTransformMatrix;
        cluster->GetTransformLinkMatrix(clusterLinkedTransformMatrix);
        clusterLinkedTransformMatrix.Get(1, 1);

        //_boneList[i];
        cluster->GetLink()->GetName();
        impMesh->_meshVertex;

        int* lIndices    = cluster->GetControlPointIndices();
        double* lWeights = cluster->GetControlPointWeights();

        for (int k = 0; k < cluster->GetControlPointIndicesCount(); k++)
        {

            for (auto& vertex : impMesh->_meshVertex)
            {
                if (vertex->_controlPoint != lIndices[k]) continue;

                if (vertex->_weightIndices[0] == (size_t)-1)
                {
                    vertex->_weightIndices[0] = i;
                    vertex->_weight.x         = static_cast<float>(lWeights[k]);
                }
                else if (vertex->_weightIndices[1] == (size_t)-1)
                {
                    vertex->_weightIndices[1] = i;
                    vertex->_weight.y         = static_cast<float>(lWeights[k]);
                }
                else if (vertex->_weightIndices[2] == (size_t)-1)
                {
                    vertex->_weightIndices[2] = i;
                    vertex->_weight.z         = static_cast<float>(lWeights[k]);
                }
                else if (vertex->_weightIndices[3] == (size_t)-1)
                {
                    vertex->_weightIndices[3] = i;
                    vertex->_weightW          = static_cast<float>(lWeights[k]);
                }
            }
        }
    }

    switch (linkMode)
    {
        case fbxsdk::FbxCluster::eNormalize:
            // 정규화
            {
                for (auto& vertex : impMesh->_meshVertex)
                {
                    float sum = vertex->_weight.x + vertex->_weight.y +
                                vertex->_weight.z + vertex->_weightW;

                    // 이게... 맞나?
                    if (std::abs(sum - 1.f) != 0) // sum == 0
                    {
                        vertex->_weight.x /= sum;
                        vertex->_weight.y /= sum;
                        vertex->_weight.z /= sum;
                    }
                }
            }
            break;
        case fbxsdk::FbxCluster::eAdditive:
        case fbxsdk::FbxCluster::eTotalOne:
        default:
            break;
    }
}

int ImpAnimationObjectLoader2::FindeBoneIndex(std::string name)
{
    for (int i = 0; i < (int)_boneList.size(); i++)
    {
        if (_boneList[i]->_boneName == name)
        {
            return i;
        }
    }

    return -1;
}

ImpMath::Matrix ImpAnimationObjectLoader2::LoadGlobalTransform(FbxNode* fbxNode)
{
    FbxAMatrix fbxMatrix = fbxNode->EvaluateGlobalTransform();

    FbxVector4 trn = fbxMatrix.GetT();
    FbxVector4 rot = fbxMatrix.GetR();
    FbxVector4 scl = fbxMatrix.GetS();

    FbxVector4 newTranslation{ trn[0], trn[1], trn[2], trn[3] };
    FbxVector4 newRotation{ rot[0], rot[1], rot[2], rot[3] };
    FbxVector4 newScale{ scl[0], scl[1], scl[2], scl[3] };

    FbxAMatrix newMatrix;
    newMatrix.SetT(newTranslation);
    newMatrix.SetR(newRotation);
    newMatrix.SetS(newScale);

    return ConverImpMatrixFrom(newMatrix);
}
ImpMath::Matrix ImpAnimationObjectLoader2::LoadLocalTransform(FbxNode* fbxNode)
{
    FbxAMatrix fbxMatrix = fbxNode->EvaluateLocalTransform();

    FbxVector4 trn = fbxMatrix.GetT();
    FbxVector4 rot = fbxMatrix.GetR();
    FbxVector4 scl = fbxMatrix.GetS();

    FbxVector4 newTranslation{ trn[0], trn[1], trn[2], trn[3] };
    FbxVector4 newRotation{ rot[0], rot[1], rot[2], rot[3] };
    FbxVector4 newScale{ scl[0], scl[1], scl[2], scl[3] };

    FbxAMatrix newMatrix;
    newMatrix.SetT(newTranslation);
    newMatrix.SetR(newRotation);
    newMatrix.SetS(newScale);

    return ConverImpMatrixFrom(newMatrix);
}
ImpMath::Matrix ImpAnimationObjectLoader2::ConverImpMatrixFrom(const FbxAMatrix& fbxMatrix)
{
    ImpMath::Matrix impMatrix;

    impMatrix._11 = static_cast<float>(fbxMatrix.Get(0, 0));
    impMatrix._12 = static_cast<float>(fbxMatrix.Get(0, 1));
    impMatrix._13 = static_cast<float>(fbxMatrix.Get(0, 2));
    impMatrix._14 = static_cast<float>(fbxMatrix.Get(0, 3));

    impMatrix._21 = static_cast<float>(fbxMatrix.Get(1, 0));
    impMatrix._22 = static_cast<float>(fbxMatrix.Get(1, 1));
    impMatrix._23 = static_cast<float>(fbxMatrix.Get(1, 2));
    impMatrix._24 = static_cast<float>(fbxMatrix.Get(1, 3));

    impMatrix._31 = static_cast<float>(fbxMatrix.Get(2, 0));
    impMatrix._32 = static_cast<float>(fbxMatrix.Get(2, 1));
    impMatrix._33 = static_cast<float>(fbxMatrix.Get(2, 2));
    impMatrix._34 = static_cast<float>(fbxMatrix.Get(2, 3));

    impMatrix._41 = static_cast<float>(fbxMatrix.Get(3, 0));
    impMatrix._42 = static_cast<float>(fbxMatrix.Get(3, 1));
    impMatrix._43 = static_cast<float>(fbxMatrix.Get(3, 2));
    impMatrix._44 = static_cast<float>(fbxMatrix.Get(3, 3));

    return impMatrix;
}
void ImpAnimationObjectLoader2::SetParentNodeAddress(const FbxNode* fbxNode,
                                                     ImpAnimationNode* impAnimNode,
                                                     ImpNodeMap& impNode)
{
    const FbxNode* parentFbxNode = fbxNode->GetParent();

    if (!parentFbxNode)
    {
        impAnimNode->_parentNode = nullptr;
        return;
    }

    auto parentName = parentFbxNode->GetName();

    if (impNode[parentName] == nullptr) return;
    auto parentImpNode = impNode[parentName];

    // 부모 노드의 주소 설정
    impAnimNode->_parentNode = parentImpNode;
    // 부모 노드의 자식 노드 추가
    parentImpNode->_childNodes.push_back(impAnimNode);
}
void ImpAnimationObjectLoader2::LoadBoneData(FbxNode* fbxNode, Bone* _bone)
{
    _bone->_boneName       = fbxNode->GetName();
    _bone->_boneTM_WorldTM = LoadGlobalTransform(fbxNode);
    _bone->_boneTM_NodeTM  = LoadGlobalTransform(fbxNode);
}

} // namespace FBXLoad