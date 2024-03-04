/// <summary>
/// CParsingDataClass.h
/// 
/// Ver 1.0
/// ASE������ �Ľ������� ������ �����͵��� ����ü�� �������.
/// 
/// �ϴ� �� �� �о���̴� ���̰�, �������� ���������� ������ �Ѵ�.
/// D3DX���ξ, XM���ξ �������� ���ڴ�.
/// 
/// (���� �ּ�)
/// �˴� struct�� public���� �ߴµ� ������ �ƹ�����
/// ���� ���⿡�� ���� �����ؾ��ϱ� ����.
/// 
/// ��� �� �ڷᱸ���� �Ѱܹ޴� D3D�� ������������
/// ĸ��ȭ�� �Ǿ��ִ�. ��� ��ȯ �Լ����� �������� �� ���� �ִ�.
/// (�׷��� �ʴٸ� ������ GetSet�޼ҵ���� ������ �Ѵ�)
/// (���� �ּ�)
/// 
/// 2021.04.11 LeHideOffice
/// </summary>

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
//#include <DirectXMath.h>
// simpleMath �� ���� ���丮 ������ �� �Ǿ��ִ� �� ����.
//#include <SimpleMath.h>
#include "../ImpLib/Inc/SimpleMath.h"
//#include "../ImpStructure/ImpMath.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;
//using namespace ImpMath;


namespace ASEParser
{
	struct IndexList
	{
		int index[3];
	};

	///--------------------------------------------------
	/// �� ���� Vertex
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///--------------------------------------------------
	struct Vertex
	{
		Vertex();

		// ��ǥ���� ��ġ��
		DirectX::SimpleMath::Vector3 m_pos;

		// �븻��
		DirectX::SimpleMath::Vector3 m_normal;
		DirectX::SimpleMath::Vector3 m_tangent; // normalMap
		DirectX::SimpleMath::Vector3 m_bitangent; // normalMap

		// �븻���� ���õƴ°�?
		bool m_isnormalset;

		// �ؽ��� ��ǥ
		float u, v;

		// �ؽ��İ��� ���õƴ°�?
		bool m_istextureset;

		/// Skinned Mesh ���� �߰�
		// weight 3��
		float m_bw1 = 0.0f, m_bw2 = 0.0f, m_bw3 = 0.0f;

		// �ε���
		int m_indices = 0;
		int m_weightIndices[4];

		int* m_indicesPointer;
	};


	///--------------------------------------------------
	/// �� ���� Texture - Vertex
	///
	/// �̰��� Raw�������̸� ����ȭ�� �̷����� �ᱹ��
	/// COneVertex�� ���� �ȴ�.
	///--------------------------------------------------
	struct COneTVertex
	{
		COneTVertex();

		float m_u, m_v, m_w;
	};


	///--------------------------------------------------
	/// �� ���� Face
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///
	/// �Ľ� �ʱ⿡ Face�� ������ ������ ������
	/// �ߺ��Ǵ� VertexNormal��, �ؽ��� �ڵ����Ʈ ����
	/// ���� �� �ִ�. �װ��� ������� �Ѵ�.
	///--------------------------------------------------
	struct Face
	{
		// �� Face�� �̷�� ���ؽ��� �ε���
		int				m_vertexindex[3];
		// Face�� Normal��
		DirectX::SimpleMath::Vector3			m_normal;
		DirectX::SimpleMath::Vector3			m_tangent;
		DirectX::SimpleMath::Vector3			m_bitangent;
		// Vertex�� Normal��
		DirectX::SimpleMath::Vector3			m_normalvertex[3];
		// Texture Coordinate
		int				m_TFace[3];

		// �ʱ�ȭ�� ���� ������.
		Face();
	};


	///--------------------------------------------------
	/// �� ���� Shape Vertex�� ������ �����ϱ� ���� ����ü
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///--------------------------------------------------
	struct ShapeVertex
	{
		int				m_indexnum;			// �ε��� �ѹ�. �׳� �Ϸù�ȣ��
		bool				m_isknot;			// KNOT�ΰ� INTERP�ΰ�?
		DirectX::SimpleMath::Vector3			m_pos;				// ������ ������
	};


	///--------------------------------------------------
	/// �� ���� Shape Object�� ������ �����ϱ� ���� ����ü
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///--------------------------------------------------
	struct ShapeLine
	{
		int	m_line_number;		// ���° �����ΰ�?
		bool m_shape_closed;		// ���� Shape�ΰ�?
		int	m_shape_vertexcount;// �� ���� Vertex�� �ִ°�?
		vector<ShapeVertex*> m_shapevertex;	// ���ؽ����� vector

