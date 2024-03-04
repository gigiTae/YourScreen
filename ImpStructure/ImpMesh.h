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
    ImpMath::Vector3 _weight; // x, y, z + 나머지 1개 합쳐서 1.0f가 되어야 함
    size_t _weightIndices[4] = { (size_t)-1, (size_t)-1, (size_t)-1, (size_t)-1 }; // 최대 4개

    // weight 값을 위한 중간 데이터 - KJY
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

    // _VertexId 를 기준으로 정렬하기 위한 < 연산자 정의
    bool operator<(const ImpVertex& other) const
    {
        return _vertexId < other._vertexId;
    }
};

struct ImpFace
{
    // 기본 인덱스를 읽은 다음에
    // 버텍스 스플릿을 해서 직접 기존의 인덱스들을 변경해야함
    size_t vertexIndex[3];
};

struct ImpMesh
{
    // temp ImpVertex와 ImpFace를 스마트 포인터로 바꾸는 게 좋겠다.
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

    // 기존 메시 1개를 띄우기 위한 데이터
    std::vector<ImpVertex*> _meshVertex;
    std::vector<ImpFace*> _meshFace;

    bool _hasTexture = false; // 텍스처 유무
    bool _hasNormal  = false; // 노말
    bool _hasTangent = false; // 탄젠트 (노말맵 사용)
    bool _hasColor   = false; // 버텍스 컬러
    bool _hasWeight  = false; // 스키닝
};

/// <summary>
/// ---------------------------------------Skinning-------------------------------
/// </summary>

struct Bone
{
    std::string _boneName; // 식별자, 나는 ASE에서 파싱을 위해 썼다. 이후에는 Node로 묶여서 안 쓴듯?

    ImpMath::Matrix _boneTM_NodeTM; // 이녀석은 처음의 NodeTM이다 - offset을 구하기 위해서 존재한다.
    ImpMath::Matrix _boneTM_WorldTM; // 이녀석은 애니메이션이 적용된, 마지막 WorldTM이다
};

/// <summary>
/// ---------------------------------------Animation-------------------------------
/// </summary>

struct AnimationPosition
{
    size_t _tick;

    ImpMath::Vector3 _pos; // 실제 움직임
};

struct AnimationRotation
{
    size_t _tick;

    ImpMath::Quaternion _rotQT_accumulation; // 상대변량이기 때문에 이전 값들과
                                             // 누적해서 가지고 있는다 (즉
                                             // 지금까지의 절대량으로 변환한다)
};

struct AnimationScale
{
    size_t _tick;

    ImpMath::Vector3 _scale; // 이거면 충분하지 않을까?
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

    // 그냥 트랜스폼으로 한방에 불러오면 안될까?
    // 아닌가 디버깅을 위해서는 쪼개는 게 맞나
    std::vector<AnimationPosition*> _positions;
    std::vector<AnimationRotation*> _rotations;
    std::vector<AnimationScale*> _scales;

    // 디버깅 할 때 둘다 가지고 있고 Release로 뽑을 때는 TM만 뽑는 걸로
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
    std::string _name; // 식별자
    ImpMesh* _mesh;    // 있을 수도 있고 없을 수도 있다

    ImpMath::Matrix _localTransform;
    ImpMath::Matrix _worldTransform;

    ImpAnimationNode* _parentNode;              // 부모 노드
    std::vector<ImpAnimationNode*> _childNodes; // 자식 노드

    Bone* _bone; // 만약 내가 Bone이라면 여기에 본을 넣어줘야 한다.
    ImpAnimation* _animation; // 애니메이션 틱 - 값

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

    float _maxAnimTime; // 마지막 재생 프레임 또는 틱 정보

    std::unordered_map<std::string, ImpAnimationNode*> _nodes; // 하나의 커다란 애니메이션이 되는 오브젝트
};
} // namespace ImpStructure

/// 주의점:
///   - texture는 하나만 있다는 가정하에 파싱된다.
///