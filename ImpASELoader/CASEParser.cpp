/*
#include "CASEParser.h"


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다.
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다.
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련. 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자.
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	/// 1) 다롱이아부지김교수의 파워풀한 재귀함수 한개로 처리 끝!
	m_object = new Object;
	Parsing_DivergeRecursiveALL(0);
	MakeFaceTangent();
	LerpAnimation();

	// 버텍스의 중복을 확인하기 위해 사용했던 것
// 	// 버텍스에 중복이 있다. 지금 구조에서는 당연한 것.
// 	for (int i = 0; i < m_OneMesh->m_meshvertex.size(); i++)
// 	{
// 		Vector3 pos = m_OneMesh->m_meshvertex[i]->m_pos;
// 		Vector3 normal = m_OneMesh->m_meshvertex[i]->m_normal;
// 		for (int j = 0; j < m_OneMesh->m_meshvertex.size(); j++)
// 		{
// 			if (i == j)
// 			{
// 				continue;;
// 			}
// 
// 			if (pos == m_OneMesh->m_meshvertex[j]->m_pos && normal == m_OneMesh->m_meshvertex[j]->m_normal)
// 			{
// 				int a = 3;
// 			}
// 		}
// 	}

	return TRUE;
}

/// 1차 변환

// CScenedata 복사
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// 값 복사
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// 구현 제거 (예시용)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// 버텍스들은 일단 모두 복사
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;
		_pVertex->m_normal = pMesh->m_meshvertex[i]->m_normal;
		_pVertex->m_tangent = pMesh->m_meshvertex[i]->m_tangent;

		_pVertex->u = pMesh->m_meshvertex[i]->u;
		_pVertex->v = pMesh->m_meshvertex[i]->v;

		_pVertex->m_bw1 = pMesh->m_meshvertex[i]->m_bw1;
		_pVertex->m_bw2 = pMesh->m_meshvertex[i]->m_bw2;
		_pVertex->m_bw3 = pMesh->m_meshvertex[i]->m_bw3;

		_pVertex->m_weightIndices[0] = pMesh->m_meshvertex[i]->m_weightIndices[0];
		_pVertex->m_weightIndices[1] = pMesh->m_meshvertex[i]->m_weightIndices[1];
		_pVertex->m_weightIndices[2] = pMesh->m_meshvertex[i]->m_weightIndices[2];
		_pVertex->m_weightIndices[3] = pMesh->m_meshvertex[i]->m_weightIndices[3];

		pMesh->m_opt_vertex.push_back(_pVertex);
	}

	//TODO: 왜 여기를 이렇게 해 두었을까?
// 	// 나머지는 face를 기준으로 한 인덱스로 찾아야 함
// 	unsigned int _faceCount = pMesh->m_meshface.size();
// 	TRACE("FaceCount : %d", _faceCount);
// 
// 	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
// 	{
// 		TRACE("FaceNum : %d / %d\n", i, _faceCount);
// 
// 		for (int j = 0; j < 3; j++)
// 		{
// 			Vertex* _nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];
// 
// 			// face기준으로 버텍스의 노말을 넣어준다.
// 			// (예시용)
// 			_nowVertex->m_normal.x = 0;
// 			_nowVertex->m_normal.y = 0;
// 			_nowVertex->m_normal.z = 0;
// 		}
// 	}

	// 인덱스는 그냥 복사
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	return FALSE;
}

void CASEParser::LerpAnimation()
{
	for (int i = 0; i < m_animations.size(); i++)
	{
		std::vector<CAnimation_pos*> newPosAnimation = {};
		if (m_animations[i]->m_position.size() != 0)
		{
			for (int k = 0; k < m_scenedata.m_lastframe; k++)
			{
				newPosAnimation.push_back(new CAnimation_pos);
				newPosAnimation[k]->m_time = k * m_scenedata.m_ticksperframe;
			}

			unsigned int curIndex = 0;
			for (int j = 0; j < m_animations[i]->m_position.size(); j++)
			{
				int firstTick;
				int secondTick;

				DirectX::SimpleMath::Vector3 firstPos;
				DirectX::SimpleMath::Vector3 secondPos;

				if (j == m_animations[i]->m_position.size() - 1)
				{
					firstTick = m_animations[i]->m_position[j]->m_time;
					secondTick = m_scenedata.m_lastframe * m_scenedata.m_ticksperframe;

					firstPos = m_animations[i]->m_position[j]->m_pos;
					secondPos = m_animations[i]->m_position[0]->m_pos;
				}
				else
				{
					firstTick = m_animations[i]->m_position[j]->m_time;
					secondTick = m_animations[i]->m_position[j + 1]->m_time;

					firstPos = m_animations[i]->m_position[j]->m_pos;
					secondPos = m_animations[i]->m_position[j + 1]->m_pos;
				}

				float t;

				while (curIndex * m_scenedata.m_ticksperframe < secondTick)
				{
					if (firstTick == curIndex * m_scenedata.m_ticksperframe) t = 0;
					else
					{
						t = (float)(curIndex * m_scenedata.m_ticksperframe - firstTick) / (float)(secondTick - firstTick);
					}

					newPosAnimation[curIndex]->m_pos = newPosAnimation[curIndex]->m_pos.Lerp(firstPos, secondPos, t);
					curIndex++;
				}
			}
			m_animations[i]->m_position = newPosAnimation;
		}

		std::vector<CAnimation_rot*> newRotAnimation = {};

		if (m_animations[i]->m_rotation.size() != 0)
		{
			// 			m_animations[i]->m_rotation[0]->m_rotQT_accumulation
			// 				= m_animations[i]->m_rotation[1]->m_rotQT_accumulation;
			for (int k = 0; k < m_scenedata.m_lastframe; k++)
			{
				newRotAnimation.push_back(new CAnimation_rot);
				newRotAnimation[k]->m_time = k * m_scenedata.m_ticksperframe;
			}

			unsigned int curIndex = 0;
			for (int j = 0; j < m_animations[i]->m_rotation.size(); j++)
			{
				int firstTick;
				int secondTick;

				DirectX::SimpleMath::Quaternion firstQuat;
				DirectX::SimpleMath::Quaternion secondQuat;

				if (j == m_animations[i]->m_rotation.size() - 1)
				{
					firstTick = m_animations[i]->m_rotation[j]->m_time;
					secondTick = m_scenedata.m_lastframe * m_scenedata.m_ticksperframe;

					firstQuat = m_animations[i]->m_rotation[j]->m_rotQT_accumulation;
					secondQuat = m_animations[i]->m_rotation[0]->m_rotQT_accumulation;
				}
				else
				{
					firstTick = m_animations[i]->m_rotation[j]->m_time;
					secondTick = m_animations[i]->m_rotation[j + 1]->m_time;

					firstQuat = m_animations[i]->m_rotation[j]->m_rotQT_accumulation;
					secondQuat = m_animations[i]->m_rotation[j + 1]->m_rotQT_accumulation;
				}

				float t;

				while (curIndex * m_scenedata.m_ticksperframe < secondTick)
				{
					if (firstTick == curIndex * m_scenedata.m_ticksperframe) t = 0;
					else
					{
						t = (float)(curIndex * m_scenedata.m_ticksperframe - firstTick) / (float)(secondTick - firstTick);
					}

					newRotAnimation[curIndex]->m_rotQT_accumulation = newRotAnimation[curIndex]->m_rotQT_accumulation.Slerp(firstQuat, secondQuat, t);
					curIndex++;
				}
			}
			m_animations[i]->m_rotation = newRotAnimation;
		}
	}
}

void CASEParser::MakeFaceTangent()
{
	Mesh* curMesh = nullptr;
	for (int i = 0; i < m_MeshList.size(); i++)
	{
		curMesh = m_MeshList[i];

		for (int j = 0; j < curMesh->m_meshface.size(); j++)
		{
			XMFLOAT3 v0 = curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[0]]->m_pos;
			XMFLOAT3 v1 = curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[1]]->m_pos;
			XMFLOAT3 v2 = curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[2]]->m_pos;

			float tu0 = curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[0]]->u;
			float tv0 = curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[0]]->v;

			float tu1 = curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[1]]->u;
			float tv1 = curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[1]]->v;

			float tu2 = curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[2]]->u;
			float tv2 = curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[2]]->v;

			XMFLOAT3 e0 = v1 - v0;
			XMFLOAT3 e1 = v2 - v0;

			float delta_u0 = tu1 - tu0;
			float delta_u1 = tu2 - tu0;

			float delta_v0 = tv1 - tv0;
			float delta_v1 = tv2 - tv0;

			float d = 1.0f / (delta_u0 * delta_v1 - delta_v0 * delta_u1);

			curMesh->m_meshface[j]->m_tangent = (e0 * delta_v1 - e1 * delta_v0) * d;
			curMesh->m_meshface[j]->m_bitangent = (e1 * delta_u0 - e0 * delta_u1) * d;

			curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[0]]->m_tangent += curMesh->m_meshface[j]->m_tangent;
			curMesh->m_meshvertex[curMesh->m_meshface[j]->m_vertexindex[0]]->m_bitangent += curMesh->m_meshface[j]->m_bitangent;
		}

		for (int j = 0; j < curMesh->m_meshvertex.size(); j++)
		{
			curMesh->m_meshvertex[j]->m_tangent.Normalize();
			curMesh->m_meshvertex[j]->m_bitangent.Normalize();
			// https://scahp.tistory.com/13
			// OutTangentArray[i].w = (Dot(Cross(t, b), n) > 0.0f) ? 1.0f : -1.0f;
			// 좌표계가 다를 때도 제대로 해주려면 필요하다. 지금은 ...
		}
	}
}

ASEParser::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

ASEParser::Node* CASEParser::GetNode(string nodeName)
{
	return m_object->object[nodeName];
}

//----------------------------------------------------------------
// 재귀 호출됨을 전제로 하는 분기 함수이다.
//
// 일단 읽고, 토큰에 따라 동작한다.
//
// 재귀를 들어가는 조건은 { 을 만날 경우이다.
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다.
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// 지역변수들 선언
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;
	// 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터.
	int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?
	// 이것은 현 토큰이 어떤 종류인가를 판별해준다.
	// 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
	LONG nowtoken;
	//----------------------------------------------------------------------

	DirectX::SimpleMath::Matrix TM1, TM2;
	DirectX::SimpleMath::Quaternion tempQT;
	DirectX::SimpleMath::Quaternion prevQT;
	DirectX::SimpleMath::Quaternion resultQT;

	// 여기가 맞는 지 모르겠다.
	// 일단 vertexnormal을 해서 vertex를 split할때 mesh facenormal의 인덱스 값이 필요하다.
	int faceNum = -1;
	int vertexIndex = 0;

	/// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
		// 넘어온 토큰에 따라 처리해준다.

		static int iv = 0;

		switch (nowtoken)
		{
			case TOKEND_BLOCK_START:

				Parsing_DivergeRecursiveALL(depth++);
				break;

			case TOKENR_HELPER_CLASS:
				break;

				//--------------------
				// 3DSMAX_ASCIIEXPORT
				//--------------------

			case TOKENR_3DSMAX_ASCIIEXPORT:
				m_data_asciiexport = Parsing_NumberLong();
				break;

				//--------------------
				// COMMENT
				//--------------------

			case TOKENR_COMMENT:
				Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐.
				//AfxMessageBox( m_TokenString, NULL, NULL);		/// 임시로 코멘트를 출력해본다
				break;

				//--------------------
				// SCENE
				//--------------------

			case TOKENR_SCENE:
				//
				break;
			case TOKENR_SCENE_FILENAME:
				m_scenedata.m_filename = Parsing_String();		// 일관성 있는 함수의 사용을 위해 String과 Int도 만들어줬다.
				break;
			case TOKENR_SCENE_FIRSTFRAME:
				m_scenedata.m_firstframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_LASTFRAME:
				m_scenedata.m_lastframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_FRAMESPEED:
				m_scenedata.m_framespeed = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_TICKSPERFRAME:
				m_scenedata.m_ticksperframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_MESHFRAMESTEP:
				m_scenedata.m_meshframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_KEYFRAMESTEP:
				m_scenedata.m_keyframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_BACKGROUND_STATIC:
				m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
				break;
			case TOKENR_SCENE_AMBIENT_STATIC:
				m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
				break;

			case TOKENR_SCENE_ENVMAP:
			{
				// 그냥 안의 내용을 읽어버린다 }가 나올때까지
				while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
				}
			}
			break;

			//--------------------
			// MATERIAL_LIST
			//--------------------


			//--------------------
			// GEOMOBJECT
			//--------------------

			case TOKENR_GROUP:
				//	한 개의 그룹 시작. 이 다음에 이름이 스트링으로 나오기는 하는데.
				break;

			case TOKENR_HELPEROBJECT:
				Create_onemesh_to_list();
				// 일단 생성하고
				// 오브젝트의 타입 정해줌. 이것에 따라 서로 다른 파싱 모드 발동.
				break;

			case TOKENR_GEOMOBJECT:
				/// 이 토큰을 만났다는건 새로운 메시가 생겼다는 것이다. 
				/// 지역 변수로 mesh를 하나 선언, 그 포인터를 리스트에 넣고, 
				/// m_onemesh에 그 포인터를 복사, 그대로 쓰면 될까?

				// y: 일단 모르겠고 Mesh를 하나 만든다.
				//m_OneMesh = new Mesh; 보니까 밑에서 만든다. Mesh일때
				Create_onemesh_to_list();
				break;

			case TOKENR_SHAPEOBJECT:
				Create_onemesh_to_list();
				break;

			case TOKENR_NODE_NAME:
				// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
				// 모드에 따라 넣어야 할 곳이 다르다.

				m_OneMesh->m_nodename = Parsing_String();

				// 한 개의 메쉬에 노드 네임이 2번 나오긴 하는데 map[]의 특성상 이미 값이 있으면 덮어씌워버리니까 실행은 된다.
				// 효율은 ...? 애초에 new인 이상 무조건 있는 지 확인해야 한다.
				if (m_object->object.find(m_OneMesh->m_nodename) == m_object->object.end())
				{
					m_object->object[m_OneMesh->m_nodename] = new Node(m_OneMesh);
					m_object->object[m_OneMesh->m_nodename]->m_name = m_OneMesh->m_nodename;
					m_addOrder.emplace_back(m_OneMesh->m_nodename);
				}

				for (int i = 0; i < m_vector_bone_list.size(); i++)
				{
					if (m_OneMesh->m_nodename == m_vector_bone_list[i]->m_bone_name)
					{
						m_object->object[m_OneMesh->m_nodename]->m_bone = m_vector_bone_list[i];
					}
				}
				break;

			case TOKENR_NODE_PARENT:
				m_OneMesh->m_nodeparent = Parsing_String();
				m_OneMesh->m_isparentexist = true;

				m_object->object[m_OneMesh->m_nodename]->m_parentNode = m_object->object[m_OneMesh->m_nodeparent];
				m_object->object[m_OneMesh->m_nodeparent]->m_childNode.push_back(m_object->object[m_OneMesh->m_nodename]);
				// 현 노드의 부모 노드의 정보.
				// 일단 입력을 하고, 나중에 정리하자.

				break;

				/// NODE_TM

			case TOKENR_NODE_TM:
				//m_parsingmode	=	eGeomobject;
				// (NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다.)
				// (Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다)

				/// 게다가,
				// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
				// 아예 이 재귀에서 리턴을 시키고 싶지만. 중간에 읽는것을 무시해야 하기 때문에...
				//if (m_onemesh->m_camera_isloadTarget) {
				//	'}'이 나올때까지 나오는건 무조건 무시! 뭐 이런거 안되나..
				// 재귀구조의 문제점이다....

				break;

			case TOKENR_INHERIT_POS:
				// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
				m_OneMesh->m_inherit_pos.x = Parsing_NumberFloat();
				m_OneMesh->m_inherit_pos.z = Parsing_NumberFloat();
				m_OneMesh->m_inherit_pos.y = Parsing_NumberFloat();
				break;
			case TOKENR_INHERIT_ROT:
				m_OneMesh->m_inherit_rot.x = Parsing_NumberFloat();
				m_OneMesh->m_inherit_rot.z = Parsing_NumberFloat();
				m_OneMesh->m_inherit_rot.y = Parsing_NumberFloat();
				break;
			case TOKENR_INHERIT_SCL:
				m_OneMesh->m_inherit_scl.x = Parsing_NumberFloat();
				m_OneMesh->m_inherit_scl.z = Parsing_NumberFloat();
				m_OneMesh->m_inherit_scl.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW0:
				m_OneMesh->m_tm_row0.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW1:
				m_OneMesh->m_tm_row1.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW2:
				m_OneMesh->m_tm_row2.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW3:
			{
				m_OneMesh->m_tm_row3.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row3.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row3.y = Parsing_NumberFloat();

				// vector3 4개를 인자로 받는 생성자가 없다.
				DirectX::SimpleMath::Vector4 row3 = m_OneMesh->m_tm_row3;

				row3.w = 1.0f;

				DirectX::SimpleMath::Matrix meshMatrix{ DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row0), DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row2), DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row1), row3 };

				m_object->object[m_OneMesh->m_nodename]->m_worldTransform = meshMatrix;
				// skinning용
				if (m_object->object[m_OneMesh->m_nodename]->m_bone != nullptr)
				{
					m_object->object[m_OneMesh->m_nodename]->m_bone->m_boneTM_NodeTM = new DirectX::SimpleMath::Matrix(meshMatrix);
					m_object->object[m_OneMesh->m_nodename]->m_bone->m_boneTM_WorldTM = new DirectX::SimpleMath::Matrix(meshMatrix);
				}

				// 로컬트랜스폼 만들기
				if (m_object->object[m_OneMesh->m_nodename]->m_parentNode != nullptr)
				{
					// 디버그 위해 쪼갬
					DirectX::SimpleMath::Matrix temp = m_object->object[m_OneMesh->m_nodeparent]->m_worldTransform.Invert();
					meshMatrix *= temp;

					//m_object->object[m_OneMesh->m_nodename]->m_localTransform = meshMatrix;
				}

				m_object->object[m_OneMesh->m_nodename]->m_localTransform = meshMatrix;
			}
			break;
			case TOKENR_TM_POS:
				m_OneMesh->m_tm_pos.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_pos.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_pos.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROTAXIS:
				m_OneMesh->m_tm_rotaxis.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_rotaxis.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_rotaxis.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROTANGLE:
			{
				m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();

				if (m_OneMesh->m_tm_rotaxis.x == 0
					&& m_OneMesh->m_tm_rotaxis.y == 0
					&& m_OneMesh->m_tm_rotaxis.z == 0)
				{

				}
				else
				{
					//Vector3 scale;
					DirectX::SimpleMath::Quaternion rotation = DirectX::XMQuaternionRotationAxis(m_OneMesh->m_tm_rotaxis, m_OneMesh->m_tm_rotangle);;
					DirectX::SimpleMath::Vector3 translation = m_OneMesh->m_tm_pos;

					//DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);
					DirectX::SimpleMath::Matrix rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
					DirectX::SimpleMath::Matrix translationMatrix = DirectX::XMMatrixTranslationFromVector(translation);

					DirectX::SimpleMath::Matrix transformMatrix = m_object->object[m_OneMesh->m_nodename]->m_worldTransform;

					transformMatrix *= translationMatrix.Invert();
					transformMatrix *= rotationMatrix.Invert();

					if (transformMatrix._11 - (-1.0f) < abs(0.1f))
					{
						m_object->object[m_OneMesh->m_nodename]->isNegativeScale = true;
						if (m_object->object[m_OneMesh->m_nodename]->m_parentNode != nullptr)
						{
							if (m_object->object[m_OneMesh->m_nodename]->m_parentNode->isNegativeScale != true)
							{
								m_object->object[m_OneMesh->m_nodename]->isNegativeScaleRoot = true;
							}
						}
					}
					else
					{
						if (m_object->object[m_OneMesh->m_nodename]->m_parentNode != nullptr)
						{
							if (m_object->object[m_OneMesh->m_nodename]->m_parentNode->isNegativeScale == true)
							{
								m_object->object[m_OneMesh->m_nodename]->isNegativeScaleNotRoot = true;
							}
						}
					}
				}
			}
			break;
			case TOKENR_TM_SCALE:
				m_OneMesh->m_tm_scale.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_scale.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_scale.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_SCALEAXIS:
				m_OneMesh->m_tm_scaleaxis.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_scaleaxis.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_scaleaxis.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_SCALEAXISANG:
				m_OneMesh->m_tm_scaleaxisang = Parsing_NumberFloat();
				// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
				break;


				/// MESH

			case TOKENR_MESH:
			{
				/// 아래의 함수에서 m_OneMesh가 생긴다.
				// 위치를 바꿔본다.
				//Create_onemesh_to_list();
			}
			break;

			case TOKENR_TIMEVALUE:
			{
				///int _timeValue = m_lexer->GetToken(m_TokenString);
				int _timeValue = Parsing_NumberInt();
				m_OneMesh->m_timevalue = _timeValue;
			}
			break;

			case TOKENR_MESH_NUMBONE:
				// 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
				// 내용 입력
				break;
			case TOKENR_MESH_NUMSKINWEIGHT:
				break;
			case TOKENR_MESH_NUMVERTEX:
				m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
				break;
			case TOKENR_MESH_NUMFACES:
				m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
				break;

				/// MESH_VERTEX_LIST

			case TOKENR_MESH_VERTEX_LIST:
				//
				// 버텍스의 값들을 집어넣어야 하는데
				// 이미 벡터로 선언이 돼 있으므로 그냥 넣으면 된다.
				break;
			case TOKENR_MESH_VERTEX:
				// 데이터 입력
			{
				Parsing_NumberInt();

				ASEParser::Vertex* tempVertex = new ASEParser::Vertex;

				// y, z를 반대로 읽어야 한다.
				tempVertex->m_pos.x = Parsing_NumberFloat();
				tempVertex->m_pos.z = Parsing_NumberFloat();
				tempVertex->m_pos.y = Parsing_NumberFloat();

				// 역행렬 곱해서 로컬 스페이스로 옮기기
				DirectX::SimpleMath::Matrix inverseMat = m_object->object[m_OneMesh->m_nodename]->m_worldTransform.Invert();
				tempVertex->m_pos = XMVector3Transform(tempVertex->m_pos, inverseMat);

				m_OneMesh->m_meshvertex.push_back(tempVertex);
			}

			break;

			/// Bone

			case TOKENR_SKIN_INITTM:
				break;
			case TOKENR_BONE_LIST:
				m_vector_bone_list = {};
				break;
			case TOKENR_BONE:
			{
				/// 모드 체인지 해 주고, Bone을 소유하고 있다는 것은 이것은 스키닝 오브젝트라는 것이다.
				// 본 하나를 만들어서 임시 포인터 보관, 벡터에 넣고
				// Bone의 넘버를 읽어 주자
				Bone* temp = new Bone;
				temp->m_bone_number = Parsing_NumberInt();

				m_vector_bone_list.emplace_back(temp);
			}
			break;
			//이 다음에 본의 이름을 넣어야 한다. 하지만 {를 한 개 더 열었으므로 임시 포인터 변수로서 보관해야겠지.
			case TOKENR_BONE_NAME:
				m_vector_bone_list[m_vector_bone_list.size() - 1]->m_bone_name = Parsing_String();
			case TOKENR_BONE_PROPERTY:
				// 이 다음 ABSOLUTE가 나오기는 하는데, 쓸 일이 없다.
				break;
				// 다음에는 TM_ROW0~3이 나오는데 역시 무시됨..

			case TOKENR_MESH_WVERTEXS:
				m_vector_wvertexs = {};
				break;

			case TOKENR_MESH_WEIGHT:
			{
				// 버텍스 하나의 정보를 만들어서 리스트에 넣음
				VertexWeight* temp = new VertexWeight;
				temp->m_wvertex_number = Parsing_NumberInt();

				m_vector_wvertexs.push_back(temp);
			}
			break;
			case TOKENR_BONE_BLENGING_WEIGHT:
			{
				// 대체 몇 단계를 들어가는거야...
				// 가중치 한개를 만들어서 리스트에 넣는다
				/// 헥 헥....
				Weight* temp = new Weight;

				temp->m_bone_number = Parsing_NumberInt();
				temp->m_bone_weight = Parsing_NumberFloat();

				m_vector_wvertexs[m_vector_wvertexs.size() - 1]->m_bone_blending_weight.push_back(temp);

				if (m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw1 == 0.0f)
				{
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw1 = temp->m_bone_weight;
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_weightIndices[0] = temp->m_bone_number;

					auto savedVertexIndexIter = m_OneMesh->m_savedVertexSplit.equal_range(m_vector_wvertexs.size() - 1);

					for (auto iter = savedVertexIndexIter.first; iter != savedVertexIndexIter.second; iter++)
					{
						m_OneMesh->m_meshvertex[iter->second]->m_bw1 = temp->m_bone_weight;
						m_OneMesh->m_meshvertex[iter->second]->m_weightIndices[0] = temp->m_bone_number;
					}
				}
				else if (m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw2 == 0.0f)
				{
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw2 = temp->m_bone_weight;
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_weightIndices[1] = temp->m_bone_number;

					auto savedVertexIndexIter = m_OneMesh->m_savedVertexSplit.equal_range(m_vector_wvertexs.size() - 1);

					for (auto iter = savedVertexIndexIter.first; iter != savedVertexIndexIter.second; iter++)
					{
						m_OneMesh->m_meshvertex[iter->second]->m_bw2 = temp->m_bone_weight;
						m_OneMesh->m_meshvertex[iter->second]->m_weightIndices[1] = temp->m_bone_number;
					}
				}
				else if (m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw3 == 0.0f)
				{
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw3 = temp->m_bone_weight;
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_weightIndices[2] = temp->m_bone_number;

					auto savedVertexIndexIter = m_OneMesh->m_savedVertexSplit.equal_range(m_vector_wvertexs.size() - 1);

					for (auto iter = savedVertexIndexIter.first; iter != savedVertexIndexIter.second; iter++)
					{
						m_OneMesh->m_meshvertex[iter->second]->m_bw3 = temp->m_bone_weight;
						m_OneMesh->m_meshvertex[iter->second]->m_weightIndices[2] = temp->m_bone_number;
					}
				}
				else
				{
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_weightIndices[3] = temp->m_bone_number;

					auto savedVertexIndexIter = m_OneMesh->m_savedVertexSplit.equal_range(m_vector_wvertexs.size() - 1);

					for (auto iter = savedVertexIndexIter.first; iter != savedVertexIndexIter.second; iter++)
					{
						m_OneMesh->m_meshvertex[iter->second]->m_weightIndices[3] = temp->m_bone_number;
					}
				}
			}
			break;


			/// MESH_FACE_LIST
			case TOKENR_MESH_FACE_LIST:
				//
				break;
			case TOKENR_MESH_FACE:
			{
				std::string a = Parsing_String(); // 면 번호
				std::string b = Parsing_String(); // A:

				ASEParser::Face* temp = new ASEParser::Face;

				// Y랑 Z를 바꿔서 읽어야 한다. 안 바꾸는 게 맞는 것 같다. 안 바꾸는 게 맞는 게 아니라 메시 오브젝트에서 복사할 때
				// 1하고 2를 바꿔서 넣어주고 있었다. 일관성을 위해 파서에서 바꿔 넣는걸로 변경 했는데
				// 여기서 바꾸면 노말, 텍스처, 인덱스 다 바꿔야 해서 그냥 저쪽에서 바꾸기로 했다... 일단은!
				temp->m_vertexindex[0] = Parsing_NumberInt();
				Parsing_String();
				temp->m_vertexindex[1] = Parsing_NumberInt();
				Parsing_String();
				temp->m_vertexindex[2] = Parsing_NumberInt();

				m_OneMesh->m_meshface.push_back(temp);

				// 가져온 토큰의 값을 확인해봐야 한다.
				// Face의 번호인데...

				// A:를 읽고
				// B:
				// C:

				/// (뒤에 정보가 더 있지만 default에 의해 스킵될 것이다.)
				/// ......

				// 벡터에 넣어준다.
			}
			break;


			case TOKENR_MESH_NUMTVERTEX:
				break;

				/// MESH_TVERTLIST

			case TOKENR_MESH_TVERTLIST:
				//
				break;
			case TOKENR_MESH_TVERT:
			{
				// 버텍스의 인덱스가 나오는데 어차피 순서와 같으므로 버린다.
				// 새로운 TVertex를 만들어서 벡터에 넣는다
				COneTVertex* tVertex = new COneTVertex;

				Parsing_NumberInt();

				// 이미 리버스 된 텍스처를 쓰고 있기 때문에 1 - 를 하지 않는다.
				tVertex->m_u = Parsing_NumberFloat();
				tVertex->m_v = 1 - Parsing_NumberFloat();

				m_OneMesh->m_mesh_tvertex.push_back(tVertex);
			}
			break;

			case TOKENR_MESH_TFACE:
			{
				int tFaceNum = Parsing_NumberInt();

				m_OneMesh->m_meshface[tFaceNum]->m_TFace[0] = Parsing_NumberInt();
				m_OneMesh->m_meshface[tFaceNum]->m_TFace[1] = Parsing_NumberInt();
				m_OneMesh->m_meshface[tFaceNum]->m_TFace[2] = Parsing_NumberInt();

				// 텍스처 버텍스 스플릿
				for (int i = 0; i < 3; i++)
				{
					// 이미 텍스처 uv값이 버텍스에 설정 되었는데 이번에 넣을 값하고 다르다면
					// or 에는 괄호를 쳐야 하는 것 같다. 연산자 우선순위와 관련이 있지 않을까
					if (m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->m_istextureset == true
						&& (m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->u != m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_u
							|| m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->v != m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_v))
					{
						ASEParser::Vertex* temp = new ASEParser::Vertex;

						temp->m_pos = m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->m_pos;

						temp->u = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_u;
						temp->v = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_v;

						m_OneMesh->m_meshvertex.push_back(temp);

						// 노말 나눌 때도 나눈 거에 노말 적용 해야된다.
						m_OneMesh->m_savedVertexSplit.insert(std::make_pair(m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i], m_OneMesh->m_meshvertex.size() - 1));

						m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i] = m_OneMesh->m_meshvertex.size() - 1;
					}
					else
					{
						m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->u = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_u;
						m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->v = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_v;
					}

					m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->m_istextureset = true;
				}
			}
			break;

			case TOKENR_MESH_NUMTVFACES:
				break;

			case TOKENR_MESH_NORMALS:
				break;

			case TOKENR_MESH_FACENORMAL:
			{
				faceNum = Parsing_NumberInt();

				m_OneMesh->m_meshface[faceNum]->m_normal.x = Parsing_NumberFloat();
				m_OneMesh->m_meshface[faceNum]->m_normal.z = Parsing_NumberFloat();
				m_OneMesh->m_meshface[faceNum]->m_normal.y = Parsing_NumberFloat();
			}
			break;

			case TOKENR_MESH_VERTEXNORMAL:
			{
				//원래는 mesh_vertexnormal 값을 읽어왔는데 텍스처에서 버텍스를 스플릿 하느라 내용이 바뀌었다.
				/ *int vertexNum = * /Parsing_NumberInt();

				DirectX::SimpleMath::Vector3 normal;
				normal.x = Parsing_NumberFloat();
				normal.z = Parsing_NumberFloat();
				normal.y = Parsing_NumberFloat();

				int vertexNum = m_OneMesh->m_meshface[faceNum]->m_vertexindex[vertexIndex];

				// 한 번 노말이 셋팅 되었었는데, 이전 값과 지금 값이 다르다면 버텍스 추가해야됨
				if (m_OneMesh->m_meshvertex[vertexNum]->m_isnormalset == true
					&& m_OneMesh->m_meshvertex[vertexNum]->m_normal != normal)
				{
					ASEParser::Vertex* temp = new ASEParser::Vertex;

					temp->m_pos = m_OneMesh->m_meshvertex[vertexNum]->m_pos;

					temp->u = m_OneMesh->m_meshvertex[vertexNum]->u;
					temp->v = m_OneMesh->m_meshvertex[vertexNum]->v;

					temp->m_normal = normal;

					m_OneMesh->m_meshvertex.push_back(temp);

					// 면에서 새로 추가한 버텍스의 기존 인덱스 값을 새로 추가한 인덱스 값으로 변경
					for (int i = 0; i < 3; i++)
					{
						if (m_OneMesh->m_meshface[faceNum]->m_vertexindex[i] == vertexNum)
						{
							m_OneMesh->m_savedVertexSplit.insert(std::make_pair(m_OneMesh->m_meshface[faceNum]->m_vertexindex[i], m_OneMesh->m_meshvertex.size() - 1));

							m_OneMesh->m_meshface[faceNum]->m_vertexindex[i] = m_OneMesh->m_meshvertex.size() - 1;
							m_OneMesh->m_meshvertex[m_OneMesh->m_meshvertex.size() - 1]->m_isnormalset = true;
						}
					}
				}
				else
				{
					m_OneMesh->m_meshvertex[vertexNum]->m_normal = normal;
					m_OneMesh->m_meshvertex[vertexNum]->m_isnormalset = true;

					// 텍스처 버텍스 스플릿 했던 것에 추가
					auto savedVertexIndexIter = m_OneMesh->m_savedVertexSplit.equal_range(vertexNum);

					for (auto iter = savedVertexIndexIter.first; iter != savedVertexIndexIter.second; iter++)
					{
						m_OneMesh->m_meshvertex[iter->second]->m_normal = normal;
					}
				}

				vertexIndex++;
				vertexIndex %= 3;
			}

			break;

			case TOKENR_TM_ANIMATION:
			{
				std::string nodeName = m_OneMesh->m_nodename;

				Animation* thisAnimation = new Animation;
				m_animations.push_back(thisAnimation);

				m_object->object[nodeName]->m_animation = thisAnimation;
				m_object->object[nodeName]->m_animation->m_nodename = nodeName;

				thisAnimation->m_firstFrame = m_scenedata.m_firstframe;
				thisAnimation->m_lastFrame = m_scenedata.m_lastframe;
				thisAnimation->m_ticksperFrame = m_scenedata.m_ticksperframe;
			}
			break;
			case TOKENR_CONTROL_POS_TRACK:
				break;
			case TOKENR_CONTROL_ROT_TRACK:
				break;
			case TOKENR_CONTROL_POS_SAMPLE:
			{
				CAnimation_pos* aniPos = new CAnimation_pos;

				aniPos->m_time = Parsing_NumberLong();
				aniPos->m_pos = Parsing_NumberVector3();

				if (m_object->object[m_OneMesh->m_nodename]->m_animation->m_position.size() == 0)
				{
					if (aniPos->m_time != 0)
					{
						CAnimation_pos* tempAniPos = new CAnimation_pos;
						tempAniPos->m_time = 0;
						DirectX::SimpleMath::Matrix tempLocalMat = m_object->object[m_OneMesh->m_nodename]->m_localTransform;

						DirectX::SimpleMath::Vector3 scale;
						DirectX::SimpleMath::Quaternion rotation;
						DirectX::SimpleMath::Vector3 translation;

						tempLocalMat.Decompose(scale, rotation, translation);

						tempAniPos->m_pos = translation;
						m_object->object[m_OneMesh->m_nodename]->m_animation->m_position.emplace_back(tempAniPos);
					}
				}

				m_object->object[m_OneMesh->m_nodename]->m_animation->m_position.emplace_back(aniPos);
			}
			break;
			case TOKENR_CONTROL_ROT_SAMPLE:
			{
				CAnimation_rot* aniRot = new CAnimation_rot;

				aniRot->m_time = Parsing_NumberLong();

				float xAxis = Parsing_NumberFloat();
				float zAxis = Parsing_NumberFloat();
				float yAxis = Parsing_NumberFloat();
				float angle = Parsing_NumberFloat();

				//float rotationAngleInRadians = DirectX::XMConvertToRadians(angle);

				XMVECTOR axis = { xAxis, yAxis, zAxis }; // 회전 축
				DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);

				// 첫 프레임
				DirectX::SimpleMath::Quaternion rotation;
				if (m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.size() == 0)
				{
					rotation = rotationQuaternion;
					aniRot->m_rotQT_accumulation = rotation;

					if (aniRot->m_time != 0)
					{
						CAnimation_rot* firstRot = new CAnimation_rot;
						firstRot->m_time = 0;

						XMVECTOR tempAxis = m_OneMesh->m_tm_rotaxis; // 회전 축
						if (m_OneMesh->m_tm_rotaxis.x == 0
							&& m_OneMesh->m_tm_rotaxis.y == 0
							&& m_OneMesh->m_tm_rotaxis.z == 0)
						{
							DirectX::SimpleMath::Quaternion rotationQuaternion = {};
						}
						else
						{
							DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationAxis(tempAxis, m_OneMesh->m_tm_rotangle);
						}
						firstRot->m_rotQT_accumulation = rotationQuaternion;

						m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.emplace_back(firstRot);
					}

					m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.emplace_back(aniRot);
				}
				else
				{
					rotation = m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation[m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.size() - 1]->m_rotQT_accumulation;
					// 이전 거가 앞
					rotation = DirectX::XMQuaternionMultiply(rotation, rotationQuaternion);

					aniRot->m_rotQT_accumulation = rotation;
					m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.emplace_back(aniRot);
				}
			}
			break;

			case TOKEND_END:
				// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

				//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
				TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
				return;

				break;

				/// 위의 아무것도 해당하지 않을때
			default:
				// 아무것도 하지 않는다.
				break;

		}	// switch()

		///-----------------------------------------------
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			// 루프를 1000000번이상이나 돌 이유가 없다. (데이터가 100000개가 아닌이상)
			// 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		/// 안전 코드.
		///-----------------------------------------------

	}		// while()

	// 여기까지 왔다면 while()을 벗어났다는 것이고 그 말은
	// 괄호를 닫았다는 것이므로
	// 리턴하자 (재귀함수)

	return;
}


///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3개의 Float를 벡터 하나로
DirectX::SimpleMath::Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG				token;
	DirectX::SimpleMath::Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
}

///--------------------------------------------------
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// 클래스간 값 복사
	m_MeshList.push_back(m_OneMesh);
}

// 메트리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

/ *
// 애니메이션데이터 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}* /

// 정점 하나를..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}*/