		// �ʱ�ȭ�� ���� ������
		ShapeLine();
	};


	/// <summary>
	/// �� ���� Scene�� �����ϱ� ���� ����ü
	/// ASE���Ͽ��� ���, �ڸ�Ʈ ���� ������ SCENE�κ��� �б� ���� ����ü.
	/// </summary>
	struct Scenedata
	{
	public:
		std::string		m_filename;					// ���� �̸�
		int				m_firstframe;
		int				m_lastframe;
		int				m_framespeed;
		int				m_ticksperframe;
		int				m_meshframestep;
		int				m_keyframestep;
		DirectX::SimpleMath::Vector3			m_scene_background_static;
		DirectX::SimpleMath::Vector3			m_scene_ambient_static;

	public:
		Scenedata();
		~Scenedata();
	};


	///--------------------------------------------------
	/// �� ���� Map�� �����ϱ� ���� Ŭ����
	///--------------------------------------------------
	struct MaterialMap
	{
		std::string		m_map_name;
		std::string		m_map_class;
		int		m_subno;
		float		m_map_amount;
		std::string		m_bitmap;
		int		m_map_type;
		float		m_uvw_u_offset;
		float		m_uvw_v_offset;
		float		m_u_tiling;
		float		m_v_tiling;
		float		m_uvw_angle;
		float		m_uvw_blur;
		float		m_uvw_blur_offset;
		float		m_uvw_noise_amt;
		float		m_uvw_noise_size;
		float		m_uvw_noise_level;
		float		m_uvw_noise_phase;
		int		m_bitmap_filter;

		//
		MaterialMap();
	};


	///--------------------------------------------------
	/// �� ���� Material�� �����ϱ� ���� Ŭ����
	///--------------------------------------------------
	struct ASEMaterial
	{
		//--------------------
		//	��� ������
		//--------------------
	public:
		// ������ *MATERIAL_COUNT 0 �ϳ��ۿ� ������
		int					m_materialnumber;			// Material�� ��ȣ. �̰��� �������� ������Ʈ���� �����Ѵ�.
		std::string					m_material_name;
		std::string					m_material_class;
		DirectX::SimpleMath::Vector3				m_material_ambient;
		DirectX::SimpleMath::Vector3				m_material_diffuse;
		DirectX::SimpleMath::Vector3				m_material_specular;
		float					m_material_shine;
		float					m_material_shinestrength;
		float					m_material_transparency;
		float					m_material_wiresize;
		int					m_material_shading;
		float					m_material_xp_falloff;
		float					m_material_selfillum;
		float					m_material_falloff;
		int					m_material_xp_type;

		int					m_numsubmtls;
		ASEMaterial* m_submaterial;				// �����Ʈ������ �����͸� ������ ����

		MaterialMap* m_map_temp;		// �ӽ÷� ���̴� ����
		MaterialMap* m_map_diffuse;	/// �ϴ� ������ map_diffuse�ۿ� ����
		MaterialMap* m_map_generic;
		//CMaterial_Map*			m_map_diffuse;
		//CMaterial_Map*			m_map_diffuse;

		bool					m_istextureexist;	// �ؽ��İ� �����ϴ°�?

		//--------------------
		//	Method
		//--------------------
	public:
		// ������ �Ҹ���
		ASEMaterial();
		~ASEMaterial();

	};


	// Light�� Animation ( Setting�� ��ȭ�� Animation )
	struct Light_AnimationData
	{
		// Light Setting
		float				m_timevalue;		// ���� mesh�� �ִµ�.. �̰� ��� ���� �̷����̸�.
		DirectX::SimpleMath::Vector3			m_light_color;
		float				m_light_intens;
		float				m_light_aspect;
		float				m_light_hotspot;		///
		float				m_light_falloff;		/// �� �ΰ����� SpotLight�� ���
		float				m_light_tdist;
		float				m_light_mapbias;
		float				m_light_maprange;
		int				m_light_mapsize;
		float				m_light_raybias;

	public:
		Light_AnimationData();

	};

	// Bone �ϳ��� ����ü
	// Mesh�� Vector�� �ִ�.
	struct Bone
	{
		~Bone()
		{
			delete m_boneTM_NodeTM;
			delete m_boneTM_WorldTM;
		}
		int		m_bone_number;		// ������ ���ͷ� ���������� ���߿� ������ �ʿ�, �˻��� �ʿ�?
		std::string		m_bone_name;
		// ��¼�� ���⿡�� ��Ʈ������ �־�� �ϴ����� ����. ������ ��ư ���ܵ���

