//------------------------------------------------------------
//
// ASE Parser
// Ver 2.1.20080124
// 
// 자체적으로 데이터 구조가 있으며 
// 이것을 D3D용 특정 오브젝트로 변환하는 곳까지 커버한다
//
//------------------------------------------------------------

enum eObjectType
{
	eObjectType_ObjectRoot, eObjectType_Geomobject, eObjectType_Shape,
	eObjectType_Camera, eObjectType_Light, eObjectType_HelperObject,
	eObjectType_MarkerObject, eObjectType_SkinnedMesh, eObjectType_Bone,
	eObjectType_CMeshPlane, eObjectType_CBullet_Trace,
	//
	eObjectType_BasicSphere, eObjectType_Missile,
	eObjectType_MeshBillboard, eObjectType_PointSprite,			// CMesh상속..
	eObjectType_End,
};

// 교수님이 제공해주신 Lexer
#include "ASEFile.h"

// 파서 내부에서 쓰는 데이터형
#include "CParsingDataClass.h"

#include <list>

using namespace std;
using namespace ASEParser;


// 파서 내부에서 특정 모드를 가지고 작동한다. (파서의 재귀구조 때문)
enum eParsingmode
{
	eNone, eGeomobject, eAnimation, eShape, eCamera, eLight, eHelperObject,
	eSkin_InitTM, eBone_List,
	eEnd,
};

#include "IASELoader.h"

/// ASE를 파싱하기 위한 클래스. 기본적으로 주어지는 Lexer클래스를 이용한다.

/// <summary>
/// 학생 때의 ASE Parser를 예시용으로 약간 개조했다.
///
/// 2021.04.12 LeHideKGIT
/// </summary>
class CASEParser : public ASELoader::IASELoader
{
public:
	CASEParser();
	virtual ~CASEParser();

	// ------------------------------ Interface -----------------------------
	virtual ImpStructure::ImpMesh* GetMesh(const char* file, size_t i) override;
	virtual std::vector<ImpStructure::ImpMesh*> GetMeshAll(const char* file) override;

	virtual ImpStructure::ImpAnimationObject* GetAnimationObject(const char* file) override;

	virtual size_t GetMeshSize(const char* file) override;
	virtual std::vector<ImpStructure::Bone*> GetBoneList() override;
private:
	std::vector<ImpStructure::ImpMesh*> _meshes;
	ImpStructure::ImpAnimationObject* _object;

	void MakeImpMesh();
	void MakeImpObject();
	//------------------------------------------------------------------------

	std::vector<ImpStructure::Bone*> _bones;
public:
	/// 초기화, 로드
	bool Init();
	bool Load(LPSTR p_File);		// ASE파일을 읽어서, 자체 리스트에 넣는다.

	/// 1차 변환
	// CScenedata 변환
	bool TranslateToD3DFormat_scene(Scenedata* pscene);

	bool Convert_Optimize(Mesh* pMesh);		// 버텍스(노말), 텍스쳐 값으로 최적화를 해 준다.
	bool ConvertAll(Mesh* pMesh);			// 모두 그냥 변환 (예시용)

	/// Yujin 추가
	void LerpAnimation();
	void MakeFaceTangent();
private:
	// 토큰을 구별해내기 위한 스트링
	char m_TokenString[256];
	int m_Token;
	int m_parsingmode;				// 지금 모드. 재귀 내의 동일 변수때문. nodename이라던지.. row123, mapname등등..


public:
	/// 기본적인 읽어들인 내용들
	int m_data_asciiexport;			// *3DSMAX_ASCIIEXPORT	200 - 맨 처음 나오는 헤더인데 무슨 의미인지는 모르겠고, 일단 예의상 읽어서 보관해준다.
	Scenedata m_scenedata;			// 현재 Scene의 데이터가 들어갈 구조체

