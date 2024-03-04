#include "Parser.h"
#include "../ImpStructure/ImpMesh.h"
using namespace ImpStructure;

#include "Common.h"
#include "Geometry.h"

FBXLoad::Parser::Parser(const char* fileName)
	: _scene{ new Scene()}
{
	assert(fileName && _scene);
	FBXSDK_printf("\n>> ���� �̸�: \"%s\" \n", fileName);

	if (InitilazeFbx())
	{
		LoadFbxFile(fileName);
		assert(IsValid());
	}
}

FBXLoad::Parser::~Parser()
{
	_fbxScene->Destroy();
	if (_fbxManager) _fbxManager->Destroy();
}

constexpr bool FBXLoad::Parser::IsValid() const
{
	return _fbxManager && _fbxScene;
}

bool FBXLoad::Parser::InitilazeFbx()
{
	FBXSDK_printf("Paser::LoadFbxFile(const char*) ȣ����...\n");
	assert(!IsValid());

	// FBX SDK �Ŵ��� �ʱ�ȭ
	_fbxManager = FbxManager::Create();

	if (!_fbxManager)
	{
		FBXSDK_printf("����: ��FbxManager���� ���� �� ����.\n\n");
		exit(1);

		return false;
	}
	else FBXSDK_printf("\n>> Autodesk FBX SDK ����: \"%s\"\n", _fbxManager->GetVersion());

	// IO ���� ��ü ���� �� �ʱ�ȭ
	FbxIOSettings* ios{ FbxIOSettings::Create(_fbxManager, IOSROOT) };
	_fbxManager->SetIOSettings(ios);
	
	return true;
}