#include "CASEParser.h"


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
	Init();
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다.
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다.
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련. 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자.
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	/// 1) 다롱이아부지김교수의 파워풀한 재귀함수 한개로 처리 끝!
	m_object = new Object;
	Parsing_DivergeRecursiveALL(0);

	//LerpAnimation();
	// 	// 버텍스에 중복이 있다. 지금 구조에서는 당연한 것.
	// 	for (int i = 0; i < m_OneMesh->m_meshvertex.size(); i++)
	// 	{
	// 		Vector3 pos = m_OneMesh->m_meshvertex[i]->m_pos;
	// 		Vector3 normal = m_OneMesh->m_meshvertex[i]->m_normal;
	// 		for (int j = 0; j < m_OneMesh->m_meshvertex.size(); j++)
	// 		{
	// 			if (i == j)
	// 			{
	// 				continue;;
	// 			}
	// 
	// 			if (pos == m_OneMesh->m_meshvertex[j]->m_pos && normal == m_OneMesh->m_meshvertex[j]->m_normal)
	// 			{
	// 				int a = 3;
	// 			}
	// 		}
	// 	}

	return TRUE;
}

/// 1차 변환

// CScenedata 복사
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// 값 복사
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// 구현 제거 (예시용)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// 버텍스들은 일단 모두 복사
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;
		_pVertex->m_normal = pMesh->m_meshvertex[i]->m_normal;
		_pVertex->u = pMesh->m_meshvertex[i]->u;
		_pVertex->v = pMesh->m_meshvertex[i]->v;
		_pVertex->m_bw1 = pMesh->m_meshvertex[i]->m_bw1;
		_pVertex->m_bw2 = pMesh->m_meshvertex[i]->m_bw2;
		_pVertex->m_bw3 = pMesh->m_meshvertex[i]->m_bw3;

		_pVertex->m_weightIndices[0] = pMesh->m_meshvertex[i]->m_weightIndices[0];
		_pVertex->m_weightIndices[1] = pMesh->m_meshvertex[i]->m_weightIndices[1];
		_pVertex->m_weightIndices[2] = pMesh->m_meshvertex[i]->m_weightIndices[2];
		_pVertex->m_weightIndices[3] = pMesh->m_meshvertex[i]->m_weightIndices[3];

		pMesh->m_opt_vertex.push_back(_pVertex);
	}

	//TODO: 왜 여기를 이렇게 해 두었을까?