		// �̳༮�� ó���� NodeTM�̴� - offset�� ���ϱ� ���ؼ� �����Ѵ�.
		DirectX::SimpleMath::Matrix* m_boneTM_NodeTM;
		// �̳༮�� �ִϸ��̼��� �����, ������ WorldTM�̴�
		DirectX::SimpleMath::Matrix* m_boneTM_WorldTM;
	};


	// bone�ϳ��� ����� ����ġ �� ��
	struct Weight
	{
		int		m_bone_number;		// weight�� ����� Bone�� IndexNumber;
		float		m_bone_weight;		// ����ġ
	};


	// Vertex �ϳ��� �־��� ����ġ���� �����ϴ� ����ü (����ü �ϳ��� ���ؽ� �ϳ��� �ϴ��� �����̴�)
	struct VertexWeight
	{
		~VertexWeight()
		{
// 			delete m_temp_bone_blend_weight;
// 			for (UINT i = 0; i < m_bone_blending_weight.size(); i++)
// 			{
// 				delete m_bone_blending_weight[i];
// 			}
		}
		int		m_wvertex_number;	// ���ؽ��� ��ȣ. ���� ���ͷ� �����ϹǷ� �� �� ������...

		// ���������� �Ʒ� ����Ʈ�� �ӽ� ����
		Weight* m_temp_bone_blend_weight;
		// ���ؽ� �ϳ��� ���� ���� Bone-Weight�� ���� �� �ִ�.
		vector<Weight*>	m_bone_blending_weight;	// bone�ϳ��� ����� ����ġ
	};



	///--------------------------------------------------
	/// �� ���� �޽ø� �Ľ� - �����ϱ� ���� Ŭ����
	/// ���� ���� �޽ð� ���� �� �ֱ� ������ ���� �� �κи� ����ü�� �������.
	///--------------------------------------------------
	struct Mesh
	{
	public:
		// temp
		~Mesh()
		{
			for (const auto& v : m_meshvertex)
			{
				delete v;
			}
			for (const auto& i : m_meshface)
			{
				delete i;
			}
			for (const auto& i : m_opt_vertex)
			{
				delete i;
			}
			for (const auto& i : m_mesh_tvertex)
			{
				delete i;
			}
			delete[] m_opt_index;
		}
		///----------------------------------
		/// Ÿ��, �ִϸ��̼��� �ִ°�? ���
		///----------------------------------
		int	m_type;					// Ÿ��
		bool m_isAnimated;			// �ִϸ��̼��� ������ �ִ°�? (������Ʈ�� ���� �ٸ��� �ؼ��� �� ����)
		Scenedata m_scenedata;		// (3DS Max��) �� ������

		///----------------------------------
		/// *NODE_NAME
		///----------------------------------
		std::string	m_nodename;		// � �������� ������ �Ʒ��� m_meshname�� ���� �̸��� �ѹ� �� ����. �Ƹ��� �޽��� �̸� ����Ʈ�����... �� �׷��� �ƴұ� ��ư ������ ���� 1�̹Ƿ� �������� ������ ����.
		std::string	m_nodeparent;	// �θ� ����� �̸�. �̰��� �ִٸ� �θ� �ִ� ���̴�.
		bool m_isparentexist;		// �׷��� Ȯ���� bool������ ������ ����.

		///----------------------------------
		/// *NODE_TM (Transform Matrix)
		///----------------------------------
		DirectX::SimpleMath::Vector3	m_inherit_pos;
		DirectX::SimpleMath::Vector3	m_inherit_rot;
		DirectX::SimpleMath::Vector3	m_inherit_scl;
		DirectX::SimpleMath::Vector3	m_tm_row0;
		DirectX::SimpleMath::Vector3	m_tm_row1;
		DirectX::SimpleMath::Vector3	m_tm_row2;
		DirectX::SimpleMath::Vector3	m_tm_row3;
		DirectX::SimpleMath::Vector3	m_tm_pos;
		DirectX::SimpleMath::Vector3	m_tm_rotaxis;
		float m_tm_rotangle;
		DirectX::SimpleMath::Vector3	m_tm_scale;
		DirectX::SimpleMath::Vector3	m_tm_scaleaxis;
		float m_tm_scaleaxisang;

		///----------------------------------
		/// *MESH
		///----------------------------------
		float m_timevalue;			// ���� ������ ���������� ���δ�. ��ư �ð���.
		int	m_numbone;				/// ���� ����. �̰��� �ִٸ� ��Ű�� ������Ʈ�� �����Ѵ�.
		int	m_numskinweight;		/// ��Ų ����ġ. ���Ҹ���?
		int	m_mesh_numvertex;		// (���ؽ��� ��ǥ������)
		int	m_mesh_numfaces;		// (���ؽ��� �ε���)

