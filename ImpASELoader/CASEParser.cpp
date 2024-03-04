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
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// 1) �ٷ��̾ƺ����豳���� �Ŀ�Ǯ�� ����Լ� �Ѱ��� ó�� ��!
	m_object = new Object;
	Parsing_DivergeRecursiveALL(0);
	MakeFaceTangent();
	LerpAnimation();

	// ���ؽ��� �ߺ��� Ȯ���ϱ� ���� ����ߴ� ��
// 	// ���ؽ��� �ߺ��� �ִ�. ���� ���������� �翬�� ��.
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

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// ���� ���� (���ÿ�)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// ���ؽ����� �ϴ� ��� ����
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

	//TODO: �� ���⸦ �̷��� �� �ξ�����?
// 	// �������� face�� �������� �� �ε����� ã�ƾ� ��
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
// 			// face�������� ���ؽ��� �븻�� �־��ش�.
// 			// (���ÿ�)
// 			_nowVertex->m_normal.x = 0;
// 			_nowVertex->m_normal.y = 0;
// 			_nowVertex->m_normal.z = 0;
// 		}
// 	}

	// �ε����� �׳� ����
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
			// ��ǥ�谡 �ٸ� ���� ����� ���ַ��� �ʿ��ϴ�. ������ ...
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
// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
//
// �ϴ� �а�, ��ū�� ���� �����Ѵ�.
//
// ��͸� ���� ������ { �� ���� ����̴�.
// �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ���̴�.
//
// ���Ҿ�, ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� � ��������� �����Ѵ�.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// ���������� ����
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;
	// ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
	int listcount = 0;		// �ſ� C����� �ٸ� �ذ�å�� ������?
	// �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
	// �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
	LONG nowtoken;
	//----------------------------------------------------------------------

	DirectX::SimpleMath::Matrix TM1, TM2;
	DirectX::SimpleMath::Quaternion tempQT;
	DirectX::SimpleMath::Quaternion prevQT;
	DirectX::SimpleMath::Quaternion resultQT;

	// ���Ⱑ �´� �� �𸣰ڴ�.
	// �ϴ� vertexnormal�� �ؼ� vertex�� split�Ҷ� mesh facenormal�� �ε��� ���� �ʿ��ϴ�.
	int faceNum = -1;
	int vertexIndex = 0;

	/// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
		// �Ѿ�� ��ū�� ���� ó�����ش�.

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
				Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
				//AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
				break;

				//--------------------
				// SCENE
				//--------------------

			case TOKENR_SCENE:
				//
				break;
			case TOKENR_SCENE_FILENAME:
				m_scenedata.m_filename = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
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
				// �׳� ���� ������ �о������ }�� ���ö�����
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
				//	�� ���� �׷� ����. �� ������ �̸��� ��Ʈ������ ������� �ϴµ�.
				break;

			case TOKENR_HELPEROBJECT:
				Create_onemesh_to_list();
				// �ϴ� �����ϰ�
				// ������Ʈ�� Ÿ�� ������. �̰Ϳ� ���� ���� �ٸ� �Ľ� ��� �ߵ�.
				break;

			case TOKENR_GEOMOBJECT:
				/// �� ��ū�� �����ٴ°� ���ο� �޽ð� ����ٴ� ���̴�. 
				/// ���� ������ mesh�� �ϳ� ����, �� �����͸� ����Ʈ�� �ְ�, 
				/// m_onemesh�� �� �����͸� ����, �״�� ���� �ɱ�?

				// y: �ϴ� �𸣰ڰ� Mesh�� �ϳ� �����.
				//m_OneMesh = new Mesh; ���ϱ� �ؿ��� �����. Mesh�϶�
				Create_onemesh_to_list();
				break;

			case TOKENR_SHAPEOBJECT:
				Create_onemesh_to_list();
				break;

			case TOKENR_NODE_NAME:
				// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
				// ��忡 ���� �־�� �� ���� �ٸ���.

				m_OneMesh->m_nodename = Parsing_String();

				// �� ���� �޽��� ��� ������ 2�� ������ �ϴµ� map[]�� Ư���� �̹� ���� ������ ����������ϱ� ������ �ȴ�.
				// ȿ���� ...? ���ʿ� new�� �̻� ������ �ִ� �� Ȯ���ؾ� �Ѵ�.
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
				// �� ����� �θ� ����� ����.
				// �ϴ� �Է��� �ϰ�, ���߿� ��������.

				break;

				/// NODE_TM

			case TOKENR_NODE_TM:
				//m_parsingmode	=	eGeomobject;
				// (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
				// (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)

				/// �Դٰ�,
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
				// �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
				//if (m_onemesh->m_camera_isloadTarget) {
				//	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
				// ��ͱ����� �������̴�....

				break;

			case TOKENR_INHERIT_POS:
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
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

				// vector3 4���� ���ڷ� �޴� �����ڰ� ����.
				DirectX::SimpleMath::Vector4 row3 = m_OneMesh->m_tm_row3;

				row3.w = 1.0f;

				DirectX::SimpleMath::Matrix meshMatrix{ DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row0), DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row2), DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row1), row3 };

				m_object->object[m_OneMesh->m_nodename]->m_worldTransform = meshMatrix;
				// skinning��
				if (m_object->object[m_OneMesh->m_nodename]->m_bone != nullptr)
				{
					m_object->object[m_OneMesh->m_nodename]->m_bone->m_boneTM_NodeTM = new DirectX::SimpleMath::Matrix(meshMatrix);
					m_object->object[m_OneMesh->m_nodename]->m_bone->m_boneTM_WorldTM = new DirectX::SimpleMath::Matrix(meshMatrix);
				}

				// ����Ʈ������ �����
				if (m_object->object[m_OneMesh->m_nodename]->m_parentNode != nullptr)
				{
					// ����� ���� �ɰ�
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
				// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
				break;


				/// MESH

			case TOKENR_MESH:
			{
				/// �Ʒ��� �Լ����� m_OneMesh�� �����.
				// ��ġ�� �ٲ㺻��.
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
				// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
				// ���� �Է�
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
				// ���ؽ��� ������ ����־�� �ϴµ�
				// �̹� ���ͷ� ������ �� �����Ƿ� �׳� ������ �ȴ�.
				break;
			case TOKENR_MESH_VERTEX:
				// ������ �Է�
			{
				Parsing_NumberInt();

				ASEParser::Vertex* tempVertex = new ASEParser::Vertex;

				// y, z�� �ݴ�� �о�� �Ѵ�.
				tempVertex->m_pos.x = Parsing_NumberFloat();
				tempVertex->m_pos.z = Parsing_NumberFloat();
				tempVertex->m_pos.y = Parsing_NumberFloat();

				// ����� ���ؼ� ���� �����̽��� �ű��
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
				/// ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
				// �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
				// Bone�� �ѹ��� �о� ����
				Bone* temp = new Bone;
				temp->m_bone_number = Parsing_NumberInt();

				m_vector_bone_list.emplace_back(temp);
			}
			break;
			//�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
			case TOKENR_BONE_NAME:
				m_vector_bone_list[m_vector_bone_list.size() - 1]->m_bone_name = Parsing_String();
			case TOKENR_BONE_PROPERTY:
				// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
				break;
				// �������� TM_ROW0~3�� �����µ� ���� ���õ�..

			case TOKENR_MESH_WVERTEXS:
				m_vector_wvertexs = {};
				break;

			case TOKENR_MESH_WEIGHT:
			{
				// ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
				VertexWeight* temp = new VertexWeight;
				temp->m_wvertex_number = Parsing_NumberInt();

				m_vector_wvertexs.push_back(temp);
			}
			break;
			case TOKENR_BONE_BLENGING_WEIGHT:
			{
				// ��ü �� �ܰ踦 ���°ž�...
				// ����ġ �Ѱ��� ���� ����Ʈ�� �ִ´�
				/// �� ��....
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
				std::string a = Parsing_String(); // �� ��ȣ
				std::string b = Parsing_String(); // A:

				ASEParser::Face* temp = new ASEParser::Face;

				// Y�� Z�� �ٲ㼭 �о�� �Ѵ�. �� �ٲٴ� �� �´� �� ����. �� �ٲٴ� �� �´� �� �ƴ϶� �޽� ������Ʈ���� ������ ��
				// 1�ϰ� 2�� �ٲ㼭 �־��ְ� �־���. �ϰ����� ���� �ļ����� �ٲ� �ִ°ɷ� ���� �ߴµ�
				// ���⼭ �ٲٸ� �븻, �ؽ�ó, �ε��� �� �ٲ�� �ؼ� �׳� ���ʿ��� �ٲٱ�� �ߴ�... �ϴ���!
				temp->m_vertexindex[0] = Parsing_NumberInt();
				Parsing_String();
				temp->m_vertexindex[1] = Parsing_NumberInt();
				Parsing_String();
				temp->m_vertexindex[2] = Parsing_NumberInt();

				m_OneMesh->m_meshface.push_back(temp);

				// ������ ��ū�� ���� Ȯ���غ��� �Ѵ�.
				// Face�� ��ȣ�ε�...

				// A:�� �а�
				// B:
				// C:

				/// (�ڿ� ������ �� ������ default�� ���� ��ŵ�� ���̴�.)
				/// ......

				// ���Ϳ� �־��ش�.
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
				// ���ؽ��� �ε����� �����µ� ������ ������ �����Ƿ� ������.
				// ���ο� TVertex�� ���� ���Ϳ� �ִ´�
				COneTVertex* tVertex = new COneTVertex;

				Parsing_NumberInt();

				// �̹� ������ �� �ؽ�ó�� ���� �ֱ� ������ 1 - �� ���� �ʴ´�.
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

				// �ؽ�ó ���ؽ� ���ø�
				for (int i = 0; i < 3; i++)
				{
					// �̹� �ؽ�ó uv���� ���ؽ��� ���� �Ǿ��µ� �̹��� ���� ���ϰ� �ٸ��ٸ�
					// or ���� ��ȣ�� �ľ� �ϴ� �� ����. ������ �켱������ ������ ���� ������
					if (m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->m_istextureset == true
						&& (m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->u != m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_u
							|| m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->v != m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_v))
					{
						ASEParser::Vertex* temp = new ASEParser::Vertex;

						temp->m_pos = m_OneMesh->m_meshvertex[m_OneMesh->m_meshface[tFaceNum]->m_vertexindex[i]]->m_pos;

						temp->u = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_u;
						temp->v = m_OneMesh->m_mesh_tvertex[m_OneMesh->m_meshface[tFaceNum]->m_TFace[i]]->m_v;

						m_OneMesh->m_meshvertex.push_back(temp);

						// �븻 ���� ���� ���� �ſ� �븻 ���� �ؾߵȴ�.
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
				//������ mesh_vertexnormal ���� �о�Դµ� �ؽ�ó���� ���ؽ��� ���ø� �ϴ��� ������ �ٲ����.
				/ *int vertexNum = * /Parsing_NumberInt();

				DirectX::SimpleMath::Vector3 normal;
				normal.x = Parsing_NumberFloat();
				normal.z = Parsing_NumberFloat();
				normal.y = Parsing_NumberFloat();

				int vertexNum = m_OneMesh->m_meshface[faceNum]->m_vertexindex[vertexIndex];

				// �� �� �븻�� ���� �Ǿ����µ�, ���� ���� ���� ���� �ٸ��ٸ� ���ؽ� �߰��ؾߵ�
				if (m_OneMesh->m_meshvertex[vertexNum]->m_isnormalset == true
					&& m_OneMesh->m_meshvertex[vertexNum]->m_normal != normal)
				{
					ASEParser::Vertex* temp = new ASEParser::Vertex;

					temp->m_pos = m_OneMesh->m_meshvertex[vertexNum]->m_pos;

					temp->u = m_OneMesh->m_meshvertex[vertexNum]->u;
					temp->v = m_OneMesh->m_meshvertex[vertexNum]->v;

					temp->m_normal = normal;

					m_OneMesh->m_meshvertex.push_back(temp);

					// �鿡�� ���� �߰��� ���ؽ��� ���� �ε��� ���� ���� �߰��� �ε��� ������ ����
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

					// �ؽ�ó ���ؽ� ���ø� �ߴ� �Ϳ� �߰�
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

				XMVECTOR axis = { xAxis, yAxis, zAxis }; // ȸ�� ��
				DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationAxis(axis, angle);

				// ù ������
				DirectX::SimpleMath::Quaternion rotation;
				if (m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.size() == 0)
				{
					rotation = rotationQuaternion;
					aniRot->m_rotQT_accumulation = rotation;

					if (aniRot->m_time != 0)
					{
						CAnimation_rot* firstRot = new CAnimation_rot;
						firstRot->m_time = 0;

						XMVECTOR tempAxis = m_OneMesh->m_tm_rotaxis; // ȸ�� ��
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
					// ���� �Ű� ��
					rotation = DirectX::XMQuaternionMultiply(rotation, rotationQuaternion);

					aniRot->m_rotQT_accumulation = rotation;
					m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.emplace_back(aniRot);
				}
			}
			break;

			case TOKEND_END:
				// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

				//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
				TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
				return;

				break;

				/// ���� �ƹ��͵� �ش����� ������
			default:
				// �ƹ��͵� ���� �ʴ´�.
				break;

		}	// switch()

		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	// ������� �Դٸ� while()�� ����ٴ� ���̰� �� ����
	// ��ȣ�� �ݾҴٴ� ���̹Ƿ�
	// �������� (����Լ�)

	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
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
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
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


// 3���� Float�� ���� �ϳ���
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

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

/ *
// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}* /

// ���� �ϳ���..
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
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// 1) �ٷ��̾ƺ����豳���� �Ŀ�Ǯ�� ����Լ� �Ѱ��� ó�� ��!
	m_object = new Object;
	Parsing_DivergeRecursiveALL(0);

	//LerpAnimation();
	// 	// ���ؽ��� �ߺ��� �ִ�. ���� ���������� �翬�� ��.
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

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// ���� ���� (���ÿ�)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// ���ؽ����� �ϴ� ��� ����
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

	//TODO: �� ���⸦ �̷��� �� �ξ�����?
// 	// �������� face�� �������� �� �ε����� ã�ƾ� ��
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
// 			// face�������� ���ؽ��� �븻�� �־��ش�.
// 			// (���ÿ�)
// 			_nowVertex->m_normal.x = 0;
// 			_nowVertex->m_normal.y = 0;
// 			_nowVertex->m_normal.z = 0;
// 		}
// 	}

	// �ε����� �׳� ����
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
// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
//
// �ϴ� �а�, ��ū�� ���� �����Ѵ�.
//
// ��͸� ���� ������ { �� ���� ����̴�.
// �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ���̴�.
//
// ���Ҿ�, ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� � ��������� �����Ѵ�.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// ���������� ����
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;
	// ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
	int listcount = 0;		// �ſ� C����� �ٸ� �ذ�å�� ������?
	// �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
	// �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
	LONG nowtoken;
	//----------------------------------------------------------------------

	DirectX::SimpleMath::Matrix TM1, TM2;
	DirectX::SimpleMath::Quaternion tempQT;
	DirectX::SimpleMath::Quaternion prevQT;
	DirectX::SimpleMath::Quaternion resultQT;

	// ���Ⱑ �´� �� �𸣰ڴ�.
	// �ϴ� vertexnormal�� �ؼ� vertex�� split�Ҷ� mesh facenormal�� �ε��� ���� �ʿ��ϴ�.
	int faceNum = -1;
	int vertexIndex = 0;

	/// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
		// �Ѿ�� ��ū�� ���� ó�����ش�.

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
				Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
				//AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
				break;

				//--------------------
				// SCENE
				//--------------------

			case TOKENR_SCENE:
				//
				break;
			case TOKENR_SCENE_FILENAME:
				m_scenedata.m_filename = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
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
				// �׳� ���� ������ �о������ }�� ���ö�����
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
				//	�� ���� �׷� ����. �� ������ �̸��� ��Ʈ������ ������� �ϴµ�.
				break;

			case TOKENR_HELPEROBJECT:
				Create_onemesh_to_list();
				// �ϴ� �����ϰ�
				// ������Ʈ�� Ÿ�� ������. �̰Ϳ� ���� ���� �ٸ� �Ľ� ��� �ߵ�.
				break;

			case TOKENR_GEOMOBJECT:
				/// �� ��ū�� �����ٴ°� ���ο� �޽ð� ����ٴ� ���̴�. 
				/// ���� ������ mesh�� �ϳ� ����, �� �����͸� ����Ʈ�� �ְ�, 
				/// m_onemesh�� �� �����͸� ����, �״�� ���� �ɱ�?

				// y: �ϴ� �𸣰ڰ� Mesh�� �ϳ� �����.
				//m_OneMesh = new Mesh; ���ϱ� �ؿ��� �����. Mesh�϶�
				Create_onemesh_to_list();
				break;

			case TOKENR_SHAPEOBJECT:
				Create_onemesh_to_list();
				break;

			case TOKENR_NODE_NAME:
				// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
				// ��忡 ���� �־�� �� ���� �ٸ���.

				m_OneMesh->m_nodename = Parsing_String();

				// �� ���� �޽��� ��� ������ 2�� ������ �ϴµ� map[]�� Ư���� �̹� ���� ������ ����������ϱ� ������ �ȴ�.
				// ȿ���� ...? ���ʿ� new�� �̻� ������ �ִ� �� Ȯ���ؾ� �Ѵ�.
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
				// �� ����� �θ� ����� ����.
				// �ϴ� �Է��� �ϰ�, ���߿� ��������.

				break;

				/// NODE_TM

			case TOKENR_NODE_TM:
				//m_parsingmode	=	eGeomobject;
				// (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
				// (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)

				/// �Դٰ�,
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
				// �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
				//if (m_onemesh->m_camera_isloadTarget) {
				//	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
				// ��ͱ����� �������̴�....

				break;

			case TOKENR_INHERIT_POS:
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
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

				// vector3 4���� ���ڷ� �޴� �����ڰ� ����.
				DirectX::SimpleMath::Vector4 row3 = m_OneMesh->m_tm_row3;

				row3.w = 1.0f;

				DirectX::SimpleMath::Matrix meshMatrix{ DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row0), DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row2), DirectX::SimpleMath::Vector4(m_OneMesh->m_tm_row1), row3 };

				m_object->object[m_OneMesh->m_nodename]->m_worldTransform = meshMatrix;

				// skinning��
				if (m_object->object[m_OneMesh->m_nodename]->m_bone != nullptr)
				{
					m_object->object[m_OneMesh->m_nodename]->m_bone->m_boneTM_NodeTM = new DirectX::SimpleMath::Matrix(meshMatrix);
					m_object->object[m_OneMesh->m_nodename]->m_bone->m_boneTM_WorldTM = new DirectX::SimpleMath::Matrix(meshMatrix);
				}

				// ����Ʈ������ �����
				if (m_object->object[m_OneMesh->m_nodename]->m_parentNode != nullptr)
				{
					// ����� ���� �ɰ�
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
				// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
				break;


				/// MESH

			case TOKENR_MESH:
			{
				/// �Ʒ��� �Լ����� m_OneMesh�� �����.
				// ��ġ�� �ٲ㺻��.
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
				// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
				// ���� �Է�
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
				// ���ؽ��� ������ ����־�� �ϴµ�
				// �̹� ���ͷ� ������ �� �����Ƿ� �׳� ������ �ȴ�.
				break;
			case TOKENR_MESH_VERTEX:
				// ������ �Է�
			{
				Parsing_NumberInt();

				ASEParser::Vertex* tempVertex = new ASEParser::Vertex;

				// y, z�� �ݴ�� �о�� �Ѵ�.
				tempVertex->m_pos.x = Parsing_NumberFloat();
				tempVertex->m_pos.z = Parsing_NumberFloat();
				tempVertex->m_pos.y = Parsing_NumberFloat();

				// ����� ���ؼ� ���� �����̽��� �ű��
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
				/// ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
				// �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
				// Bone�� �ѹ��� �о� ����
				Bone* temp = new Bone;
				temp->m_bone_number = Parsing_NumberInt();

				m_vector_bone_list.emplace_back(temp);
			}
			break;
			//�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
			case TOKENR_BONE_NAME:
				m_vector_bone_list[m_vector_bone_list.size() - 1]->m_bone_name = Parsing_String();
			case TOKENR_BONE_PROPERTY:
				// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
				break;
				// �������� TM_ROW0~3�� �����µ� ���� ���õ�..

			case TOKENR_MESH_WVERTEXS:
				m_vector_wvertexs = {};
				break;

			case TOKENR_MESH_WEIGHT:
			{
				// ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
				VertexWeight* temp = new VertexWeight;
				temp->m_wvertex_number = Parsing_NumberInt();

				m_vector_wvertexs.push_back(temp);
			}
			break;
			case TOKENR_BONE_BLENGING_WEIGHT:
			{
				// ��ü �� �ܰ踦 ���°ž�...
				// ����ġ �Ѱ��� ���� ����Ʈ�� �ִ´�
				/// �� ��....
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
				std::string a = Parsing_String(); // �� ��ȣ
				std::string b = Parsing_String(); // A:

				ASEParser::Face* temp = new ASEParser::Face;

				// Y�� Z�� �ٲ㼭 �о�� �Ѵ�. �� �ٲٴ� �� �´� �� ����. �� �ٲٴ� �� �´� �� �ƴ϶� �޽� ������Ʈ���� ������ ��
				// 1�ϰ� 2�� �ٲ㼭 �־��ְ� �־���. �ϰ����� ���� �ļ����� �ٲ� �ִ°ɷ� ���� �ߴµ�
				// ���⼭ �ٲٸ� �븻, �ؽ�ó, �ε��� �� �ٲ�� �ؼ� �׳� ���ʿ��� �ٲٱ�� �ߴ�... �ϴ���!
				temp->m_vertexindex[0] = Parsing_NumberInt();
				Parsing_String();
				temp->m_vertexindex[1] = Parsing_NumberInt();
				Parsing_String();
				temp->m_vertexindex[2] = Parsing_NumberInt();

				m_OneMesh->m_meshface.push_back(temp);

				// ������ ��ū�� ���� Ȯ���غ��� �Ѵ�.
				// Face�� ��ȣ�ε�...

				// A:�� �а�
				// B:
				// C:

				/// (�ڿ� ������ �� ������ default�� ���� ��ŵ�� ���̴�.)
				/// ......

				// ���Ϳ� �־��ش�.
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
				// ���ؽ��� �ε����� �����µ� ������ ������ �����Ƿ� ������.
				// ���ο� TVertex�� ���� ���Ϳ� �ִ´�
				COneTVertex* tVertex = new COneTVertex;

				Parsing_NumberInt();

				// �̹� ������ �� �ؽ�ó�� ���� �ֱ� ������ 1 - �� ���� �ʴ´�.
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

				// �ؽ�ó ���ؽ� ���ø�
				for (int i = 0; i < 3; i++)
				{
					// �̹� �ؽ�ó uv���� ���ؽ��� ���� �Ǿ��µ� �̹��� ���� ���ϰ� �ٸ��ٸ�
					// or ���� ��ȣ�� �ľ� �ϴ� �� ����. ������ �켱������ ������ ���� ������
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
				//������ mesh_vertexnormal ���� �о�Դµ� �ؽ�ó���� ���ؽ��� ���ø� �ϴ��� ������ �ٲ����.
				/*int vertexNum = */Parsing_NumberInt();

				DirectX::SimpleMath::Vector3 normal;
				normal.x = Parsing_NumberFloat();
				normal.z = Parsing_NumberFloat();
				normal.y = Parsing_NumberFloat();

				int vertexNum = m_OneMesh->m_meshface[faceNum]->m_vertexindex[vertexIndex];

				// �� �� �븻�� ���� �Ǿ����µ�, ���� ���� ���� ���� �ٸ��ٸ� ���ؽ� �߰��ؾߵ�
				if (m_OneMesh->m_meshvertex[vertexNum]->m_isnormalset == true
					&& m_OneMesh->m_meshvertex[vertexNum]->m_normal != normal)
				{
					ASEParser::Vertex* temp = new ASEParser::Vertex;

					temp->m_pos = m_OneMesh->m_meshvertex[vertexNum]->m_pos;

					temp->u = m_OneMesh->m_meshvertex[vertexNum]->u;
					temp->v = m_OneMesh->m_meshvertex[vertexNum]->v;

					temp->m_normal = normal;

					m_OneMesh->m_meshvertex.push_back(temp);

					// �鿡�� ���� �߰��� ���ؽ��� ���� �ε��� ���� ���� �߰��� �ε��� ������ ����
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

				XMVECTOR axis = { xAxis, yAxis, zAxis }; // ȸ�� ��
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

								// ù ������
				if (m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.size() == 0)
				{
					DirectX::SimpleMath::Quaternion rotation = rotationQuaternion;
					aniRot->m_rotQT_accumulation = rotation;

					m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.emplace_back(aniRot);
				}
				else
				{
					DirectX::SimpleMath::Quaternion rotation = m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation[m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.size() - 1]->m_rotQT_accumulation;
					// ���� �Ű� ��
					rotation = DirectX::XMQuaternionMultiply(rotation, rotationQuaternion);

					aniRot->m_rotQT_accumulation = rotation;
					m_object->object[m_OneMesh->m_nodename]->m_animation->m_rotation.emplace_back(aniRot);
				}

				int a = 3;
			}
			break;

			case TOKEND_END:
				// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

				//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
				TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
				return;

				break;

				/// ���� �ƹ��͵� �ش����� ������
			default:
				// �ƹ��͵� ���� �ʴ´�.
				break;

		}	// switch()

		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	// ������� �Դٸ� while()�� ����ٴ� ���̰� �� ����
	// ��ȣ�� �ݾҴٴ� ���̹Ƿ�
	// �������� (����Լ�)

	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
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
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
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


// 3���� Float�� ���� �ϳ���
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

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

/*
// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}*/

// ���� �ϳ���..
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

		// ������ ���� �ڵ� ����
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