// 	// 나머지는 face를 기준으로 한 인덱스로 찾아야 함
// 	unsigned int _faceCount = pMesh->m_meshface.size();
// 	TRACE("FaceCount : %d", _faceCount);
// 
// 	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
// 	{
// 		TRACE("FaceNum : %d / %d\n", i, _faceCount);
// 
// 		for (int j = 0; j < 3; j++)
// 		{
// 			Vertex* _nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];
// 
// 			// face기준으로 버텍스의 노말을 넣어준다.
// 			// (예시용)
// 			_nowVertex->m_normal.x = 0;
// 			_nowVertex->m_normal.y = 0;
// 			_nowVertex->m_normal.z = 0;
// 		}
// 	}

	// 인덱스는 그냥 복사
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	return FALSE;
}

void CASEParser::LerpAnimation()
{
	for (int i = 0; i < m_animations.size(); i++)
	{
		std::vector<CAnimation_pos*> newPosAnimation = {};
		if (m_animations[i]->m_position.size() != 0)
		{
			for (int k = 0; k < m_scenedata.m_lastframe; k++)
			{
				newPosAnimation.push_back(new CAnimation_pos);
				newPosAnimation[k]->m_time = k * m_scenedata.m_ticksperframe;
			}

			unsigned int curIndex = 0;
			for (int j = 0; j < m_animations[i]->m_position.size(); j++)
			{
				int firstTick;
				int secondTick;

				DirectX::SimpleMath::Vector3 firstPos;
				DirectX::SimpleMath::Vector3 secondPos;

				if (j == m_animations[i]->m_position.size() - 1)
				{
					firstTick = m_animations[i]->m_position[j]->m_time;
					secondTick = m_scenedata.m_lastframe * m_scenedata.m_ticksperframe;

					firstPos = m_animations[i]->m_position[j]->m_pos;
					secondPos = m_animations[i]->m_position[0]->m_pos;
				}
				else
				{
					firstTick = m_animations[i]->m_position[j]->m_time;
					secondTick = m_animations[i]->m_position[j + 1]->m_time;

					firstPos = m_animations[i]->m_position[j]->m_pos;
					secondPos = m_animations[i]->m_position[j + 1]->m_pos;
				}

				float t;

				while (curIndex * m_scenedata.m_ticksperframe < secondTick)
				{
					if (firstTick == curIndex * m_scenedata.m_ticksperframe) t = 0;
					else
					{
						t = (float)(curIndex * m_scenedata.m_ticksperframe - firstTick) / (float)(secondTick - firstTick);
					}

					newPosAnimation[curIndex]->m_pos = newPosAnimation[curIndex]->m_pos.Lerp(firstPos, secondPos, t);
					curIndex++;
				}
			}
			m_animations[i]->m_position = newPosAnimation;
		}

		std::vector<CAnimation_rot*> newRotAnimation = {};
		if (m_animations[i]->m_rotation.size() != 0)
		{
			for (int k = 0; k < m_scenedata.m_lastframe; k++)
			{
				newRotAnimation.push_back(new CAnimation_rot);
				newRotAnimation[k]->m_time = k * m_scenedata.m_ticksperframe;
			}

			unsigned int curIndex = 0;
			for (int j = 0; j < m_animations[i]->m_rotation.size(); j++)
			{
				int firstTick;
				int secondTick;

				DirectX::SimpleMath::Quaternion firstQuat;
				DirectX::SimpleMath::Quaternion secondQuat;

				if (j == m_animations[i]->m_rotation.size() - 1)
				{
					firstTick = m_animations[i]->m_rotation[j]->m_time;
					secondTick = m_scenedata.m_lastframe * m_scenedata.m_ticksperframe;

					firstQuat = m_animations[i]->m_rotation[j]->m_rotQT_accumulation;
					secondQuat = m_animations[i]->m_rotation[0]->m_rotQT_accumulation;
				}
				else
				{
					firstTick = m_animations[i]->m_rotation[j]->m_time;
					secondTick = m_animations[i]->m_rotation[j + 1]->m_time;

					firstQuat = m_animations[i]->m_rotation[j]->m_rotQT_accumulation;
					secondQuat = m_animations[i]->m_rotation[j + 1]->m_rotQT_accumulation;
				}

				float t;

				while (curIndex * m_scenedata.m_ticksperframe < secondTick)
				{
					if (firstTick == curIndex * m_scenedata.m_ticksperframe) t = 0;
					else
					{
						t = (float)(curIndex * m_scenedata.m_ticksperframe - firstTick) / (float)(secondTick - firstTick);
					}

					newRotAnimation[curIndex]->m_rotQT_accumulation = newRotAnimation[curIndex]->m_rotQT_accumulation.Slerp(firstQuat, secondQuat, t);
					curIndex++;
				}
			}
			m_animations[i]->m_rotation = newRotAnimation;
		}
	}
}