		// �޽� ���� vertex �迭�� �����ϱ� ���� ������
		/// *MESH_VERTEX_LIST
		vector<Vertex*>	m_meshvertex;	// ���ؽ�
		/// *MESH_FACE_LIST
		vector<Face*> m_meshface;		// �ε���

		// �ؽ��Ŀ� �����͵�
		/// *MESH_NUMTVERTEX
		int	m_mesh_numtvertex;		// �ؽ��Ŀ� ���ؽ� ����
		vector<COneTVertex*> m_mesh_tvertex;		// �ؽ��Ŀ� ���ؽ�
		/// *MESH_RVERTLIST
		int	m_mesh_tvfaces;			// �ؽ��Ŀ� ���̽� ���� - (������ ���� Face�� ���� �̹� ������ ��������Ƿ� ���븸 �־��ָ� �ȴ�)
		int	m_mesh_numcvertex;		// �� �̰� ��� ���°�?

		// �븻 ������
		DirectX::SimpleMath::Vector3* m_mesh_facenormal;			// �� ���̽� �븻��
		DirectX::SimpleMath::Vector3* m_mesh_vertexnormal;		// ���ؽ� �븻�� 3����

		vector<Vertex*> m_opt_vertex;	/// ����ȭ ���� ���ؽ���
		IndexList* m_opt_index;			/// ����ȭ ���� �ε�����

		///----------------------------------
		/// Shape Object
		///----------------------------------
		int				m_shape_linecount;			// ������ ��
		vector<ShapeLine*>	m_vector_shape_line;	// ������ ������ ������


		/// ������ ������
		int				m_prop_motionblur;			//*PROP_MOTIONBLUR 0
		int				m_prop_castshadow;			//*PROP_CASTSHADOW 1
		int				m_prop_recvshadow;			//*PROP_RECVSHADOW 1
		int				m_material_ref;				//*MATERIAL_REF			// �� �޽��� ��Ʈ������ ��ȣ
		DirectX::SimpleMath::Vector3			m_wireframe_color;			//*WIREFRAME_COLOR 0.89803922	0.65098041	0.84313726


		///----------------------------------
		/// Camera
		///----------------------------------
		// �̷��� ������ �ѹ��� �о������ ���� ���α� �ѵ�..
		// �ϴ� �̷��� �����������ϱ�. �׸��� �ļ��� ��ͱ����� ����� �Ǹ� �̷��� �ϴ°� ���ϴ�
		// ��� D3D�� Ŭ������ ������ �� ��ȯ�� �Ǵϱ�
		std::string			m_camera_type;
		// CAMERA_SETTINGS
		//float				m_timevalue;		// mesh�� �̹� �ִ�.
		float				m_camera_near;
		float				m_camera_far;
		float				m_camera_fov;
		float				m_camera_tdist;
		bool				m_camera_isloadTarget;	// ī�޶�� NodeTM�� �ΰ��� �ִµ�, ��ͱ��������� ������ ����� 
													// (�׻� ������ Target�� �о���� �� �ִ� �׷��� �װ��� ���ذ��� �Ѵ�)

		///----------------------------------
		/// Light
		///----------------------------------
		std::string				m_light_type;
		// Omni�ΰ� SpotLight�ΰ��� ó���� �� ��� �Ѵ�.

		// Light Property
		std::string				m_light_shadows;
		bool				m_light_uselight;
		std::string				m_light_spotshape;
		bool				m_light_useglobal;
		bool				m_light_absmapbias;
		bool				m_light_overshoot;

		// Light Setting & Animation
		Light_AnimationData* m_lightanimation;		// Light Setting
		vector<Light_AnimationData*>	m_list_lightanimation;	// �װ��� Animation

		bool				m_light_isloadTarget;	// ���� NodeTM�� �� �� �ְ�, ī�޶�� ���� ������ �� ������ �ִ�.
													/// ��(�ƹ����� ���� ���� ������. �ٲ���.)
		bool				m_light_isAnimate;		// Animation�� �Ǵ°�?

		///----------------------------------
		/// Skinning Object
		///----------------------------------
		// SKIN_INITTM�� �����ϰ�
		// TM_ROW0~3
		// BONE_LIST
		//bool					m_is_skinningobject;	// ��Ű�� ������Ʈ�ΰ�?
		bool					m_is_bone;	// ��Ű�� ������Ʈ�ΰ�?

