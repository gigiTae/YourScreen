#include "CParsingDataClass.h"

namespace ASEParser
{
	Vertex::Vertex()
		: m_pos(), m_normal(), m_isnormalset(false)
		, u(0.0f), v(0.0f), m_istextureset(false)
		, m_bw1(0.0f), m_bw2(0.0f), m_bw3(0.0f), m_indices(0), m_indicesPointer(nullptr)
	{

	}

	Face::Face()
	{
		m_vertexindex[0] = 0;
		m_vertexindex[1] = 0;
		m_vertexindex[2] = 0;

		m_normal = Vector3(0, 0, 0);
	}

	COneTVertex::COneTVertex()
	{
		m_u = 0;
		m_v = 0;
		m_w = 0;
	}

	ShapeLine::ShapeLine()
	{

		m_line_number = 0;		// 몇번째 라인인가?
		m_shape_closed = false;	// 닫힌 Shape인가?
		m_shape_vertexcount = 0;		// 몇 개의 Vertex가 있는가?
		//m_shapevertex;					// 버텍스들의 vector
	}

	Scenedata::Scenedata()
	{
		//m_filename;		CString 초기화 필요없음?
		m_firstframe = 0;
		m_lastframe = 0;
		m_framespeed = 0;
		m_ticksperframe = 0;
		m_meshframestep = 0;
		m_keyframestep = 0;
		m_scene_background_static = Vector3(0, 0, 0);
		m_scene_ambient_static = Vector3(0, 0, 0);
	}

	Scenedata::~Scenedata()
	{

	}

	MaterialMap::MaterialMap()
	{
		// 	CString		m_map_name;
		// 	CString		m_map_class;
		m_subno = 0;
		m_map_amount = 0;
		//	CString		m_bitmap;
		m_map_type = 0;
		m_uvw_u_offset = 0;
		m_uvw_v_offset = 0;
		m_u_tiling = 0;
		m_v_tiling = 0;
		m_uvw_angle = 0;
		m_uvw_blur = 0;
		m_uvw_blur_offset = 0;
		m_uvw_noise_amt = 0;
		m_uvw_noise_size = 0;
		m_uvw_noise_level = 0;
		m_uvw_noise_phase = 0;
		m_bitmap_filter = 0;
	}

	ASEMaterial::ASEMaterial()
	{
		m_materialnumber = 0;	// Material의 번호. 이것을 기준으로 오브젝트에서 참조한다.
		//CString					m_material_name;
		//CString					m_material_class;
		m_material_ambient = Vector3(0, 0, 0);
		m_material_diffuse = Vector3(0, 0, 0);
		m_material_specular = Vector3(0, 0, 0);
		m_material_shine = 0.0f;
		m_material_shinestrength = 0.0f;
		m_material_transparency = 0.0f;
		m_material_wiresize = 0;
		m_material_shading = 0;
		m_material_xp_falloff = 0.0f;
		m_material_selfillum = 0.0f;
		m_material_falloff = 0.0f;
		m_material_xp_type = 0;

		m_numsubmtls = 0;

		m_istextureexist = false;
	}

	ASEMaterial::~ASEMaterial()
	{

	}

	Light_AnimationData::Light_AnimationData()
	{
		// Light Setting
		m_timevalue = -1;// 이건 애니메이션이 없는경우의 셋팅이다. 그런데 시간이 -1인 경우가 있으면 어쩌지?
		m_light_color = Vector3(1.0f, 1.0f, 1.0f);
		m_light_intens = 0.0f;
		m_light_aspect = 0.0f;
		m_light_hotspot = 0.0f;		///
		m_light_falloff = 0.0f;		/// 이 두가지는 SpotLight인 경우
		m_light_tdist = 0.0f;
		m_light_mapbias = 0.0f;
		m_light_maprange = 0.0f;
		m_light_mapsize = 0;
		m_light_raybias = 0.0f;
	}