ASEParser::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

ASEParser::Node* CASEParser::GetNode(string nodeName)
{
	return m_object->object[nodeName];
}

//----------------------------------------------------------------
// 재귀 호출됨을 전제로 하는 분기 함수이다.
//
// 일단 읽고, 토큰에 따라 동작한다.
//
// 재귀를 들어가는 조건은 { 을 만날 경우이다.
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다.
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// 지역변수들 선언
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;
	// 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터.
	int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?
	// 이것은 현 토큰이 어떤 종류인가를 판별해준다.
	// 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
	LONG nowtoken;
	//----------------------------------------------------------------------

	DirectX::SimpleMath::Matrix TM1, TM2;
	DirectX::SimpleMath::Quaternion tempQT;
	DirectX::SimpleMath::Quaternion prevQT;
	DirectX::SimpleMath::Quaternion resultQT;

	// 여기가 맞는 지 모르겠다.
	// 일단 vertexnormal을 해서 vertex를 split할때 mesh facenormal의 인덱스 값이 필요하다.
	int faceNum = -1;
	int vertexIndex = 0;

	/// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
		// 넘어온 토큰에 따라 처리해준다.

		static int iv = 0;

		switch (nowtoken)
		{
			case TOKEND_BLOCK_START:

				Parsing_DivergeRecursiveALL(depth++);
				break;

			case TOKENR_HELPER_CLASS:
				break;

				//--------------------
				// 3DSMAX_ASCIIEXPORT
				//--------------------

			case TOKENR_3DSMAX_ASCIIEXPORT:
				m_data_asciiexport = Parsing_NumberLong();
				break;

				//--------------------
				// COMMENT
				//--------------------

			case TOKENR_COMMENT:
				Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐.
				//AfxMessageBox( m_TokenString, NULL, NULL);		/// 임시로 코멘트를 출력해본다
				break;

				//--------------------
				// SCENE
				//--------------------

			case TOKENR_SCENE:
				//
				break;
			case TOKENR_SCENE_FILENAME:
				m_scenedata.m_filename = Parsing_String();		// 일관성 있는 함수의 사용을 위해 String과 Int도 만들어줬다.
				break;
			case TOKENR_SCENE_FIRSTFRAME:
				m_scenedata.m_firstframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_LASTFRAME:
				m_scenedata.m_lastframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_FRAMESPEED:
				m_scenedata.m_framespeed = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_TICKSPERFRAME:
				m_scenedata.m_ticksperframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_MESHFRAMESTEP:
				m_scenedata.m_meshframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_KEYFRAMESTEP:
				m_scenedata.m_keyframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_BACKGROUND_STATIC:
				m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
				break;
			case TOKENR_SCENE_AMBIENT_STATIC:
				m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
				break;

			case TOKENR_SCENE_ENVMAP:
			{
				// 그냥 안의 내용을 읽어버린다 }가 나올때까지
				while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
				}
			}
			break;

			//--------------------
			// MATERIAL_LIST
			//--------------------


			//--------------------
			// GEOMOBJECT
			//--------------------

			case TOKENR_GROUP:
				//	한 개의 그룹 시작. 이 다음에 이름이 스트링으로 나오기는 하는데.
				break;

			case TOKENR_HELPEROBJECT:
				Create_onemesh_to_list();
				// 일단 생성하고
				// 오브젝트의 타입 정해줌. 이것에 따라 서로 다른 파싱 모드 발동.
				break;

			case TOKENR_GEOMOBJECT:
				/// 이 토큰을 만났다는건 새로운 메시가 생겼다는 것이다. 
				/// 지역 변수로 mesh를 하나 선언, 그 포인터를 리스트에 넣고, 
				/// m_onemesh에 그 포인터를 복사, 그대로 쓰면 될까?

				// y: 일단 모르겠고 Mesh를 하나 만든다.
				//m_OneMesh = new Mesh; 보니까 밑에서 만든다. Mesh일때
				Create_onemesh_to_list();
				break;

			case TOKENR_SHAPEOBJECT:
				Create_onemesh_to_list();
				break;

			case TOKENR_NODE_NAME:
				// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
				// 모드에 따라 넣어야 할 곳이 다르다.

				m_OneMesh->m_nodename = Parsing_String();

				// 한 개의 메쉬에 노드 네임이 2번 나오긴 하는데 map[]의 특성상 이미 값이 있으면 덮어씌워버리니까 실행은 된다.
				// 효율은 ...? 애초에 new인 이상 무조건 있는 지 확인해야 한다.
				if (m_object->object.find(m_OneMesh->m_nodename) == m_object->object.end())
				{
					m_object->object[m_OneMesh->m_nodename] = new Node(m_OneMesh);
					m_object->object[m_OneMesh->m_nodename]->m_name = m_OneMesh->m_nodename;
					m_addOrder.emplace_back(m_OneMesh->m_nodename);
				}
				for (int i = 0; i < m_vector_bone_list.size(); i++)
				{
					if (m_OneMesh->m_nodename == m_vector_bone_list[i]->m_bone_name)
					{
						m_object->object[m_OneMesh->m_nodename]->m_bone = m_vector_bone_list[i];
					}
				}
				break;

			case TOKENR_NODE_PARENT:
				m_OneMesh->m_nodeparent = Parsing_String();
				m_OneMesh->m_isparentexist = true;

				m_object->object[m_OneMesh->m_nodename]->m_parentNode = m_object->object[m_OneMesh->m_nodeparent];
				m_object->object[m_OneMesh->m_nodeparent]->m_childNode.push_back(m_object->object[m_OneMesh->m_nodename]);

				//m_object->object[m_OneMesh->m_nodename]->m_parentName = m_OneMesh->m_nodeparent;
				// 현 노드의 부모 노드의 정보.
				// 일단 입력을 하고, 나중에 정리하자.

				break;

				/// NODE_TM

			case TOKENR_NODE_TM:
				//m_parsingmode	=	eGeomobject;
				// (NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다.)
				// (Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다)

				/// 게다가,
				// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
				// 아예 이 재귀에서 리턴을 시키고 싶지만. 중간에 읽는것을 무시해야 하기 때문에...
				//if (m_onemesh->m_camera_isloadTarget) {
				//	'}'이 나올때까지 나오는건 무조건 무시! 뭐 이런거 안되나..
				// 재귀구조의 문제점이다....

				break;

			case TOKENR_INHERIT_POS:
				// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
				m_OneMesh->m_inherit_pos.x = Parsing_NumberFloat();
				m_OneMesh->m_inherit_pos.z = Parsing_NumberFloat();
				m_OneMesh->m_inherit_pos.y = Parsing_NumberFloat();
				break;
			case TOKENR_INHERIT_ROT:
				m_OneMesh->m_inherit_rot.x = Parsing_NumberFloat();
				m_OneMesh->m_inherit_rot.z = Parsing_NumberFloat();
				m_OneMesh->m_inherit_rot.y = Parsing_NumberFloat();
				break;
			case TOKENR_INHERIT_SCL:
				m_OneMesh->m_inherit_scl.x = Parsing_NumberFloat();
				m_OneMesh->m_inherit_scl.z = Parsing_NumberFloat();
				m_OneMesh->m_inherit_scl.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW0:
				m_OneMesh->m_tm_row0.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW1:
				m_OneMesh->m_tm_row1.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW2:
				m_OneMesh->m_tm_row2.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROW3:
			{
				m_OneMesh->m_tm_row3.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row3.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row3.y = Parsing_NumberFloat();

				// vector3 4개를 인자로 받는 생성자가 없다.
				DirectX::SimpleMath::Vector4 row3 = m_OneMesh->m_tm_row3;

				row3.w = 1.0f;

				DirectX::SimpleMath::Matrix meshMatrix{ DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row0), DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row2), DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row1), row3 };

				m_object->object[m_OneMesh->m_nodename]->m_worldTransform = meshMatrix;

				// skinning용
				if (m_object->object[m_OneMesh->m_nodename]->m_bone != nullptr)
				{
					m_object->object[m_OneMesh->m_nodename]->m_bone->m_boneTM_NodeTM = new DirectX::SimpleMath::Matrix(meshMatrix);
					m_object->object[m_OneMesh->m_nodename]->m_bone->m_boneTM_WorldTM = new DirectX::SimpleMath::Matrix(meshMatrix);
				}

				// 로컬트랜스폼 만들기
				if (m_object->object[m_OneMesh->m_nodename]->m_parentNode != nullptr)
				{
					// 디버그 위해 쪼갬
					DirectX::SimpleMath::Matrix temp = m_object->object[m_OneMesh->m_nodeparent]->m_worldTransform.Invert();
					meshMatrix *= temp;

					//m_object->object[m_OneMesh->m_nodename]->m_localTransform = meshMatrix;
				}

				m_object->object[m_OneMesh->m_nodename]->m_localTransform = meshMatrix;
			}

			break;
			case TOKENR_TM_POS:
				m_OneMesh->m_tm_pos.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_pos.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_pos.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROTAXIS:
				m_OneMesh->m_tm_rotaxis.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_rotaxis.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_rotaxis.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_ROTANGLE:
			{
				m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();

				if (m_OneMesh->m_tm_rotaxis.x == 0
					&& m_OneMesh->m_tm_rotaxis.y == 0
					&& m_OneMesh->m_tm_rotaxis.z == 0)
				{

				}
				else
				{
					//Vector3 scale;
					DirectX::SimpleMath::Quaternion rotation = DirectX::XMQuaternionRotationAxis(m_OneMesh->m_tm_rotaxis, m_OneMesh->m_tm_rotangle);;
					DirectX::SimpleMath::Vector3 translation = m_OneMesh->m_tm_pos;

					//DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);
					DirectX::SimpleMath::Matrix rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
					DirectX::SimpleMath::Matrix translationMatrix = DirectX::XMMatrixTranslationFromVector(translation);

					DirectX::SimpleMath::Matrix transformMatrix = m_object->object[m_OneMesh->m_nodename]->m_worldTransform;

					transformMatrix *= translationMatrix.Invert();
					transformMatrix *= rotationMatrix.Invert();

					if (transformMatrix._11 - (-1.0f) < abs(0.1f))
					{
						m_object->object[m_OneMesh->m_nodename]->isNegativeScale = true;
						if (m_object->object[m_OneMesh->m_nodename]->m_parentNode != nullptr)
						{
							if (m_object->object[m_OneMesh->m_nodename]->m_parentNode->isNegativeScale != true)
							{
								m_object->object[m_OneMesh->m_nodename]->isNegativeScaleRoot = true;
							}
						}
					}
					else
					{
						if (m_object->object[m_OneMesh->m_nodename]->m_parentNode != nullptr)
						{
							if (m_object->object[m_OneMesh->m_nodename]->m_parentNode->isNegativeScale == true)
							{
								m_object->object[m_OneMesh->m_nodename]->isNegativeScaleNotRoot = true;
							}
						}
					}
				}
			}
			break;
			case TOKENR_TM_SCALE:
				m_OneMesh->m_tm_scale.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_scale.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_scale.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_SCALEAXIS:
				m_OneMesh->m_tm_scaleaxis.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_scaleaxis.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_scaleaxis.y = Parsing_NumberFloat();
				break;
			case TOKENR_TM_SCALEAXISANG:
				m_OneMesh->m_tm_scaleaxisang = Parsing_NumberFloat();
				// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
				break;


				/// MESH

			case TOKENR_MESH:
			{
				/// 아래의 함수에서 m_OneMesh가 생긴다.
				// 위치를 바꿔본다.
				//Create_onemesh_to_list();
			}
			break;

			case TOKENR_TIMEVALUE:
			{
				///int _timeValue = m_lexer->GetToken(m_TokenString);
				int _timeValue = Parsing_NumberInt();
				m_OneMesh->m_timevalue = _timeValue;
			}
			break;

			case TOKENR_MESH_NUMBONE:
				// 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
				// 내용 입력
				break;
			case TOKENR_MESH_NUMSKINWEIGHT:
				break;
			case TOKENR_MESH_NUMVERTEX:
				m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
				break;
			case TOKENR_MESH_NUMFACES:
				m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
				break;

				/// MESH_VERTEX_LIST

			case TOKENR_MESH_VERTEX_LIST:
				//
				// 버텍스의 값들을 집어넣어야 하는데
				// 이미 벡터로 선언이 돼 있으므로 그냥 넣으면 된다.
				break;
			case TOKENR_MESH_VERTEX:
				// 데이터 입력
			{
				Parsing_NumberInt();

				ASEParser::Vertex* tempVertex = new ASEParser::Vertex;

				// y, z를 반대로 읽어야 한다.
				tempVertex->m_pos.x = Parsing_NumberFloat();
				tempVertex->m_pos.z = Parsing_NumberFloat();
				tempVertex->m_pos.y = Parsing_NumberFloat();

				// 역행렬 곱해서 로컬 스페이스로 옮기기
				DirectX::SimpleMath::Matrix inverseMat = m_object->object[m_OneMesh->m_nodename]->m_worldTransform.Invert();
				tempVertex->m_pos = XMVector3Transform(tempVertex->m_pos, inverseMat);

				m_OneMesh->m_meshvertex.push_back(tempVertex);
			}

			break;

			/// Bone

			case TOKENR_SKIN_INITTM:
				break;
			case TOKENR_BONE_LIST:
				m_vector_bone_list = {};
				break;
			case TOKENR_BONE:
			{
				/// 모드 체인지 해 주고, Bone을 소유하고 있다는 것은 이것은 스키닝 오브젝트라는 것이다.
				// 본 하나를 만들어서 임시 포인터 보관, 벡터에 넣고
				// Bone의 넘버를 읽어 주자
				Bone* temp = new Bone;
				temp->m_bone_number = Parsing_NumberInt();

				m_vector_bone_list.emplace_back(temp);
			}
			break;
			//이 다음에 본의 이름을 넣어야 한다. 하지만 {를 한 개 더 열었으므로 임시 포인터 변수로서 보관해야겠지.
			case TOKENR_BONE_NAME:
				m_vector_bone_list[m_vector_bone_list.size() - 1]->m_bone_name = Parsing_String();
			case TOKENR_BONE_PROPERTY:
				// 이 다음 ABSOLUTE가 나오기는 하는데, 쓸 일이 없다.
				break;
				// 다음에는 TM_ROW0~3이 나오는데 역시 무시됨..

			case TOKENR_MESH_WVERTEXS:
				m_vector_wvertexs = {};
				break;

			case TOKENR_MESH_WEIGHT:
			{
				// 버텍스 하나의 정보를 만들어서 리스트에 넣음
				VertexWeight* temp = new VertexWeight;
				temp->m_wvertex_number = Parsing_NumberInt();

				m_vector_wvertexs.push_back(temp);
			}
			break;
			case TOKENR_BONE_BLENGING_WEIGHT:
			{
				// 대체 몇 단계를 들어가는거야...
				// 가중치 한개를 만들어서 리스트에 넣는다
				/// 헥 헥....
				Weight* temp = new Weight;

				temp->m_bone_number = Parsing_NumberInt();
				temp->m_bone_weight = Parsing_NumberFloat();

				m_vector_wvertexs[m_vector_wvertexs.size() - 1]->m_bone_blending_weight.push_back(temp);

				if (m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw1 == 0.0f)
				{
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw1 = temp->m_bone_weight;
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_weightIndices[0] = temp->m_bone_number;
				}
				else if (m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw2 == 0.0f)
				{
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw2 = temp->m_bone_weight;
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_weightIndices[1] = temp->m_bone_number;
				}
				else if (m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw3 == 0.0f)
				{
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_bw3 = temp->m_bone_weight;
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_weightIndices[2] = temp->m_bone_number;
				}
				else
				{
					m_OneMesh->m_meshvertex[m_vector_wvertexs.size() - 1]->m_weightIndices[3] = temp->m_bone_number;
				}

				m_vector_weight.push_back(temp);
			}
			break;


			/// MESH_FACE_LIST
			case TOKENR_MESH_FACE_LIST:
				//
				break;
			case TOKENR_MESH_FACE:
			{
				std::string a = Parsing_String(); // 면 번호
				std::string b = Parsing_String(); // A:

				ASEParser::Face* temp = new ASEParser::Face;

				// Y랑 Z를 바꿔서 읽어야 한다. 안 바꾸는 게 맞는 것 같다. 안 바꾸는 게 맞는 게 아니라 메시 오브젝트에서 복사할 때
				// 1하고 2를 바꿔서 넣어주고 있었다. 일관성을 위해 파서에서 바꿔 넣는걸로 변경 했는데
				// 여기서 바꾸면 노말, 텍스처, 인덱스 다 바꿔야 해서 그냥 저쪽에서 바꾸기로 했다... 일단은!
				temp->m_vertexindex[0] = Parsing_NumberInt();
				Parsing_String();
				temp->m_vertexindex[1] = Parsing_NumberInt();
				Parsing_String();
				temp->m_vertexindex[2] = Parsing_NumberInt();

				m_OneMesh->m_meshface.push_back(temp);

				// 가져온 토큰의 값을 확인해봐야 한다.
				// Face의 번호인데...

				// A:를 읽고
				// B:
				// C:

				/// (뒤에 정보가 더 있지만 default에 의해 스킵될 것이다.)
				/// ......

				// 벡터에 넣어준다.
			}
			break;


			case TOKENR_MESH_NUMTVERTEX:
				break;

				/// MESH_TVERTLIST

			case TOKENR_MESH_TVERTLIST:
				//
				break;
			case TOKENR_MESH_TVERT:
			{
				// 버텍스의 인덱스가 나오는데 어차피 순서와 같으므로 버린다.
				// 새로운 TVertex를 만들어서 벡터에 넣는다
				COneTVertex* tVertex = new COneTVertex;

				Parsing_NumberInt();

				// 이미 리버스 된 텍스처를 쓰고 있기 때문에 1 - 를 하지 않는다.
				tVertex->m_u = Parsing_NumberFloat();
				tVertex->m_v = 1 - Parsing_NumberFloat();

				m_OneMesh->m_mesh_tvertex.push_back(tVertex);
			}
			break;

			case TOKENR_MESH_TFACE:
			{
				int tFaceNum = Parsing_NumberInt();

				m_OneMesh->m_meshface[tFaceNum]->m_TFace[0] = Parsing_NumberInt();
				m_OneMesh->m_meshface[tFaceNum]->m_TFace[1] = Parsing_NumberInt();
				m_OneMesh->m_meshface[tFaceNum]->m_TFace[2] = Parsing_NumberInt();

				// 텍스처 버텍스 스플릿
				for (int i = 0; i < 3; i++)
				{
					// 이미 텍스처 uv값이 버텍스에 설정 되었는데 이번에 넣을 값하고 다르다면
					// or 에는 괄호를 쳐야 하는 것 같다. 연산자 우선순위와 관련이 있지 않을까
					if (m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->m_istextureset == true
						&& (m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->u != m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_u
							|| m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->v != m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_v))
					{
						ASEParser::Vertex* temp = new ASEParser::Vertex;

						temp->m_pos = m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->m_pos;

						temp->u = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_u;
						temp->v = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_v;

						m_OneMesh->m_meshvertex.push_back(temp);

						m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i] = m_OneMesh->m_meshvertex.size() - 1;
					}
					else
					{
						m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->u = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_u;
						m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->v = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_v;
					}

					m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->m_istextureset = true;
				}
			}
			break;

			case TOKENR_MESH_NUMTVFACES:
				break;

			case TOKENR_MESH_NORMALS:
				break;

			case TOKENR_MESH_FACENORMAL:
			{
				faceNum = Parsing_NumberInt();

				m_OneMesh->m_meshface[faceNum]->m_normal.x = Parsing_NumberFloat();
				m_OneMesh->m_meshface[faceNum]->m_normal.z = Parsing_NumberFloat();
				m_OneMesh->m_meshface[faceNum]->m_normal.y = Parsing_NumberFloat();
			}
			break;

			case TOKENR_MESH_VERTEXNORMAL:
			{
				//원래는 mesh_vertexnormal 값을 읽어왔는데 텍스처에서 버텍스를 스플릿 하느라 내용이 바뀌었다.
				/*int vertexNum = */Parsing_NumberInt();

				DirectX::SimpleMath::Vector3 normal;
				normal.x = Parsing_NumberFloat();
				normal.z = Parsing_NumberFloat();
				normal.y = Parsing_NumberFloat();

				int vertexNum = m_OneMesh->m_meshface[faceNum]->m_vertexindex[vertexIndex];

				// 한 번 노말이 셋팅 되었었는데, 이전 값과 지금 값이 다르다면 버텍스 추가해야됨
				if (m_OneMesh->m_meshvertex[vertexNum]->m_isnormalset == true
					&& m_OneMesh->m_meshvertex[vertexNum]->m_normal != normal)
				{
					ASEParser::Vertex* temp = new ASEParser::Vertex;

					temp->m_pos = m_OneMesh->m_meshvertex[vertexNum]->m_pos;

					temp->u = m_OneMesh->m_meshvertex[vertexNum]->u;
					temp->v = m_OneMesh->m_meshvertex[vertexNum]->v;

					temp->m_normal = normal;

					m_OneMesh->m_meshvertex.push_back(temp);

					// 면에서 새로 추가한 버텍스의 기존 인덱스 값을 새로 추가한 인덱스 값으로 변경
					for (int i = 0; i < 3; i++)
					{
						if (m_OneMesh->m_meshface[faceNum]->m_vertexindex[i] == vertexNum)
						{
							m_OneMesh->m_meshface[faceNum]->m_vertexindex[i] = m_OneMesh->m_meshvertex.size() - 1;
							m_OneMesh->m_meshvertex[m_OneMesh->m_meshvertex.size() - 1]->m_isnormalset = true;
						}
					}
				}
				else
				{
					m_OneMesh->m_meshvertex[vertexNum]->m_normal = normal;
					m_OneMesh->m_meshvertex[vertexNum]->m_isnormalset = true;
				}

				vertexIndex++;
				vertexIndex %= 3;
			}

			break;

			case TOKENR_TM_ANIMATION:
			{
				std::string nodeName = m_OneMesh->m_nodename;

				Animation* thisAnimation = new Animation;
				m_animations.push_back(thisAnimation);

				m_object->object[nodeName]->m_animation = thisAnimation;
				m_object->object[nodeName]->m_animation->m_nodename = nodeName;

				thisAnimation->m_firstFrame = m_scenedata.m_firstframe;
				thisAnimation->m_lastFrame = m_scenedata.m_lastframe;
				thisAnimation->m_ticksperFrame = m_scenedata.m_ticksperframe;
			}
			break;
			case TOKENR_CONTROL_POS_TRACK:
				break;
			case TOKENR_CONTROL_ROT_TRACK:
				break;
			case TOKENR_CONTROL_POS_SAMPLE:
			{
				CAnimation_pos* aniPos = new CAnimation_pos;

				aniPos->m_time = Parsing_NumberLong();
				aniPos->m_pos = Parsing_NumberVector3();

				m_object->object[m_OneMesh->m_nodename]->m_animation->m_position.emplace_back(aniPos);
			}
			break;
			case TOKENR_CONTROL_ROT_SAMPLE:
			{
				CAnimation_rot* aniRot = new CAnimation_rot;

				aniRot->m_time = Parsing_NumberLong();

				float xAxis = Parsing_NumberFloat();
				float zAxis = Parsing_NumberFloat();
				float yAxis = Parsing_NumberFloat();
				float angle = Parsing_NumberFloat();

				//float rotationAngleInRadians = DirectX::XMConvertToRadians(angle);

				XMVECTOR axis = { xAxis, yAxis, zAxis }; // 회전 축
				DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);

				/*
								Quaternion rotationQuaternion;
								rotationQuaternion.x = Parsing_NumberFloat();
								rotationQuaternion.z = Parsing_NumberFloat();
								rotationQuaternion.y = Parsing_NumberFloat();
								rotationQuaternion.w = Parsing_NumberFloat();

								rotationQuaternion.x = sinf(rotationQuaternion.w / 2.0f) * rotationQuaternion.x;
								rotationQuaternion.y = sinf(rotationQuaternion.w / 2.0f) * rotationQuaternion.y;
								rotationQuaternion.z = sinf(rotationQuaternion.w / 2.0f) * rotationQuaternion.z;
								rotationQuaternion.w = cosf(rotationQuaternion.w / 2.0f);*/

								// 첫 프레임
				if (m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.size() == 0)
				{
					DirectX::SimpleMath::Quaternion rotation = rotationQuaternion;
					aniRot->m_rotQT_accumulation = rotation;

					m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.emplace_back(aniRot);
				}
				else
				{
					DirectX::SimpleMath::Quaternion rotation = m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation[m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.size() - 1]->m_rotQT_accumulation;
					// 이전 거가 앞
					rotation = DirectX::XMQuaternionMultiply(rotation, rotationQuaternion);

					aniRot->m_rotQT_accumulation = rotation;
					m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.emplace_back(aniRot);
				}

				int a = 3;
			}
			break;

			case TOKEND_END:
				// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

				//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
				TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
				return;

				break;

				/// 위의 아무것도 해당하지 않을때
			default:
				// 아무것도 하지 않는다.
				break;

		}	// switch()

		///-----------------------------------------------
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			// 루프를 1000000번이상이나 돌 이유가 없다. (데이터가 100000개가 아닌이상)
			// 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		/// 안전 코드.
		///-----------------------------------------------

	}		// while()

	// 여기까지 왔다면 while()을 벗어났다는 것이고 그 말은
	// 괄호를 닫았다는 것이므로
	// 리턴하자 (재귀함수)

	return;
}


