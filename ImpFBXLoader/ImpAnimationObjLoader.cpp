#include "ImpAnimationObjLoader.h"

#include <fbxsdk.h>
#include <cassert>
#include <algorithm>

namespace FBXLoad
{

void ImpAnimationObjLoader::CreateAnimationObj(const char* file)
{
    // ���ο� ������Ʈ�� �ε��� �� ���� �� ����Ʈ�� �޽� ����Ʈ �ʱ�ȭ
    _boneList.clear();
    _meshList.clear();

    // ������ ������ ��������
    SceneLoader scene{ file };

    // �ִϸ��̼� ������Ʈ ����
    ImpStructure::ImpAnimationObject* impAnimObj = new ImpStructure::ImpAnimationObject();

    // �� �ִϸ��̼� �ð�
    impAnimObj->_maxAnimTime = (float)scene._animationInfo.duration;

    // ����� �̸�, ��ȯ���, �ִϸ��̼�, ��
    for (auto& myFbxNode : scene._myFbxNodesList)
    {
        LoadImpNodes(myFbxNode, impAnimObj->_nodes, scene._fbxAnimationTrans);
    }

    // �޽�, �� �ε���, ����ġ
    //LoadMeshes(scene, _impAnimObj);
    for (auto& myFbxNode : scene._myFbxNodesList)
    {
        LoadMeshes(impAnimObj->_nodes[myFbxNode->name], myFbxNode->mesh);
    }

    // ������� ���� ���
    Display(impAnimObj);

    // �ִϸ��̼� ������ ����
    _impAnimObj = impAnimObj;
}

ImpStructure::ImpMesh* ImpAnimationObjLoader::GetMesh(size_t i)
{
    assert(_meshList.empty() != true && "�޽ð� ����");

    return _meshList[i];
}

ImpStructure::ImpAnimationObject* ImpAnimationObjLoader::GetAnimationObject()
{
    return _impAnimObj;
}

std::vector<ImpStructure::Bone*> ImpAnimationObjLoader::LoadBoneList()
{
    return _boneList;
}

std::vector<ImpStructure::ImpMesh*> ImpAnimationObjLoader::LoadMeshList()
{
    return _meshList;
}

void ImpAnimationObjLoader::Release()
{
    if (_impAnimObj) delete _impAnimObj;

    for (auto& mesh : _meshList)
    {
        delete mesh;
    }

    for (auto& bone : _boneList)
    {
        delete bone;
    }
}

void ImpAnimationObjLoader::LoadImpNodes(std::unique_ptr<MyFbxNode>& myNode,
    std::unordered_map<std::string, ImpStructure::ImpAnimationNode*>& impNodes,
    std::unordered_map<std::string, std::unique_ptr<MyFbxAnimTransform>>& myAnimTrans)
{
    ImpStructure::ImpAnimationNode* impNode{ new ImpStructure::ImpAnimationNode() };

    // ���� �޽�, �ִϸ��̼��� ������ �⺻���� �ε�
    LoadImpNode(impNode, myNode);

    // �θ�-�ڽ� ��� �ּ� �ε�
    SetParentNodeAddress(myNode->node, impNode, impNodes);

    // �ִϸ��̼� ������ �ε�
    LoadImpAnimation(impNode, myAnimTrans[myNode->name].get());

    // �� �����Ͱ� �ִٸ� �ε�
    LoadImpBone(impNode, myNode->skeleton);

    // �ִϸ��̼� ������Ʈ�� ��� �ʿ� �ֱ�
    impNodes.insert(
        std::unordered_map<std::string, ImpStructure::ImpAnimationNode*>::value_type(
        impNode->_name, impNode));
}

void ImpAnimationObjLoader::LoadImpNode(ImpStructure::ImpAnimationNode* impNode,
    std::unique_ptr<MyFbxNode>& myNode)
{
    impNode->_name           = myNode->name;
    impNode->_mesh           = nullptr;
    impNode->_localTransform = LoadLocalTransform(myNode->node);
    impNode->_worldTransform = LoadGlobalTransform(myNode->node);
    impNode->_bone           = nullptr;
    impNode->_animation      = nullptr;
}

void ImpAnimationObjLoader::LoadImpAnimation(ImpStructure::ImpAnimationNode* impNode,
    MyFbxAnimTransform* animTrans)
{
    if (animTrans == nullptr) return;

    impNode->_animation = new ImpStructure::ImpAnimation();

    for (auto& map : animTrans->translation)
    {
        ImpStructure::AnimationPosition* animPosition{
            new ImpStructure::AnimationPosition()
        };

        animPosition->_tick  = map.first;
        animPosition->_pos.x = (float)map.second[0];
        animPosition->_pos.y = (float)map.second[1];
        animPosition->_pos.z = (float)map.second[2];

        impNode->_animation->_positions.emplace_back(animPosition);
    }

    for (auto& map : animTrans->rotation)
    {
        ImpStructure::AnimationRotation* animRotation{
            new ImpStructure::AnimationRotation()
        };

        animRotation->_tick                 = map.first;
        animRotation->_rotQT_accumulation.x = (float)map.second[0];
        animRotation->_rotQT_accumulation.y = (float)map.second[1];
        animRotation->_rotQT_accumulation.z = (float)map.second[2];
        animRotation->_rotQT_accumulation.w = (float)map.second[3];

        Vector3 euler = animRotation->_rotQT_accumulation.ToEuler();

        impNode->_animation->_rotations.emplace_back(animRotation);
    }

    for (auto& map : animTrans->scale)
    {
        ImpStructure::AnimationScale* animScale{ new ImpStructure::AnimationScale() };

        animScale->_tick    = map.first;
        animScale->_scale.x = (float)map.second[0];
        animScale->_scale.y = (float)map.second[1];
        animScale->_scale.z = (float)map.second[2];

        impNode->_animation->_scales.emplace_back(animScale);
    }
}

void ImpAnimationObjLoader::LoadImpBone(ImpStructure::ImpAnimationNode* impNode,
    FbxSkeleton* skeleton)
{
    if (skeleton == nullptr) return;

    ImpStructure::Bone* impBone = new ImpStructure::Bone();

    impNode->_bone = impBone;

    impBone->_boneName = impNode->_name;
    // impBone->_boneTM_NodeTM  = impNode->_worldTransform;
    impBone->_boneTM_WorldTM = impNode->_worldTransform;

    _boneList.emplace_back(impBone);
}

void ImpAnimationObjLoader::LoadMeshes(ImpStructure::ImpAnimationNode* impNode,
    FbxMesh* fbxMesh)
{
    if (fbxMesh == nullptr) return;

    ImpStructure::ImpMesh* newMesh{ new ImpStructure::ImpMesh() };

    newMesh->_meshName = impNode->_name;

    LoadVertex(fbxMesh, newMesh);

    LoadFace(newMesh);

    LoadBoneWeight(fbxMesh, newMesh);

    // �׽�Ʈ��
    // Node TM �� LinkTM �ִ� ��
    for (auto& impBone : _boneList)
    {
        LoadBoneNodeTM(impBone, fbxMesh);
    }

    _meshList.emplace_back(newMesh);

    impNode->_mesh = newMesh;
}

void ImpAnimationObjLoader::LoadVertex(FbxMesh* fbxMesh,
    ImpStructure::ImpMesh* impMesh)
{
    int polygonCount          = fbxMesh->GetPolygonCount();
    FbxVector4* controlPoints = fbxMesh->GetControlPoints();
    std::vector<ImpStructure::ImpVertex> myMeshVertex;

    myMeshVertex.resize(polygonCount * 3);

    int vertexId{ 0 }; // i * 3 + j �� ����

    for (int i = 0; i < polygonCount; i++)
    {
        for (int j = 0; j < fbxMesh->GetPolygonSize(i); j++)
        {
            // ���ؽ��� Control Point ���
            int controlPointIndex = fbxMesh->GetPolygonVertex(i, j);

            /// �ε��� ������ �ִ´� ***

            myMeshVertex[vertexId]._controlPoint = controlPointIndex;
            myMeshVertex[vertexId]._vertexId     = vertexId;

            /// ----------------
            /// Vertex Position
            /// ----------------

            // �������� ��� ������ �غ��Ѵ�.
            FbxVector4 localPosition;

            if (controlPointIndex < 0)
            {
                continue;
            }
            else
            {
                localPosition = controlPoints[controlPointIndex];
                // ��ġ���� ��´�.
                myMeshVertex[i * 3 + j]._localPos = {
                    static_cast<float>(localPosition[0]),
                    static_cast<float>(localPosition[1]),
                    static_cast<float>(localPosition[2])
                };
            }

            /// --------------
            /// Vertex Color
            /// --------------

            // �÷����� ��� ������ �غ��Ѵ�.
            FbxColor vertexColor;

            int eColorCount = fbxMesh->GetElementVertexColorCount();

            // �÷��� ������ ���� ǥ���Ѵ�.
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

                // �÷� ���� ��´�.
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

            // UV �����͸� ��� ������ �غ��Ѵ�.
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
                                uv = leUV->GetDirectArray().GetAt(
                                    controlPointIndex);
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

                // uv ��ǥ���� ��´�.
                myMeshVertex[i * 3 + j]._u = static_cast<float>(uv[0]);
                myMeshVertex[i * 3 + j]._v = static_cast<float>(uv[1]);

            } // for PolygonSize

            /// --------------
            /// Vertex Normal
            /// --------------

            // �븻 ���� ��� ������ �̸� �غ��Ѵ�.
            FbxVector4 normal;

            int normalCount = fbxMesh->GetElementNormalCount();

            // �븻 ���� ���� ǥ���Ѵ�.
            if (normalCount) impMesh->_hasNormal = true;

            /// �븻�� ������ �������ش�. **

            if (impMesh->_hasNormal == false)
            {
                fbxMesh->InitNormals();
                fbxMesh->GenerateNormals();
            }

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

                    // �븻 ���� ��´�.
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

            // ź��Ʈ ���� ��� ������ �غ��Ѵ�.
            FbxVector4 tangent;

            int tangentCount = fbxMesh->GetElementTangentCount();

            if (tangentCount) impMesh->_hasTangent = true;

            /// ź��Ʈ, ���̳븻�� ������ �������ش�. ***

            if (impMesh->_hasNormal == true && impMesh->_hasTangent == false)
            {
                fbxMesh->InitTangents();
                fbxMesh->InitBinormals();
                fbxMesh->CreateElementBinormal();
                fbxMesh->GenerateTangentsData(0, true, false);
            }

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

                // ź��Ʈ ���� ��´�.
                myMeshVertex[i * 3 + j]._tangent = { static_cast<float>(tangent[0]),
                    static_cast<float>(tangent[2]),
                    static_cast<float>(tangent[1]) };

            } // for tangentCount


            /// ---------
            /// Binormal
            /// ---------

            // ���̳븻 ���� ��� ������ �غ��Ѵ�.

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

                // ���̳븻 ���� ��´�.
                myMeshVertex[i * 3 + j]._bitangent = {
                    static_cast<float>(bitangent[0]),
                    static_cast<float>(bitangent[1]),
                    static_cast<float>(bitangent[2])
                };

            } // for Binormal Count