	Mesh::Mesh()
	{
		m_type = 0;
		m_isAnimated = false;
		//CString				m_nodename;				// CString은 자동으로 초기화 시켜주는가?
		//CString				m_nodeparent;
		m_isparentexist = false;					// 기본적으로 부모 노드는 존재치 않는다.

		//CString				m_meshname;
		m_inherit_pos = Vector3(0, 0, 0);
		m_inherit_rot = Vector3(0, 0, 0);
		m_inherit_scl = Vector3(0, 0, 0);
		m_tm_row0 = Vector3(0, 0, 0);
		m_tm_row1 = Vector3(0, 0, 0);
		m_tm_row2 = Vector3(0, 0, 0);
		m_tm_row3 = Vector3(0, 0, 0);
		m_tm_pos = Vector3(0, 0, 0);
		m_tm_rotaxis = Vector3(0, 0, 0);
		m_tm_rotangle = 0;
		m_tm_scale = Vector3(0, 0, 0);
		m_tm_scaleaxis = Vector3(0, 0, 0);
		m_tm_scaleaxisang = 0;

		m_timevalue = 0;
		m_numbone = 0;					/// 본의 갯수. 이것이 있다면 스키닝 오브젝트로 간주한다.
		m_numskinweight = 0;					/// 스킨 가중치. 뭔소린지?
		m_mesh_numvertex = 0;
		m_mesh_numfaces = 0;

		//Vector3*		m_meshvertex;			// 이건 버텍스
		//Vector3*		m_meshface;				// 이게 페이스(버텍스 인덱스, 노말)

		m_mesh_numtvertex = 0;

		//m_mesh_tverlist		// 텍스쳐의 버텍스 리스트
		m_mesh_tvfaces = 0;
		//m_mesh_tfacelist	=	// 텍스쳐의 페이스 리스트
		m_mesh_numcvertex = 0;

		//m_mesh_facenormal		// Face의	normal
		//m_mesh_vertexnormal	// Vertex의 normal

		m_prop_motionblur = 0;
		m_prop_castshadow = 0;
		m_prop_recvshadow = 0;
		m_wireframe_color = Vector3(0, 0, 0);


		///----------------------------------
		/// Camera
		///----------------------------------
		//CString				m_camera_type;
		// CAMERA_SETTINGS
		m_timevalue = 0;
		m_camera_near = 0.0f;
		m_camera_fov = 0.0f;
		m_camera_tdist = 0.0f;

		m_camera_isloadTarget = false;


		///----------------------------------
		/// Light
		///----------------------------------
		//CString				m_light_type;
		// 이녀석도 Target이 있군..

		// Light Property
		//CString				m_light_shadows;
		m_light_uselight = false;
		//CString				m_spotshape;
		m_light_useglobal = false;
		m_light_absmapbias = false;
		m_light_overshoot = false;

		// Light Setting
		m_light_isloadTarget = false;
		m_light_isAnimate = false;


		///----------------------------------
		/// Skinning Object
		///----------------------------------
		// SKIN_INITTM은 무시하고
		// TM_ROW0~3
		// BONE_LIST
		m_is_bone = false;		// 스키닝 오브젝트인가?
		//vector<CBone*>			m_vector_bone_list;		// 이건 그냥 번호와 이름 관계이다. 이 안에 포인터를 넣어놓아도 괜찮겠구나(참조하기 편하게 - 그렇지 않으면 매번 이름으로 검색을 해야될지도. 아니면 인덱스가 있으니 상수 시간대로 접근을 할수도?)
		//vector<CWeight_Vertex*>	m_vector_wvertexs;		// 버텍스 하나에 달라붙어있는 가중치들

	}

	void Node::Transform(Vector3 distance, float angle, Vector3 value)
	{
		// 분해해서 변한 값 적용
		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;

		m_localTransform.Decompose(scale, rotation, translation);

		// 스케일
		scale += value;

		// 회전
		float rotationAngleInRadians = DirectX::XMConvertToRadians(angle);

		XMVECTOR axis = { 1.0f, 0.0f, 0.0f }; // 회전 축
		DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationAxis(axis, rotationAngleInRadians);

		// 현재의 Quaternion을 회전 Quaternion으로 결합할 수 있습니다.
		rotation = DirectX::XMQuaternionMultiply(rotation, rotationQuaternion);

		// 이동
		translation += distance;

		DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
		DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(translation);

		// 분해 한것 다시 합치기
		Matrix transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

		m_localTransform = transformMatrix;

		// 부모의 행렬 값 구하기
		Node* curNode = m_parentNode;
		Matrix totalMat = {};

		while (curNode != nullptr)
		{
			totalMat *= curNode->m_localTransform;
			curNode = curNode->m_parentNode;
		}

		SetNode(totalMat, /*temp*/totalMat);
	}

	void Node::SetNode(Matrix parentMat, Matrix meshTM)
	{
		m_worldTransform = m_localTransform * parentMat;

		if (m_bone != nullptr)
		{
			m_bone->m_boneTM_WorldTM = new Matrix(m_worldTransform);
		}

		if (m_childNode.size() == 0) return;

		for (int i = 0; i < m_childNode.size(); i++)
		{
			m_childNode[i]->SetNode(m_worldTransform, meshTM);
		}
	}

	void Node::Translate(Vector3 distance, Matrix meshTM)
	{
		Node* curNode = m_parentNode;
		Matrix totalMat = {};

		// 분명 조에서 애니메이션 POS 는 월드TM 기준이었는데
		// 돼지는 로컬 TM이다. 이게 함정이 아니었다니
// 		while (curNode != nullptr)
// 		{
// 			totalMat *= curNode->m_localTransform;
// 			curNode = curNode->m_parentNode;
// 		}

		// 부모 행렬 구해서 NodeTM만들기
		m_localTransform *= totalMat;

		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;

		m_localTransform.Decompose(scale, rotation, translation);

		// POS값 바꾸고
		translation = distance;

		DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
		DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(translation);

		// 분해 한것 다시 합치기
		Matrix transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

		m_localTransform = transformMatrix * totalMat.Invert();

		SetNode(totalMat, meshTM);
	}

	void Node::Rotate(Quaternion nextRotation)
	{
		Node* curNode = m_parentNode;
		Matrix totalMat = {};

		while (curNode != nullptr)
		{
			totalMat *= curNode->m_localTransform;
			curNode = curNode->m_parentNode;
		}

		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;

		m_localTransform.Decompose(scale, rotation, translation);

		// Rot값 바꾸고
		rotation = nextRotation;

		if (isNegativeScaleNotRoot || isNegativeScaleRoot) // IK_Joe 에서는 Biped-L_legup, Biped-R_foot01
		{
			scale = { -1.0f, -1.0f, -1.0f };
		}

		Matrix scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);
		Matrix rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
		Matrix translationMatrix = DirectX::XMMatrixTranslationFromVector(translation);

		// 분해 한것 다시 합치기
		Matrix transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

		m_localTransform = transformMatrix;

		SetNode(totalMat, /*temp*/totalMat);
	}

	void Node::Scale(Vector3 value)
	{

	}
}