///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3개의 Float를 벡터 하나로
DirectX::SimpleMath::Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG				token;
	DirectX::SimpleMath::Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
}

///--------------------------------------------------
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// 클래스간 값 복사
	m_MeshList.push_back(m_OneMesh);
}

// 메트리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

/*
// 애니메이션데이터 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}*/

// 정점 하나를..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}






void CASEParser::MakeImpMesh()
{
	for (int i = 0; i < m_MeshList.size(); i++)
	{
		ImpStructure::ImpMesh* tempImpMesh = new ImpStructure::ImpMesh;
		tempImpMesh->_meshName = m_MeshList[i]->m_nodename;

		for (int j = 0; j < m_MeshList[i]->m_opt_vertex.size(); j++)
		{
			ImpStructure::ImpVertex* tempImpVertex = new ImpStructure::ImpVertex;

			tempImpVertex->_localPos.x = m_MeshList[i]->m_opt_vertex[j]->m_pos.x;
			tempImpVertex->_localPos.y = m_MeshList[i]->m_opt_vertex[j]->m_pos.y;
			tempImpVertex->_localPos.z = m_MeshList[i]->m_opt_vertex[j]->m_pos.z;

			tempImpVertex->_normal.x = m_MeshList[i]->m_opt_vertex[j]->m_normal.x;
			tempImpVertex->_normal.y = m_MeshList[i]->m_opt_vertex[j]->m_normal.y;
			tempImpVertex->_normal.z = m_MeshList[i]->m_opt_vertex[j]->m_normal.z;

			tempImpVertex->_u = m_MeshList[i]->m_opt_vertex[j]->u;
			tempImpVertex->_v = m_MeshList[i]->m_opt_vertex[j]->v;

			tempImpVertex->_weight.x = m_MeshList[i]->m_opt_vertex[j]->m_bw1;
			tempImpVertex->_weight.y = m_MeshList[i]->m_opt_vertex[j]->m_bw2;
			tempImpVertex->_weight.z = m_MeshList[i]->m_opt_vertex[j]->m_bw3;

			tempImpVertex->_weightIndices[0] = m_MeshList[i]->m_opt_vertex[j]->m_weightIndices[0];
			tempImpVertex->_weightIndices[1] = m_MeshList[i]->m_opt_vertex[j]->m_weightIndices[1];
			tempImpVertex->_weightIndices[2] = m_MeshList[i]->m_opt_vertex[j]->m_weightIndices[2];
			tempImpVertex->_weightIndices[3] = m_MeshList[i]->m_opt_vertex[j]->m_weightIndices[3];

			tempImpMesh->_meshVertex.push_back(tempImpVertex);
			tempImpMesh->_hasNormal = true;
			tempImpMesh->_hasTexture = true;
			tempImpMesh->_hasWeight = true;
		}

		// 교수님 기존 코드 참고
		UINT tcount;
		tcount = m_MeshList[i]->m_mesh_numfaces;

		UINT IndexCount = 3 * tcount;
		std::vector<UINT> indices(IndexCount);
		for (UINT j = 0; j < tcount; ++j)
		{
			ImpStructure::ImpFace* tempImpFace = new ImpStructure::ImpFace;

			tempImpFace->vertexIndex[0] = m_MeshList[i]->m_opt_index[j].index[0];
			tempImpFace->vertexIndex[1] = m_MeshList[i]->m_opt_index[j].index[2];
			tempImpFace->vertexIndex[2] = m_MeshList[i]->m_opt_index[j].index[1];

			tempImpMesh->_meshFace.push_back(tempImpFace);
			// 			indices[i * 3 + 0] = m_MeshList[i]->m_opt_index[i].index[0];
			// 			indices[i * 3 + 1] = m_MeshList[i]->m_opt_index[i].index[2];
			// 			indices[i * 3 + 2] = m_MeshList[i]->m_opt_index[i].index[1];
		}

		_meshes.push_back(tempImpMesh);
	}
}