		/// Yujin �߰�
		std::unordered_multimap<unsigned int, unsigned int> m_savedVertexSplit;
		//std::vector<XMFLOAT3> m_faceTangents;

	public:
		Mesh();

	};

	class CAnimation_pos
	{
	public:
		int m_time;
		DirectX::SimpleMath::Vector3	m_pos;
	};

	class CAnimation_rot
	{
	public:
		int				m_time;
		DirectX::SimpleMath::Vector3		m_rot;
		float			m_angle;
		DirectX::SimpleMath::Quaternion	m_rotQT_accumulation;		// ��뺯���̱� ������ ���� ����� �����ؼ� ������ �ִ´� (�� ���ݱ����� ���뷮���� ��ȯ�Ѵ�)
	};

	class CAnimation_scl {

	public:
		int				m_time;
		DirectX::SimpleMath::Vector3		m_scale;
		DirectX::SimpleMath::Vector3		m_scaleaxis;
		float			m_scaleaxisang;
		DirectX::SimpleMath::Quaternion	m_scalerotQT_accumulation;
	};


	///--------------------------------------------------
	/// �� Ŭ������ ������ Animation Ŭ����
	///--------------------------------------------------
	class Animation {

	public:
		//temp
		~Animation()
		{
			for (const auto& pos : m_position)
			{
				delete pos;
			}
			for (const auto& rot : m_rotation)
			{
				delete rot;
			}
			for (const auto& scl : m_scale)
			{
				delete scl;
			}
		}
		// �ִϸ��̼� �������� �̸�. ������ ���������̴�
		std::string						m_nodename;
		vector<CAnimation_pos*>		m_position;
		vector<CAnimation_rot*>		m_rotation;
		vector<CAnimation_scl*>		m_scale;

		int		m_ticksperFrame;	// �� �������� ƽ ũ��. �̰��� AnimationTable�� ������ ������ ������Ʈ���� �ٸ� �� �ִ�. // 1������ �� ƽ ��
		int		m_minKeyframe;		// Ű �����Ӱ��� �ּ�ũ��. �̰��� ���ǰͰ� �� ��ġ������ �ʴ´�.
		
		unsigned int		m_firstFrame;		// Ű �����Ӱ��� �ּ�ũ��. �̰��� ���ǰͰ� �� ��ġ������ �ʴ´�.
		unsigned int		m_lastFrame;		// Ű �����Ӱ��� �ּ�ũ��. �̰��� ���ǰͰ� �� ��ġ������ �ʴ´�.
	};

	/// <summary>
	/// �޽� + Ʈ������ ����̸� �ɱ�?
	/// </summary>
	struct Node
	{
		Node(Mesh* mesh = nullptr) : m_mesh(mesh), m_childNode({}), m_parentNode(nullptr) {};
		// temp
		~Node()
		{
			delete m_animation;
			//delete m_bone;
		}

		std::string m_name;
		Mesh* m_mesh;

		DirectX::SimpleMath::Matrix m_localTransform;
		DirectX::SimpleMath::Matrix m_worldTransform;

		Node* m_parentNode;
		std::vector<Node*> m_childNode;

		Animation* m_animation = nullptr;
		Bone* m_bone = nullptr;

		// �ִϸ��̼��� ���� �ʿ��ϴ�. ��Ȯ���� ��Ծ��� ���� ���ؼ�
		bool isNegativeScaleRoot = false;
		bool isNegativeScaleNotRoot = false;
		bool isNegativeScale = false;

		void Transform(DirectX::SimpleMath::Vector3 distance, float angle, DirectX::SimpleMath::Vector3 value);
		void SetNode(DirectX::SimpleMath::Matrix parentMat, DirectX::SimpleMath::Matrix meshTM);

		void Translate(DirectX::SimpleMath::Vector3 distance, DirectX::SimpleMath::Matrix meshTM);
		void Rotate(DirectX::SimpleMath::Quaternion nextRotation);
		void Scale(DirectX::SimpleMath::Vector3 value);
	};

	/// <summary>
	/// ���� ���� �޽ø� ������ �ִ� ������Ʈ�� �����? Ʈ�� ����?
	/// ��Ʈ�� ����Ű�� ������?
	/// �ϴ� �ε����� NODE_NAME�̴�.
	/// ���� �̰� �� ���� ������� ����? map���� �ϸ� �ȴ�.
	/// </summary>
	struct Object
	{
		//temp
		~Object()
		{
			for (const auto& node : object)
			{
				delete node.second;
			}
		}
		std::unordered_map<std::string, Node*> object;
	};
}