	/// 매트리얼 관련
	int	m_materialcount;					// Material이 총 몇개인가?
	ASEMaterial* m_materialdata;			// Material의 데이터가 들어갈 구조체
	list<ASEMaterial*> m_list_materialdata;	// Material들의 리스트

	/// 한 씬에 N개의 메시가 존재할 수 있다.
	Mesh* m_OneMesh;						// 메시 하나의 데이터가 들어갈 구조체
	vector<Mesh*> m_MeshList;				// 메시들

	/// 쉐이프 오브젝트를 파싱할 때 필요한 중간 변수
	//( 파서 내부에서 쉐이프 오브젝트는 GeomObject와 동일하게 취급된다)
	ShapeLine* m_nowshapeline;				// 현재 읽고 있는 Line
	ShapeVertex* m_nowshapevertex;			// 현재 읽고 있는 ShapeVertex

	/// 애니메이션
// 	Animation* m_animation;					// 한 오브젝트의 애니메이션 데이터(파싱용)
// 	list<Animation*> m_list_animation;		// 그 데이터의 리스트

	/// Yujin 추가
	Object* m_object;
	std::vector<std::string> m_addOrder = {};
	std::vector<Animation*> m_animations = {};

	Bone* m_bone;									// Bone 한개의 포인터 (파서 내부에서 임시 공간으로 쓰임)
	VertexWeight* m_wvertex;						// Weight_Vertex 한개의 포인터 (마찬가지로 임시공간)
	vector<Bone*>			m_vector_bone_list;		// 이건 그냥 번호와 이름 관계이다. 이 안에 포인터를 넣어놓아도 괜찮겠구나(참조하기 편하게 - 그렇지 않으면 매번 이름으로 검색을 해야될지도. 아니면 인덱스가 있으니 상수 시간대로 접근을 할수도?)
	vector<VertexWeight*>	m_vector_wvertexs;		// 버텍스 하나에 달라붙어있는 가중치들
	std::vector<Weight*> m_vector_weight;

	// 분리할 버텍스를 저장해 둘 구조, 기준이 되는 버텍스를 놓고 새로 만든 버텍스를 저장한다. vector가 최선인지는 전혀 모르겠지만
	// 이렇게 하면 n log n 시간 복잡도가 나올 것 같다.
	//std::unordered_map<unsigned int, std::vector<unsigned int>> m_savedVertexSplit;

	// multimap 쓰면 된다.
	//std::unordered_multimap<unsigned int, unsigned int> m_savedVertexSplit;
public:
	//--------------------------------------------------
	// Export
	//--------------------------------------------------
	Mesh* GetMesh(int index);
	Node* GetNode(string nodeName);
	int GetSizeMeshList() { return m_MeshList.size(); }

public:
	//--------------------------------------------------
	//	Lexcer
	//--------------------------------------------------
private:
	ASE::CASELexer* m_lexer;


	//--------------------------------------------------
	// Parsing_DivergeRecursiveALL() 내부에서 쓰기 위한 함수들.
	// (parsing을 위한 단위별 함수들)
	//--------------------------------------------------
private:
	/// 파싱을 맡게 되는 거대한 재귀함수. 여기서 모든 토큰의 처리를 한다.
	void Parsing_DivergeRecursiveALL(int depth);

	int Parsing_NumberLong();			// long을 읽어서 리턴해준다.
	float Parsing_NumberFloat();		// float
	LPSTR Parsing_String();				// string
	int Parsing_NumberInt();			// int
	DirectX::SimpleMath::Vector3 Parsing_NumberVector3();	// 3개의 Float를 벡터 하나로 읽어서 리턴해준다.

	void Create_onemesh_to_list();		// 메시를 하나 동적으로 생성하고, 리스트에 넣는다.
	void Create_materialdata_to_list();	// 메트리얼 하나를
	void Create_animationdata_to_list();// 애니메이션 데이터(한 개의 오브젝트 대응) 하나를 동적으로 생성하고, 리스트에 넣는다.
	void Create_onevertex_to_list();	// 버텍스 하나를
};