void CASEParser::MakeImpObject()
{
	_object = new ImpStructure::ImpAnimationObject;

	for (const auto& node : m_object->object)
	{
		ImpStructure::ImpAnimationNode* impNode = new ImpStructure::ImpAnimationNode;

		impNode->_name = node.second->m_name;
		for (const auto& mesh : _meshes)
		{
			if (node.second->m_name == mesh->_meshName)
			{
				impNode->_mesh = mesh;
			}
		}

		impNode->_localTransform = ImpMath::Matrix(&node.second->m_localTransform._11);
		impNode->_worldTransform = ImpMath::Matrix(&node.second->m_worldTransform._11);

		if (node.second->m_bone != nullptr)
		{
			impNode->_bone = new ImpStructure::Bone;

			impNode->_bone->_boneName = node.second->m_bone->m_bone_name;
			impNode->_bone->_boneTM_NodeTM = ImpMath::Matrix(&node.second->m_bone->m_boneTM_NodeTM->_11);
			impNode->_bone->_boneTM_WorldTM = ImpMath::Matrix(&node.second->m_bone->m_boneTM_WorldTM->_11);

			_bones.push_back(impNode->_bone);
		}

// 		impNode->_animation = new ImpStructure::ImpAnimation;
// 		if (node.second->m_animation != nullptr)
// 		{
// 			for (const auto& posAnim : node.second->m_animation->m_position)
// 			{
// 				ImpStructure::AnimationPosition* ani_pos = new ImpStructure::AnimationPosition;
// 				ani_pos->_tick = posAnim->m_time;
// 				ani_pos->_pos.x = posAnim->m_pos.x;
// 				ani_pos->_pos.y = posAnim->m_pos.y;
// 				ani_pos->_pos.z = posAnim->m_pos.z;
// 				impNode->_animation->_positions.push_back(ani_pos);
// 			}
// 		}

		if (node.second->m_animation != nullptr)
		{
			for (const auto& rotAnim : node.second->m_animation->m_rotation)
			{
				ImpStructure::AnimationRotation* ani_rot = new ImpStructure::AnimationRotation;
				ani_rot->_tick = rotAnim->m_time;
				ani_rot->_rotQT_accumulation.x = rotAnim->m_rotQT_accumulation.x;
				ani_rot->_rotQT_accumulation.y = rotAnim->m_rotQT_accumulation.y;
				ani_rot->_rotQT_accumulation.z = rotAnim->m_rotQT_accumulation.z;
				ani_rot->_rotQT_accumulation.w = rotAnim->m_rotQT_accumulation.w;
				//impNode->_animation->_rotations.push_back(ani_rot);
			}
		}
		//final
		_object->_nodes[impNode->_name] = impNode;
	}

	for (const auto& node : m_object->object)
	{
		if (node.second->m_parentNode != nullptr)
		{
			_object->_nodes[node.first]->_parentNode = _object->_nodes[node.second->m_parentNode->m_name];
		}
		for (const auto& childNode : node.second->m_childNode)
		{
			_object->_nodes[node.first]->_childNodes.push_back(_object->_nodes[childNode->m_name]);
		}
	}

	_object->_maxAnimTime = m_scenedata.m_lastframe * m_scenedata.m_ticksperframe;
}