            vertexId++;

        } // for polygonSize

    } // for polygonCount

    for (auto& vertex : myMeshVertex)
    {
        impMesh->_meshVertex.emplace_back(new ImpStructure::ImpVertex(vertex));
    }
}

void ImpAnimationObjLoader::LoadFace(ImpStructure::ImpMesh* impMesh)
{

    size_t vertexCount = impMesh->_meshVertex.size();
    size_t faceCount   = vertexCount / 3;

    impMesh->_meshFace.resize(faceCount);

    for (size_t i = 0; i < faceCount; i++)
    {
        impMesh->_meshFace[i] = new ImpStructure::ImpFace();

        for (size_t j = 0; j < 3; j++)
        {
            impMesh->_meshFace[i]->vertexIndex[j] = i * 3 + j;
        }
    }
}

void ImpAnimationObjLoader::LoadBoneNodeTM(ImpStructure::Bone* impNode,
    FbxMesh* fbxMesh)
{
    if (fbxMesh->GetDeformerCount(FbxDeformer::eSkin) <= 0) return;

    for (int i = 0; i < fbxMesh->GetDeformerCount(); i++)
    {
        FbxDeformer* fbxDeformer{ fbxMesh->GetDeformer(i) };

        if (fbxDeformer == nullptr) continue;

        switch (fbxDeformer->GetDeformerType())
        {
            case FbxDeformer::eSkin:
            {
                if ((FbxSkin*)fbxDeformer == nullptr) continue;
                // LoadBoneWeight((FbxSkin*)fbxDeformer, impMesh);

                FbxSkin* fbxSkin = (FbxSkin*)fbxDeformer;
                int clusterCount{ fbxSkin->GetClusterCount() };

                FbxCluster::ELinkMode linkMode{ FbxCluster::ELinkMode::eNormalize };

                for (int i = 0; i < clusterCount; i++)
                {
                    FbxCluster* cluster{ fbxSkin->GetCluster(i) };

                    auto temp = cluster->GetName();

                    // cluster �� �̸��� �����ϴ� ���� ��ġ�� �� ������.
                    // �׷��� �� �� �ִ��� Ȯ��
                    if (temp == nullptr || temp[0] == '\0')
                    {
                        const char* cluName = cluster->mBeforeVersion6LinkName.Buffer();
                        if (impNode->_boneName != cluName) continue;
                    }
                    else
                    {
                        const char* cluName = temp;
                        if (impNode->_boneName != cluName) continue;
                    }

                    FbxAMatrix clusterLinkedTransformMatrix;
                    cluster->GetTransformLinkMatrix(clusterLinkedTransformMatrix);

                    impNode->_boneTM_NodeTM = ConverImpMatrixFrom(
                        clusterLinkedTransformMatrix);
                }
            }
            break;

            default:
                break;
        }
    }
}

