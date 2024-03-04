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
	FBXSDK_printf("Loader::InitilazeFbx(void) 호출중...\n");
	assert(!IsValid());

	// FBX SDK 매니저 초기화
	_fbxManager = FbxManager::Create();

	if (!_fbxManager)
	{
		FBXSDK_printf("오류: 《FbxManager》를 만들 수 없음.\n\n");
		exit(1);

	}
	else FBXSDK_printf("\n>> Autodesk FBX SDK 버전: \"%s\"\n", _fbxManager->GetVersion());

	// IO 설정 객체 생성 및 초기화
	FbxIOSettings* ios{ FbxIOSettings::Create(_fbxManager, IOSROOT) };
	_fbxManager->SetIOSettings(ios);
}

void FBXLoad::Loader::LoadFbxFile(const char* file)
{
	assert(file);
	FBXSDK_printf("\n>> 파일 이름: \"%s\" \n", file);

	// 만일 새로운 파일을 받을 경우를 대비해서
	// 벡터를 비움
	_meshes.clear();

	// importer 생성
	FbxImporter* fbxImporter{ FbxImporter::Create(_fbxManager, "Importer") };

	// 해당 파일이 없으면 다음과 같은 오류 발생
	if (!fbxImporter->Initialize(file, -1, _fbxManager->GetIOSettings()))
	{
		FBXSDK_printf("FbxImporter::Initialize() 함수 호출 실패.\n\n");
		FBXSDK_printf("오류 보고: %s\n\n", fbxImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	// 씬 생성
	_fbxScene = FbxScene::Create(_fbxManager, "Importer Scene");
	FBXSDK_printf("\n>> Scene name: \"%s\"\n\n", _fbxScene->GetName());

	if (!_fbxScene)
	{
		FBXSDK_printf("오류: 《FbxScene》을 만들 수 없음!\n\n");
		exit(1);
	}

	// importer 에게 씬 전달
	if (!fbxImporter->Import(_fbxScene))
	{
		printf("FbxImporter::Import() 함수 호출 실패.\n\n");
		printf("오류 보고: %s\n\n", fbxImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	// importer 삭제
	fbxImporter->Destroy();

	assert(IsValid());

	/// Scene Load ***
	LoadScene();
}

void FBXLoad::Loader::LoadScene(FbxNode* root)
{
	assert(IsValid());

	// 루트가 없다면 새 루트 노드 생성
	if (!root)
	{
		root = _fbxScene->GetRootNode();

		if (!root)
		{
			FBXSDK_printf("오류: 여전히 루트 노드는 nullptr 입니다.\n\n");
			return;
		}
	}

	// 자식 노드 가져오기
	const int numNode{ root->GetChildCount() };
	FBXSDK_printf("\n>> Node Child count: %d\n", numNode);


	for (int i{ 0 }; i < numNode; i++)
	{
		FBXSDK_printf("\n>> Node Child number: %d\n", i);
		FbxNode* node{ root->GetChild(i) };

		// 재귀 호출 탈출문
		if (!node) continue;

		/// 노드 속성 별 데이터 가져오기
		LoadMesh(node);

		// 재귀 호출. 더 낮은 계층 단계로 내려감 
		LoadScene(node); 
	}
}

void FBXLoad::Loader::LoadMesh(FbxNode* node)
{
	if (FbxMesh * fbxMesh{ node->GetMesh() }) // 원본 메시 **
	{
		if (fbxMesh->RemoveBadPolygons() < 0) return;

		// 모든 노드를 가능한 모두 삼각형 폴리곤으로 메시로 만드는 것
		int* indeces{ fbxMesh->GetPolygonVertices() };		// 인덱스

		const int numVertices{ fbxMesh->GetPolygonVertexCount() };
		const int numFace{ fbxMesh->GetPolygonCount() };
		FbxVector4* ctrlPoints{ fbxMesh->GetControlPoints() }; // 정점

		FbxGeometryConverter geoConvertor{ _fbxManager };
		fbxMesh = static_cast<FbxMesh*>(geoConvertor.Triangulate(fbxMesh, true));
		if (!fbxMesh || fbxMesh->RemoveBadPolygons() < 0) return;

		// 새 메시 생성
		ImpMesh* myMesh = new ImpMesh();

		/*** 메시 데이터 파싱 메서드 호출 ***/
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

	// 데이터 가겨오기
	fbxMesh->GetUVSetNames(uvNames);
	fbxMesh->GetPolygonVertexNormals(normals);
	fbxMesh->GetTangents(&tangents);
	fbxMesh->GetBinormals(&binormals);
	fbxMesh->GetTextureIndices(&textureIndices, fbxsdk::FbxLayerElement::EType::eTextureDiffuse);
	
	// 크기 
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

	// 자료 유무
	myMesh._hasTexture = (textureIndices) ? true : false;
	myMesh._hasTangent = (tangents && binormals) ? true : false;
	myMesh._hasNormal = (normals) ? true : false;

	// 실제 데이터
	int* indeces{ fbxMesh->GetPolygonVertices() };		// 인덱스
	FbxVector4* ctrlPoints{ fbxMesh->GetControlPoints() }; // 정점
	fbxMesh->GetPolygonVertexUVs(uvNames.GetStringAt(0), uvs);
	
	for (int i = 0; i < 12; i++)
	{
		int a = fbxMesh->GetPolygonVertex(i, 0);
		int b = fbxMesh->GetPolygonVertex(i, 2);
		int c = fbxMesh->GetPolygonVertex(i, 1);
		int d = 3;
	}

	//int a{ textureIndices->GetAt(0) };

	// 벡터 리사이즈
	myMesh._meshVertex.resize(numVertices);
	myMesh._meshFace.resize(numFace);

	for (int i{ 0 }; i < numFace; i++) // 정점 페이스 수
	{
		myMesh._meshFace[i] = new ImpFace();

		for (int j{ 0 }; j < 3; j++) // 정점 인덱스 수 만큼
		{
			const int i3j{ 3 * i + j };
			myMesh._meshVertex[3 * i + j] = new ImpVertex();

			/// *** Index *** ///

			// *** vertex *** //
			const int idxVertex{ indeces[i3j] }; // 현재 인덱스
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
