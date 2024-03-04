#include "Loader.h"

#include "../ImpStructure/ImpMesh.h"

#include <cassert>
#include <vector>

FBXLoad::Loader::Loader()
{
	InitilazeFbx();
}

FBXLoad::Loader::~Loader()
{
	_fbxScene->Destroy();
	if (_fbxManager) _fbxManager->Destroy();
}

constexpr bool FBXLoad::Loader::IsValid() const
{
	return _fbxManager && _fbxScene;
}

void FBXLoad::Loader::InitilazeFbx()
{
	FBXSDK_printf("Loader::InitilazeFbx(void) ȣ����...\n");
	assert(!IsValid());

	// FBX SDK �Ŵ��� �ʱ�ȭ
	_fbxManager = FbxManager::Create();

	if (!_fbxManager)
	{
		FBXSDK_printf("����: ��FbxManager���� ���� �� ����.\n\n");
		exit(1);

	}
	else FBXSDK_printf("\n>> Autodesk FBX SDK ����: \"%s\"\n", _fbxManager->GetVersion());

	// IO ���� ��ü ���� �� �ʱ�ȭ
	FbxIOSettings* ios{ FbxIOSettings::Create(_fbxManager, IOSROOT) };
	_fbxManager->SetIOSettings(ios);
}

void FBXLoad::Loader::LoadFbxFile(const char* file)
{
	assert(file);
	FBXSDK_printf("\n>> ���� �̸�: \"%s\" \n", file);

	// ���� ���ο� ������ ���� ��츦 ����ؼ�
	// ���͸� ���
	_meshes.clear();

	// importer ����
	FbxImporter* fbxImporter{ FbxImporter::Create(_fbxManager, "Importer") };

	// �ش� ������ ������ ������ ���� ���� �߻�
	if (!fbxImporter->Initialize(file, -1, _fbxManager->GetIOSettings()))
	{
		FBXSDK_printf("FbxImporter::Initialize() �Լ� ȣ�� ����.\n\n");
		FBXSDK_printf("���� ����: %s\n\n", fbxImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	// �� ����
	_fbxScene = FbxScene::Create(_fbxManager, "Importer Scene");
	FBXSDK_printf("\n>> Scene name: \"%s\"\n\n", _fbxScene->GetName());

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

	// importer ����
	fbxImporter->Destroy();

	assert(IsValid());

	/// Scene Load ***
	LoadScene();
}

void FBXLoad::Loader::LoadScene(FbxNode* root)
{
	assert(IsValid());

	// ��Ʈ�� ���ٸ� �� ��Ʈ ��� ����
	if (!root)
	{
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
		LoadMesh(node);

		// ��� ȣ��. �� ���� ���� �ܰ�� ������ 
		LoadScene(node); 
	}
}

void FBXLoad::Loader::LoadMesh(FbxNode* node)
{
	if (FbxMesh * fbxMesh{ node->GetMesh() }) // ���� �޽� **
	{
		if (fbxMesh->RemoveBadPolygons() < 0) return;

		// ��� ��带 ������ ��� �ﰢ�� ���������� �޽÷� ����� ��
		int* indeces{ fbxMesh->GetPolygonVertices() };		// �ε���

		const int numVertices{ fbxMesh->GetPolygonVertexCount() };
		const int numFace{ fbxMesh->GetPolygonCount() };
		FbxVector4* ctrlPoints{ fbxMesh->GetControlPoints() }; // ����

		FbxGeometryConverter geoConvertor{ _fbxManager };
		fbxMesh = static_cast<FbxMesh*>(geoConvertor.Triangulate(fbxMesh, true));
		if (!fbxMesh || fbxMesh->RemoveBadPolygons() < 0) return;

		// �� �޽� ����
		ImpMesh* myMesh = new ImpMesh();

		/*** �޽� ������ �Ľ� �޼��� ȣ�� ***/
		if (LoadMeshData(fbxMesh, *myMesh))
		{
			_meshes.emplace_back(myMesh);
		}
	}
}

bool FBXLoad::Loader::LoadMeshData(FbxMesh* fbxMesh, ImpMesh& myMesh)
{
	assert(fbxMesh);
	FBXSDK_printf("\n=================================\n\"%s\"\n--------------------------------- ", fbxMesh->GetName());

	FbxStringList uvNames;
	FbxArray<FbxVector4> normals;
	FbxArray<FbxVector2> uvs;
	FbxLayerElementArrayTemplate<FbxVector4>* tangents{ nullptr };
	FbxLayerElementArrayTemplate<FbxVector4>* binormals{ nullptr };
	FbxLayerElementArrayTemplate<int>* textureIndices{ nullptr };

	// Generate
	fbxMesh->GenerateNormals();
	fbxMesh->GenerateTangentsData();

	// ������ ���ܿ���
	fbxMesh->GetUVSetNames(uvNames);
	fbxMesh->GetPolygonVertexNormals(normals);
	fbxMesh->GetTangents(&tangents);
	fbxMesh->GetBinormals(&binormals);
	fbxMesh->GetTextureIndices(&textureIndices, fbxsdk::FbxLayerElement::EType::eTextureDiffuse);
	
	// ũ�� 
	const int numVertices{ fbxMesh->GetPolygonVertexCount() };
	const int numFace{ fbxMesh->GetPolygonCount() };
	const int numUv{ uvNames.GetCount() };
	if(normals) const int numNormal{ normals.Size() };
	if(tangents) const int numTan{ tangents->GetCount() };
	if(binormals) const int numBitan{ binormals->GetCount() };
	if(textureIndices) int lIndexCount = textureIndices->GetCount();

	FBXSDK_printf("\n# Vertex count: %d\n", numVertices);
	FBXSDK_printf("\n# GetPolygonCount : %d\n", fbxMesh->GetPolygonCount());
	FBXSDK_printf("\n# UV \"%s\" count: %d\n", uvNames.GetStringAt(0), numUv);

	// �ڷ� ����
	myMesh._hasTexture = (textureIndices) ? true : false;
	myMesh._hasTangent = (tangents && binormals) ? true : false;
	myMesh._hasNormal = (normals) ? true : false;

	// ���� ������
	int* indeces{ fbxMesh->GetPolygonVertices() };		// �ε���
	FbxVector4* ctrlPoints{ fbxMesh->GetControlPoints() }; // ����
	fbxMesh->GetPolygonVertexUVs(uvNames.GetStringAt(0), uvs);
	
	for (int i = 0; i < 12; i++)
	{
		int a = fbxMesh->GetPolygonVertex(i, 0);
		int b = fbxMesh->GetPolygonVertex(i, 2);
		int c = fbxMesh->GetPolygonVertex(i, 1);
		int d = 3;
	}

	//int a{ textureIndices->GetAt(0) };

	// ���� ��������
	myMesh._meshVertex.resize(numVertices);
	myMesh._meshFace.resize(numFace);

	for (int i{ 0 }; i < numFace; i++) // ���� ���̽� ��
	{
		myMesh._meshFace[i] = new ImpFace();

		for (int j{ 0 }; j < 3; j++) // ���� �ε��� �� ��ŭ
		{
			const int i3j{ 3 * i + j };
			myMesh._meshVertex[3 * i + j] = new ImpVertex();

			/// *** Index *** ///

			// *** vertex *** //
			const int idxVertex{ indeces[i3j] }; // ���� �ε���
			myMesh._meshFace[i]->vertexIndex[j] = idxVertex;

			// *** texture *** // 
			if (myMesh._hasTexture)
			{
				myMesh._meshFace[i]->textureIndex[j] = textureIndices->GetAt(i3j);
			}

			/// *** Vertex *** ///

			// *** _localPosz *** //
			FbxVector4 position = ctrlPoints[i3j];
			myMesh._meshVertex[i3j]->_localPos = {
				static_cast<float>(position[0]),
				static_cast<float>(position[1]),
				static_cast<float>(position[2]) };

			// ***  _u, _v *** //
			myMesh._meshVertex[i3j]->_u = static_cast<float>(uvs[i3j][0]);
			myMesh._meshVertex[i3j]->_v = static_cast<float>(uvs[i3j][1]);

			// ***  _normal *** //
			if (myMesh._hasNormal)
			{
				FbxVector4 normal = normals[i3j];
				myMesh._meshVertex[i3j]->_normal = {
					 static_cast<float>(normal[0]),
					 static_cast<float>(normal[1]),
					 static_cast<float>(normal[2]) };
			}
 
			// ***  _tangent *** //
			if (myMesh._hasTangent)
			{
				FbxVector4 tangent{ tangents->GetAt(0) };
				myMesh._meshVertex[i]->_tangent = {
					static_cast<float>(tangent[0]),
					static_cast<float>(tangent[1]),
					static_cast<float>(tangent[2]) };

				// ***  _bitangent *** //
				FbxVector4 bitangent{ binormals->GetAt(0) };
				myMesh._meshVertex[i]->_bitangent = {
					static_cast<float>(bitangent[0]),
					static_cast<float>(bitangent[1]),
					static_cast<float>(bitangent[2]) };
			}
		}
	}

	FBXSDK_printf("---------------------------------\n");

	return true;
}

ImpMesh* FBXLoad::Loader::GetMesh(const char* file, size_t i)
{
	LoadFbxFile(file);

	if (i >= _meshes.size())
	{
		return nullptr;
	}

    return _meshes[i];
}

std::vector<ImpMesh*> FBXLoad::Loader::GetMeshAll(const char* file)
{
	LoadFbxFile(file);

    return _meshes;
}

size_t FBXLoad::Loader::GetMeshSize(const char* file)
{
	LoadFbxFile(file);

	return _meshes.size();
}