ImpStructure::ImpMesh* CASEParser::GetMesh(const char* file, size_t i)
{
	m_MeshList.clear();
	_meshes.clear();
	m_animations.clear();
	Load(const_cast<LPSTR>(file));
	for (int i = 0; i < m_MeshList.size(); i++)
	{
		ConvertAll(m_MeshList[i]);
	}
	MakeImpMesh();
	delete m_object;
	m_object = nullptr;
	for (const auto& mesh : m_MeshList)
	{
		delete mesh;
	}

	for (UINT meshIndex = 0; meshIndex < _meshes.size(); meshIndex++)
	{
		if (meshIndex == i)
		{
			continue;
		}
		delete _meshes[meshIndex];
	}
	for (UINT i = 0; i < m_vector_bone_list.size(); i++)
	{
		delete m_vector_bone_list[i];
	}
	m_vector_bone_list.clear();
	for (UINT i = 0; i < m_vector_wvertexs.size(); i++)
	{
		delete m_vector_wvertexs[i];
	}
	m_vector_wvertexs.clear();
	for (UINT i = 0; i < m_vector_weight.size(); i++)
	{
		delete m_vector_weight[i];
	}
	m_vector_weight.clear();
	m_lexer->Close();
	return _meshes[i];
}

std::vector<ImpStructure::ImpMesh*> CASEParser::GetMeshAll(const char* file)
{
	m_MeshList.clear();
	_meshes.clear();
	m_animations.clear();
	Load(const_cast<LPSTR>(file));
	for (int i = 0; i < m_MeshList.size(); i++)
	{
		ConvertAll(m_MeshList[i]);
	}
	MakeImpMesh();
	delete m_object;
	m_object = nullptr;
	for (const auto& mesh : m_MeshList)
	{
		delete mesh;
	}
	for (UINT i = 0; i < m_vector_bone_list.size(); i++)
	{
		delete m_vector_bone_list[i];
	}
	m_vector_bone_list.clear();
	for (UINT i = 0; i < m_vector_wvertexs.size(); i++)
	{
		delete m_vector_wvertexs[i];
	}
	m_vector_wvertexs.clear();
	for (UINT i = 0; i < m_vector_weight.size(); i++)
	{
		delete m_vector_weight[i];
	}
	m_vector_weight.clear();
	m_lexer->Close();
	return _meshes;
}

