#pragma once
#include "ImpMath.h"
#include <vector>
#include <unordered_map>
#include <string>

using namespace ImpMath;

namespace ImpStructure
{
struct ImpVertex
{
    ImpMath::Vector3 _localPos;

    float _u = 0, _v = 0; // texture
    // std::vector<Vector2> _uvs;

    ImpMath::Vector3 _normal;
    ImpMath::Vector3 _tangent;   // normalMap
    ImpMath::Vector3 _bitangent; // normalMap
    ImpMath::Vector4 _color;
    ImpMath::Vector3 _weight; // x, y, z + ������ 1�� ���ļ� 1.0f�� �Ǿ�� ��
    size_t _weightIndices[4] = { (size_t)-1, (size_t)-1, (size_t)-1, (size_t)-1 }; // �ִ� 4��

    // weight ���� ���� �߰� ������ - KJY
    unsigned int _controlPoint;
    int _vertexId;
    float _weightW;
    std::unordered_map<std::string, float> _boneData;

    bool operator==(const ImpVertex& other) const
    {
        return _localPos == other._localPos &&
               _normal == other._normal &&
               _tangent == other._tangent && _bitangent == other._bitangent &&
               _color == other._color && _weight == other._weight &&
               _controlPoint == other._controlPoint; 
    }

    // _VertexId �� �������� �����ϱ� ���� < ������ ����
    bool operator<(const ImpVertex& other) const
    {
        return _vertexId < other._vertexId;
    }
};

struct ImpFace
{
    // �⺻ �ε����� ���� ������
    // ���ؽ� ���ø��� �ؼ� ���� ������ �ε������� �����ؾ���
    size_t vertexIndex[3];
};

struct ImpMesh
{
    // temp ImpVertex�� ImpFace�� ����Ʈ �����ͷ� �ٲٴ� �� ���ڴ�.
    ~ImpMesh()
    {
        for (const auto& vertex : _meshVertex)
        {
            delete vertex;
        }
        for (const auto& face : _meshFace)
        {
            delete face;
        }
    }

    std::string _meshName;

    // ���� �޽� 1���� ���� ���� ������
    std::vector<ImpVertex*> _meshVertex;
    std::vector<ImpFace*> _meshFace;

    bool _hasTexture = false; // �ؽ�ó ����
    bool _hasNormal  = false; // �븻
    bool _hasTangent = false; // ź��Ʈ (�븻�� ���)
    bool _hasColor   = false; // ���ؽ� �÷�
    bool _hasWeight  = false; // ��Ű��
};

/// <summary>
/// ---------------------------------------Skinning-------------------------------
/// </summary>

struct Bone
{
    std::string _boneName; // �ĺ���, ���� ASE���� �Ľ��� ���� ���. ���Ŀ��� Node�� ������ �� ����?

    ImpMath::Matrix _boneTM_NodeTM; // �̳༮�� ó���� NodeTM�̴� - offset�� ���ϱ� ���ؼ� �����Ѵ�.
    ImpMath::Matrix _boneTM_WorldTM; // �̳༮�� �ִϸ��̼��� �����, ������ WorldTM�̴�
};

/// <summary>
/// ---------------------------------------Animation-------------------------------
/// </summary>

struct AnimationPosition
{
    size_t _tick;

    ImpMath::Vector3 _pos; // ���� ������
};

struct AnimationRotation
{
    size_t _tick;

    ImpMath::Quaternion _rotQT_accumulation; // ��뺯���̱� ������ ���� �����
                                             // �����ؼ� ������ �ִ´� (��
                                             // ���ݱ����� ���뷮���� ��ȯ�Ѵ�)
};

struct AnimationScale
{
    size_t _tick;

    ImpMath::Vector3 _scale; // �̰Ÿ� ������� ������?
                             // 		ImpMath::Vector3		m_scaleaxis;
                             // 		float			m_scaleaxisang;
                             // 		ImpMath::Quaternion m_scalerotQT_accumulation;
};

struct AnimationTransform
{
    size_t _tick;

    ImpMath::Matrix _transformTM;
};

struct ImpAnimation
{
    // temp
    ~ImpAnimation()
    {
        for (const auto& pos : _positions)
        {
            delete pos;
        }
        for (const auto& rot : _rotations)
        {
            delete rot;
        }
        for (const auto& scl : _scales)
        {
            delete scl;
        }
        for (const auto& tm : _transformTMs)
        {
            delete tm;
        }
    }

    // �׳� Ʈ���������� �ѹ濡 �ҷ����� �ȵɱ�?
    // �ƴѰ� ������� ���ؼ��� �ɰ��� �� �³�
    std::vector<AnimationPosition*> _positions;
    std::vector<AnimationRotation*> _rotations;
    std::vector<AnimationScale*> _scales;

    // ����� �� �� �Ѵ� ������ �ְ� Release�� ���� ���� TM�� �̴� �ɷ�
    std::vector<AnimationTransform*> _transformTMs;
};

struct ImpAnimationNode
{
    ImpAnimationNode(ImpMesh* mesh = nullptr)
        : _mesh(mesh)
        , _childNodes({})
        , _parentNode(nullptr)
        , _animation(/*nullptr*/)
        , _bone(nullptr){};
    // temp
    ~ImpAnimationNode()
    {
        //delete _mesh;
        delete _animation;
    }
    std::string _name; // �ĺ���
    ImpMesh* _mesh;    // ���� ���� �ְ� ���� ���� �ִ�

    ImpMath::Matrix _localTransform;
    ImpMath::Matrix _worldTransform;

    ImpAnimationNode* _parentNode;              // �θ� ���
    std::vector<ImpAnimationNode*> _childNodes; // �ڽ� ���

    Bone* _bone; // ���� ���� Bone�̶�� ���⿡ ���� �־���� �Ѵ�.
    ImpAnimation* _animation; // �ִϸ��̼� ƽ - ��

    void SetNode(ImpMath::Matrix parentMat);

    void Translate(ImpMath::Vector3 distance);
    void Rotate(ImpMath::Quaternion nextRotation);
};

struct ImpAnimationObject
{
    // temp
    ~ImpAnimationObject()
    {
        for (const auto& node : _nodes)
        {
            delete node.second;
        }
    }

    float _maxAnimTime; // ������ ��� ������ �Ǵ� ƽ ����

    std::unordered_map<std::string, ImpAnimationNode*> _nodes; // �ϳ��� Ŀ�ٶ� �ִϸ��̼��� �Ǵ� ������Ʈ
};
} // namespace ImpStructure

/// ������:
///   - texture�� �ϳ��� �ִٴ� �����Ͽ� �Ľ̵ȴ�.
///