void ImpAnimationObjLoader::LoadBoneWeight(FbxMesh* fbxMesh,
    ImpStructure::ImpMesh* impMesh)
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

void ImpAnimationObjLoader::LoadBoneWeight(FbxSkin* fbxSkin,
    ImpStructure::ImpMesh* impMesh)
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

        int* lIndices    = cluster->GetControlPointIndices();
        double* lWeights = cluster->GetControlPointWeights();

        const char* clusterName = cluster->GetName();
        int boneIndex           = FindeBoneIndex(clusterName);

        for (int k = 0; k < cluster->GetControlPointIndicesCount(); k++)
        {
            for (auto& vertex : impMesh->_meshVertex)
            {
                if (vertex->_controlPoint != lIndices[k]) continue;

                // �� �ε��� �� ��ŭ
                for (int j = 0; j < 4; j++)
                {
                    if (vertex->_weightIndices[j] == (size_t)-1)
                    {
                        vertex->_weightIndices[j] = boneIndex;

                        switch (j)
                        {
                            case 0:
                                vertex->_weight.x = static_cast<float>(lWeights[k]);
                                break;
                            case 1:
                                vertex->_weight.y = static_cast<float>(lWeights[k]);
                                break;
                            case 2:
                                vertex->_weight.z = static_cast<float>(lWeights[k]);
                                break;
                            case 3:
                                vertex->_weightW = static_cast<float>(lWeights[k]);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                }
            }
        }
    }

    switch (linkMode)
    {
        case fbxsdk::FbxCluster::eNormalize:
            // ����ȭ
            {
                for (auto& vertex : impMesh->_meshVertex)
                {
                    float sum = vertex->_weight.x + vertex->_weight.y +
                                vertex->_weight.z + vertex->_weightW;

                    // �̰�... �³�?
                    if (std::abs(sum - 1.f) != 0) // sum == 0
                    {
                        vertex->_weight.x /= sum;
                        vertex->_weight.y /= sum;
                        vertex->_weight.z /= sum;
#ifdef _DEBUG
                        vertex->_weightW /= sum;
#endif
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

int ImpAnimationObjLoader::FindeBoneIndex(std::string name)
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

void ImpAnimationObjLoader::Display(ImpStructure::ImpAnimationObject* impAnimObj)
{
    for (auto& node : impAnimObj->_nodes)
    {
        Display(node.second);
    }
}

void ImpAnimationObjLoader::Display(ImpStructure::ImpAnimationNode* impAnimNode)
{

    FBXSDK_printf(
        "// "
        "----------------------------------------------------------------------"
        "\n\n");

    FBXSDK_printf("��� �̸�:\t%s\n", impAnimNode->_name.c_str());

    if (impAnimNode->_parentNode != nullptr)
    {
        FBXSDK_printf(
            "�θ� ��� �̸�:\t%s\n", impAnimNode->_parentNode->_name.c_str());
    }

    FBXSDK_printf(
        "�޽� ����:\t%s\n", impAnimNode->_mesh != nullptr ? "True" : "False");

    FBXSDK_printf("���̷��� ����:\t%s\n",
        impAnimNode->_bone != nullptr ? "True" : "False");

   //DisplayNodeTM(impAnimNode);

    FBXSDK_printf("\n");

    //DisplayWeight(impAnimNode);

    FBXSDK_printf("\n");

    //DisplayTransform(impAnimNode);

    FBXSDK_printf("\n");

    DisplayAnimation(impAnimNode);

    FBXSDK_printf("\n");
}

void ImpAnimationObjLoader::DisplayNodeTM(ImpStructure::ImpAnimationNode* impAnimNode)
{
    if (impAnimNode->_bone == nullptr) return;

    const auto& boneTM = impAnimNode->_bone->_boneTM_NodeTM;
    FBXSDK_printf("��� ��ȯ ���\n");
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f \n",
        impAnimNode->_bone->_boneTM_NodeTM._11,
        impAnimNode->_bone->_boneTM_NodeTM._12,
        impAnimNode->_bone->_boneTM_NodeTM._13,
        impAnimNode->_bone->_boneTM_NodeTM._14);
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f \n",
        impAnimNode->_bone->_boneTM_NodeTM._21,
        impAnimNode->_bone->_boneTM_NodeTM._22,
        impAnimNode->_bone->_boneTM_NodeTM._23,
        impAnimNode->_bone->_boneTM_NodeTM._24);
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f \n",
        impAnimNode->_bone->_boneTM_NodeTM._31,
        impAnimNode->_bone->_boneTM_NodeTM._32,
        impAnimNode->_bone->_boneTM_NodeTM._33,
        impAnimNode->_bone->_boneTM_NodeTM._34);
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f \n",
        impAnimNode->_bone->_boneTM_NodeTM._41,
        impAnimNode->_bone->_boneTM_NodeTM._42,
        impAnimNode->_bone->_boneTM_NodeTM._43,
        impAnimNode->_bone->_boneTM_NodeTM._44);
}

void ImpAnimationObjLoader::DisplayWeight(ImpStructure::ImpAnimationNode* impAnimNode)
{
    if (impAnimNode->_mesh == nullptr) return;

    FBXSDK_printf("����ġ �� ����:\t%s\n",
        impAnimNode->_mesh->_hasWeight == true ? "True" : "False");

    if (impAnimNode->_mesh->_hasWeight)
    {
        auto sdfs = impAnimNode->_mesh->_meshVertex.size();

        for (auto& vertex : impAnimNode->_mesh->_meshVertex)
        {
            FBXSDK_printf(
                "    %-d - %-4d .... ", vertex->_vertexId, vertex->_controlPoint);

            // �� �ε��� �� ��ŭ
            for (int j = 0; j < 4; j++)
            {
                if (vertex->_weightIndices[j] != (size_t)-1)
                {
                    FBXSDK_printf("( %zu ", vertex->_weightIndices[j]);

                    switch (j)
                    {
                        case 0:
                            FBXSDK_printf(": %f )\t", vertex->_weight.x);
                            break;
                        case 1:
                            FBXSDK_printf(": %f )\t", vertex->_weight.y);
                            break;
                        case 2:
                            FBXSDK_printf(": %f )\t", vertex->_weight.z);
                            break;
                        case 3:
                            FBXSDK_printf(": %f )\t", vertex->_weightW);
                            break;
                        default:
                            break;
                    }
                }
            }
            FBXSDK_printf("\n");
        }
    }
}

void ImpAnimationObjLoader::DisplayTransform(
    ImpStructure::ImpAnimationNode* impAnimNode)
{
    FBXSDK_printf("���� ��ȯ ���:\n");
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f\n",
        impAnimNode->_worldTransform._11,
        impAnimNode->_worldTransform._12,
        impAnimNode->_worldTransform._13,
        impAnimNode->_worldTransform._14);
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f\n",
        impAnimNode->_worldTransform._21,
        impAnimNode->_worldTransform._22,
        impAnimNode->_worldTransform._23,
        impAnimNode->_worldTransform._24);
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f\n",
        impAnimNode->_worldTransform._31,
        impAnimNode->_worldTransform._32,
        impAnimNode->_worldTransform._33,
        impAnimNode->_worldTransform._34);
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f\n",
        impAnimNode->_worldTransform._41,
        impAnimNode->_worldTransform._42,
        impAnimNode->_worldTransform._43,
        impAnimNode->_worldTransform._44);

    FBXSDK_printf("���� ��ȯ ���:\n");
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f\n",
        impAnimNode->_localTransform._11,
        impAnimNode->_localTransform._12,
        impAnimNode->_localTransform._13,
        impAnimNode->_localTransform._14);
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f\n",
        impAnimNode->_localTransform._21,
        impAnimNode->_localTransform._22,
        impAnimNode->_localTransform._23,
        impAnimNode->_localTransform._24);
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f\n",
        impAnimNode->_localTransform._31,
        impAnimNode->_localTransform._32,
        impAnimNode->_localTransform._33,
        impAnimNode->_localTransform._34);
    FBXSDK_printf("\t%9.4f %9.4f %9.4f %9.4f\n",
        impAnimNode->_localTransform._41,
        impAnimNode->_localTransform._42,
        impAnimNode->_localTransform._43,
        impAnimNode->_localTransform._44);

    FBXSDK_printf("\n");
}