ImpStructure::ImpAnimationObject* CASEParser::GetAnimationObject(const char* file)
{
	m_MeshList.clear();
	_meshes.clear();
	m_animations.clear();
	Load(const_cast<LPSTR>(file));
	for (int i = 0; i < m_MeshList.size(); i++)
	{
		ConvertAll(m_MeshList[i]);
	}
	MakeImpMesh();
	MakeImpObject();
	delete m_object;
	m_object = nullptr;
	for (const auto& mesh : m_MeshList)
	{
		delete mesh;
	}
	for (UINT meshIndex = 0; meshIndex < _meshes.size(); meshIndex++)
	{
		delete _meshes[meshIndex];
	}
	for (UINT i = 0; i < m_vector_bone_list.size(); i++)
	{
		delete m_vector_bone_list[i];
	}
	m_vector_bone_list.clear();
	for (UINT i = 0; i < m_vector_wvertexs.size(); i++)
	{
		delete m_vector_wvertexs[i];
	}
	m_vector_wvertexs.clear();
	for (UINT i = 0; i < m_vector_weight.size(); i++)
	{
		delete m_vector_weight[i];
	}
	m_vector_weight.clear();
	m_lexer->Close();
	return _object;
}

size_t CASEParser::GetMeshSize(const char* file)
{
	m_MeshList.clear();
	_meshes.clear();
	m_animations.clear();
	Load(const_cast<LPSTR>(file));
	delete m_object;
	m_object = nullptr;
	for (const auto& mesh : m_MeshList)
	{
		delete mesh;
	}
	for (UINT meshIndex = 0; meshIndex < _meshes.size(); meshIndex++)
	{
		delete _meshes[meshIndex];
	}
	for (UINT i = 0; i < m_vector_bone_list.size(); i++)
	{
		delete m_vector_bone_list[i];
	}
	m_vector_bone_list.clear();
	for (UINT i = 0; i < m_vector_wvertexs.size(); i++)
	{
		delete m_vector_wvertexs[i];
	}
	m_vector_wvertexs.clear();
	for (UINT i = 0; i < m_vector_weight.size(); i++)
	{
		delete m_vector_weight[i];
	}
	m_vector_weight.clear();
	m_lexer->Close();
	return _meshes.size();
}

std::vector<ImpStructure::Bone*> CASEParser::GetBoneList()
{
	reverse(_bones.begin(), _bones.begin() + _bones.size());

	return _bones;
}