void FBXLoad::Parser::LoadFbxFile(const char* file)
{
	// 
	// 1. importer ����
	// 2. importer �� scene ����
	// 3. scene ������ �б�
	// 4. importer ����
	//

	/// importer ����

	// FBX SDK �Ŵ����� importer ����
	FbxImporter* fbxImporter{ FbxImporter::Create(_fbxManager, "Importer") };

	// �ش� ������ ������ ������ ���� ���� �߻�
	if (!fbxImporter->Initialize(file, -1, _fbxManager->GetIOSettings()))
	{
		FBXSDK_printf("FbxImporter::Initialize() �Լ� ȣ�� ����.\n\n");
		FBXSDK_printf("���� ����: %s\n\n", fbxImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	/// �� ������ �б�

	// ���� ����
	_fbxScene = FbxScene::Create(_fbxManager, "Importer Scene");
	_scene->name = _fbxScene->GetName();
	FBXSDK_printf("\n>> Scene name: \"%s\"\n\n", _scene->name);

	if (!_fbxScene)
	{
		FBXSDK_printf("����: ��FbxScene���� ���� �� ����!\n\n");
		exit(1);
	}

	// importer ���� �� ����
	if (!fbxImporter->Import(_fbxScene))
	{
		printf("FbxImporter::Import() �Լ� ȣ�� ����.\n\n");
		printf("���� ����: %s\n\n", fbxImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	/// importer ����

	// ���� �ҷ������Ƿ� importer ����
	fbxImporter->Destroy();
}

void FBXLoad::Parser::GetScene(FbxNode* root /*= nullptr*/)
{
	FBXSDK_printf("\n> Paser::GetScene(FbxNode*) ȣ����...\n");
	assert(IsValid());

	// ��Ʈ�� ���ٸ� �� ��Ʈ ��� ����
	if (!root)
	{
		//FBXSDK_printf("FbxScene::GetRootNode() ȣ����...\n");
		root = _fbxScene->GetRootNode();

		if (!root)
		{
			FBXSDK_printf("����: ������ ��Ʈ ���� nullptr �Դϴ�.\n\n");
			return;
		}
	}

	// �ڽ� ��� ��������
	const int numNode{ root->GetChildCount() };
	FBXSDK_printf("\n>> Node Child count: %d\n", numNode);
	for (int i{ 0 }; i < numNode; i++)
	{
		FBXSDK_printf("\n>> Node Child number: %d\n", i);
		FbxNode* node{ root->GetChild(i) };

		// ��� ȣ�� Ż�⹮
		if (!node) continue; 

		/// ��� �Ӽ� �� ������ ��������
		GetNode(node);

		// ��� ȣ��. �� ���� ���� �ܰ�� ������
		GetScene(node);
	}
}

void FBXLoad::Parser::GetNode(FbxNode* node)
{
	/*
	* �����غ��� ���� Mesh ���� ������ ���� �� ����
	* 2023-12-21
	*/

	FBXSDK_printf("\n> Paser::GetNode(FbxNode*) ȣ����...\n");
	FbxNodeAttribute* nodeAttribute{ node->GetNodeAttribute() };
	switch (nodeAttribute->GetAttributeType())
	{
	case FbxNodeAttribute::eUnknown:
		FBXSDK_printf("\n>>FbxNodeAttribute::eUnknown\n");
		break;
	case FbxNodeAttribute::eNull:
		FBXSDK_printf("\n>>FbxNodeAttribute::eNull\n");
		break;
	case FbxNodeAttribute::eMarker:
		FBXSDK_printf("\n>>FbxNodeAttribute::eMarker\n");
		break;
	case FbxNodeAttribute::eSkeleton:
		FBXSDK_printf("\n>>FbxNodeAttribute::eSkeleton\n");
		break;
	case FbxNodeAttribute::eMesh:
		FBXSDK_printf("\n>>FbxNodeAttribute::eMesh\n");
		GetMesh(node, _scene->meshes);
		break;
	case FbxNodeAttribute::eNurbs:
		FBXSDK_printf("\n>>FbxNodeAttribute::eNurbs\n");
		break;
	case FbxNodeAttribute::ePatch:
		FBXSDK_printf("\n>>FbxNodeAttribute::ePatch\n");
		break;
	case FbxNodeAttribute::eCamera:
		FBXSDK_printf("\n>>FbxNodeAttribute::eCamera\n");
		break;
	case FbxNodeAttribute::eCameraStereo:
		FBXSDK_printf("\n>>FbxNodeAttribute::eCameraStereo\n");
		break;
	case FbxNodeAttribute::eCameraSwitcher:
		FBXSDK_printf("\n>>FbxNodeAttribute::eCameraSwitcher\n");
		break;
	case FbxNodeAttribute::eLight:
		FBXSDK_printf("\n>>FbxNodeAttribute::eLight\n");
		break;
	case FbxNodeAttribute::eOpticalReference:
		FBXSDK_printf("\n>>FbxNodeAttribute::eOpticalReference\n");
		break;
	case FbxNodeAttribute::eOpticalMarker:
		FBXSDK_printf("\n>>FbxNodeAttribute::eOpticalMarker\n");
		break;
	case FbxNodeAttribute::eNurbsCurve:
		FBXSDK_printf("\n>>FbxNodeAttribute::eNurbsCurve\n");
		break;
	case FbxNodeAttribute::eTrimNurbsSurface:
		FBXSDK_printf("FbxNodeAttribute::eTrimNurbsSurface\n");
		break;
	case FbxNodeAttribute::eBoundary:
		FBXSDK_printf("\n>>FbxNodeAttribute::eBoundary\n");
		break;
	case FbxNodeAttribute::eNurbsSurface:
		FBXSDK_printf("\n>>FbxNodeAttribute::eNurbsSurface\n");
		break;
	case FbxNodeAttribute::eShape:
		FBXSDK_printf("\n>>FbxNodeAttribute::eShape\n");
		break;
	case FbxNodeAttribute::eLODGroup:
		FBXSDK_printf("\n>>FbxNodeAttribute::eLODGroup\n");
		break;
	case FbxNodeAttribute::eSubDiv:
		FBXSDK_printf("\n>>FbxNodeAttribute::eSubDiv\n");
		break;
	case FbxNodeAttribute::eCachedEffect:
		FBXSDK_printf("\n>>FbxNodeAttribute::eCachedEffect\n");
		break;
	case FbxNodeAttribute::eLine:
		FBXSDK_printf("\n>>FbxNodeAttribute::eLine\n");
		break;
	default:
		break;
	}
}

void FBXLoad::Parser::GetMesh(FbxNode* node, std::vector<Mesh>& meshes)
{
	FBXSDK_printf("Paser::GetMesh(FbxNode*, std::vector<Mesh>&) ȣ����...\n");
	assert(node);

	if (FbxMesh * fbxMesh{ node->GetMesh() }) // ���� �޽� **
	{
		if (fbxMesh->RemoveBadPolygons() < 0) return;

		// ��� ��带 ������ ��� �ﰢ�� ���������� �޽÷� ����� ��
		FbxGeometryConverter geoConvertor{ _fbxManager };
		fbxMesh = static_cast<FbxMesh*>(geoConvertor.Triangulate(fbxMesh, true));
		if (!fbxMesh || fbxMesh->RemoveBadPolygons() < 0) return;

		// �� �޽� ����
		Mesh myMesh;
		myMesh.id = (int)meshes.size();
		myMesh.name = (node->GetName()[0] != '\0') ? node->GetName() : fbxMesh->GetName();

		/*** �޽� ������ �Ľ� �޼��� ȣ�� ***/
		FBXSDK_printf("Parser::GetMeshData() ȣ����...\n");
		if (GetMeshData(fbxMesh, myMesh))
		{
			meshes.emplace_back(myMesh);
		}

	}
}

bool FBXLoad::Parser::GetMeshData(FbxMesh* fbxMesh, Mesh& myMesh)
{
	assert(fbxMesh);
	FBXSDK_printf("\n=================================\n\"%s\"\n--------------------------------- ", myMesh.name);

	/// *** ���͸��� *** ///
	if(!GetMaterial(fbxMesh, myMesh)) return false;

	/// *** ������ �Ľ� *** ///
	if(!GetControlPoints(fbxMesh, myMesh)) return false;

	/// *** �븻 *** ///
	GetVertexNormals(fbxMesh, myMesh);

	/// *** ź��Ʈ *** ///
	GetTangents(fbxMesh, myMesh);

	/// *** UV *** ///
	GetUVSet(fbxMesh, myMesh);

	FBXSDK_printf("---------------------------------\n");

	// ��
	return true;
}

bool FBXLoad::Parser::GetControlPoints(FbxMesh* fbxMesh, Mesh& myMesh)
{
	// ���� ��
	const int numVertices{ fbxMesh->GetControlPointsCount() };
	FbxVector4* vertices{ fbxMesh->GetControlPoints() }; 
	FBXSDK_printf("\n# Vertex count: %d\n", numVertices);

	// �ε���
	const int numIndices{ fbxMesh->GetPolygonVertexCount() };
	int* indeces{ fbxMesh->GetPolygonVertices() };
	myMesh.indices.resize(numIndices);
	FBXSDK_printf("\n# Indices count: %d\n", numIndices);

	assert(numVertices > 0 && vertices && numIndices > 0 && indeces);
	if(!(numVertices > 0 && vertices && numIndices > 0 && indeces)) return false;

	const int invalidId = -666;
	std::vector<int> refVertex(numVertices, invalidId); // ������ ���� �ε��� 

	// 
	// �Ϲ�������
	// (������ �ε��� ��) > (������ ��)
	// 
	// �ε����� ����(������ ���� �ߺ��Ǵ�) �������� �Ľ��� 
	// �����ϴ� �۾��� �Žô�.
	// 

	for (int i{ 0 }; i < numIndices; i++) // ���� �ε��� �� ��ŭ ��ȸ
	{
		const int idxVertex{ indeces[i] }; // ���� �ε���

		FbxVector4 vertex = vertices[idxVertex];
		Vertex newVertex;
		newVertex.position = vertex;

		myMesh.indices[i] = i;
		myMesh.vertices.push_back(newVertex);

		// �̹� ������ ������ ��?
		if (refVertex[idxVertex] != invalidId)
		{
			// ������ �ε��� ������Ʈ *** 
			myMesh.indices[i] = refVertex[idxVertex];
		}
		// ������ ������ ���� ����?
		else
		{
			refVertex[idxVertex] = myMesh.indices[i]; // ������ ���� ������Ʈ
		}

		FBXSDK_printf("## Vertex Position %-8d, index %-4d: ( %f, %f, %f )\n", i, myMesh.indices[i], vertex[0], vertex[1], vertex[2]);
	}

	// �ﰢ�� ������ �޽ø� �Ľ��ϱ� ������ �ε��� ����� 3�� �ƴϸ� �ȵȴ�. 
	assert(myMesh.indices.size() % 3 == 0);

	return true;
}

bool FBXLoad::Parser::GetMaterial(FbxMesh* fbxMesh, Mesh& myMesh)
{
	// ������ ��
	const int numPolygon{ fbxMesh->GetPolygonCount() };
	if (numPolygon <= 0) return false;
	FBXSDK_printf("\n# Polygin count: %d\n", numPolygon);

	assert(numPolygon > 0);

	//
	// �����︶�� �ߺ��Ǵ� ���͸����� �����Ƿ� 
	// �ߺ��� �����ϴ� �۾��� �Ѵ�.
	//

	FbxLayerElementArrayTemplate<int>* mtlsIndices;
	if (fbxMesh->GetMaterialIndices(&mtlsIndices))
	{
		for (int i{ 0 }; i < numPolygon; i++)
		{
			const int mtlIdex{ mtlsIndices->GetAt(i) };
			assert(mtlIdex >= 0);
			myMesh.materialIndices.emplace_back(mtlIdex);

			// materialUsed �ȿ� mtlIdex �� ������( `std::find`�� ���� �� ã���� ������ ���� ��ȯ�Ѵ�.) ���ο� �ε��� �߰�
			if (std::find(myMesh.materialUsed.begin(), myMesh.materialUsed.end(), mtlIdex) == myMesh.materialUsed.end())
			{
				myMesh.materialUsed.emplace_back(mtlIdex); // materialUsed �ȿ� �߰�
				FBXSDK_printf("\n## Material Index: %d", mtlIdex);
			}
		}

		FBXSDK_printf("\n");
	}

	return true;
}

void FBXLoad::Parser::GetVertexNormals(FbxMesh* fbxMesh, Mesh& myMesh)
{
	FbxArray<FbxVector4> normals;
	FbxArray<FbxVector4> binormal;
	if (fbxMesh->GenerateNormals() &&
		fbxMesh->GetPolygonVertexNormals(normals) && normals.Size() > 0)
	{
		int numNormals{ normals.Size() };
		FBXSDK_printf("\n# Normals count: %d\n", numNormals);
		for (int i{ 0 }; i < numNormals; i++)
		{
			myMesh.vertices[i].normal = normals[i];
			//FBXSDK_printf("## Normals: ( %f, %f, %f )\n", normals[i][0], normals[i][1], normals[i][2]);
		}
	}
}

void FBXLoad::Parser::GetTangents(FbxMesh* fbxMesh, Mesh& myMesh)
{
	FbxLayerElementArrayTemplate<FbxVector4>* tangents{ nullptr };
	if (fbxMesh->GenerateTangentsData() &&
		fbxMesh->GetTangents(&tangents) &&
		tangents && tangents->GetCount() > 0)
	{
		const int numTangent{ tangents->GetCount() };
		FBXSDK_printf("# Tangent count: %d\n", numTangent);

		for (int i{ 0 }; i < numTangent; i++)
		{
			FbxVector4 tangent{ tangents->GetAt(i) };
			myMesh.vertices[i].tangent = tangent;
			FBXSDK_printf("## Tangent: ( %f, %f, %f, %f )\n", tangent[0], tangent[1], tangent[2], tangent[3]);
		}
	}
}

void FBXLoad::Parser::GetUVSet(FbxMesh* fbxMesh, Mesh& myMesh)
{
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);
	const int uvCount{ uvNames.GetCount() };
	FBXSDK_printf("\n# UV \"%s\" count: %d\n", uvNames.GetStringAt(0), uvCount);

	for (int i{ 0 }; i < uvCount; i++)
	{
		FbxArray<FbxVector2> uvs;
		if (fbxMesh->GetPolygonVertexUVs(uvNames.GetStringAt(i), uvs))
		{
			const int uvsNum{ uvs.Size() };
			FBXSDK_printf("## UV (%d) size: %d\n", i, uvsNum);

			for (int j{ 0 }; j < uvsNum; j++)
			{
				myMesh.vertices[i].uv = uvs[j];
				FBXSDK_printf("### UV: ( %f, %f )\n", uvs[j][0], uvs[j][1]);
			}
		}
	}
}