void ImpAnimationObjLoader::DisplayAnimation(
    ImpStructure::ImpAnimationNode* impAnimNode)
{
    if (impAnimNode->_animation == nullptr) return;

    if (!impAnimNode->_animation->_positions.empty())
        FBXSDK_printf("�̵� ��ȯ �ִϸ��̼�\n");
    for (auto& pos : impAnimNode->_animation->_positions)
    {
        FBXSDK_printf("\ttick %9zu .... position: %9.4f, %9.4f, %9.4f\n",
            pos->_tick,
            pos->_pos.x,
            pos->_pos.y,
            pos->_pos.z);
    }

    FBXSDK_printf("\n");

    if (!impAnimNode->_animation->_rotations.empty())
        FBXSDK_printf("ȸ�� ��ȯ �ִϸ��̼�\n");
    for (auto& rot : impAnimNode->_animation->_rotations)
    {
        FBXSDK_printf("\ttick %9zu .... rotaion: %9.4f, %9.4f, %9.4f, %9.4f\n",
            rot->_tick,
            rot->_rotQT_accumulation.x,
            rot->_rotQT_accumulation.y,
            rot->_rotQT_accumulation.z,
            rot->_rotQT_accumulation.w);
    }

    FBXSDK_printf("\n");

    if (!impAnimNode->_animation->_scales.empty())
        FBXSDK_printf("ũ�� ��ȯ �ִϸ��̼�\n");
    for (auto& scl : impAnimNode->_animation->_scales)
    {
        FBXSDK_printf("\ttick %9zu .... scale: %9.4f, %9.4f, %9.4f\n",
            scl->_tick,
            scl->_scale.x,
            scl->_scale.y,
            scl->_scale.z);
    }
}

ImpMath::Matrix ImpAnimationObjLoader::LoadGlobalTransform(FbxNode* fbxNode)
{
    FbxAMatrix fbxMatrix = fbxNode->EvaluateGlobalTransform();

    return ConverImpMatrixFrom(fbxMatrix);
}

ImpMath::Matrix ImpAnimationObjLoader::LoadLocalTransform(FbxNode* fbxNode)
{
    FbxAMatrix fbxMatrix = fbxNode->EvaluateLocalTransform();

    return ConverImpMatrixFrom(fbxMatrix);
}

ImpMath::Matrix ImpAnimationObjLoader::ConverImpMatrixFrom(
    const FbxAMatrix& fbxMatrix)
{
    // ��ǥ ���� �ٲٷ��� �ǵ��ε� �� �� �ȵȴ�

    /*
    FbxVector4 trn = fbxMatrix.GetT();
    FbxVector4 rot = fbxMatrix.GetR();
    FbxVector4 scl = fbxMatrix.GetS();
    FbxVector4 newTranslation{ trn[0], trn[2], trn[1], trn[3] };
    FbxVector4 newRotation{
        rot[0] * (90 * ImpMath::PI / 180.f), rot[1], rot[2], rot[3]
    };
    FbxVector4 newScale{ scl[0], scl[2], scl[1], scl[3] };
    */

    // newMatrix.SetTRS(newTranslation, newRotation, newScale);

    FbxAMatrix newMatrix = fbxMatrix;

    ImpMath::Matrix impMatrix;
    impMatrix._11 = static_cast<float>(newMatrix.Get(0, 0));
    impMatrix._21 = static_cast<float>(newMatrix.Get(1, 0));
    impMatrix._31 = static_cast<float>(newMatrix.Get(2, 0));
    impMatrix._41 = static_cast<float>(newMatrix.Get(3, 0));

    impMatrix._12 = static_cast<float>(newMatrix.Get(0, 1));
    impMatrix._22 = static_cast<float>(newMatrix.Get(1, 1));
    impMatrix._32 = static_cast<float>(newMatrix.Get(2, 1));
    impMatrix._42 = static_cast<float>(newMatrix.Get(3, 1));

    impMatrix._13 = static_cast<float>(newMatrix.Get(0, 2));
    impMatrix._23 = static_cast<float>(newMatrix.Get(1, 2));
    impMatrix._33 = static_cast<float>(newMatrix.Get(2, 2));
    impMatrix._43 = static_cast<float>(newMatrix.Get(3, 2));

    impMatrix._14 = static_cast<float>(newMatrix.Get(0, 3));
    impMatrix._24 = static_cast<float>(newMatrix.Get(1, 3));
    impMatrix._34 = static_cast<float>(newMatrix.Get(2, 3));
    impMatrix._44 = static_cast<float>(newMatrix.Get(3, 3));

    return impMatrix;
}

void ImpAnimationObjLoader::SetParentNodeAddress(const FbxNode* fbxNode,
    ImpStructure::ImpAnimationNode* impAnimNode,
    std::unordered_map<std::string, ImpStructure::ImpAnimationNode*>& impNode)
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

    // �θ� ����� �ּ� ����
    impAnimNode->_parentNode = parentImpNode;
    // �θ� ����� �ڽ� ��� �߰�
    parentImpNode->_childNodes.push_back(impAnimNode);
}

} // namespace FBXLoad