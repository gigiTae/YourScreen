#include "Parser.h"
#include "../ImpStructure/ImpMesh.h"
using namespace ImpStructure;

#include "Common.h"
#include "Geometry.h"

FBXLoad::Parser::Parser(const char* fileName)
	: _scene{ new Scene()}
{
	assert(fileName && _scene);
	FBXSDK_printf("\n>> 파일 이름: \"%s\" \n", fileName);

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
	FBXSDK_printf("Paser::LoadFbxFile(const char*) 호출중...\n");
	assert(!IsValid());

	// FBX SDK 매니저 초기화
	_fbxManager = FbxManager::Create();

	if (!_fbxManager)
	{
		FBXSDK_printf("오류: 《FbxManager》를 만들 수 없음.\n\n");
		exit(1);

		return false;
	}
	else FBXSDK_printf("\n>> Autodesk FBX SDK 버전: \"%s\"\n", _fbxManager->GetVersion());

	// IO 설정 객체 생성 및 초기화
	FbxIOSettings* ios{ FbxIOSettings::Create(_fbxManager, IOSROOT) };
	_fbxManager->SetIOSettings(ios);
	
	return true;
}

void FBXLoad::Parser::LoadFbxFile(const char* file)
{
	// 
	// 1. importer 생성
	// 2. importer 로 scene 생성
	// 3. scene 데이터 읽기
	// 4. importer 삭제
	//

	/// importer 생성

	// FBX SDK 매니저로 importer 생성
	FbxImporter* fbxImporter{ FbxImporter::Create(_fbxManager, "Importer") };

	// 해당 파일이 없으면 다음과 같은 오류 발생
	if (!fbxImporter->Initialize(file, -1, _fbxManager->GetIOSettings()))
	{
		FBXSDK_printf("FbxImporter::Initialize() 함수 호출 실패.\n\n");
		FBXSDK_printf("오류 보고: %s\n\n", fbxImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	/// 씬 데이터 읽기

	// 씬을 생성
	_fbxScene = FbxScene::Create(_fbxManager, "Importer Scene");
	_scene->name = _fbxScene->GetName();
	FBXSDK_printf("\n>> Scene name: \"%s\"\n\n", _scene->name);

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

	/// importer 삭제

	// 씬을 불러왔으므로 importer 삭제
	fbxImporter->Destroy();
}

void FBXLoad::Parser::GetScene(FbxNode* root /*= nullptr*/)
{
	FBXSDK_printf("\n> Paser::GetScene(FbxNode*) 호출중...\n");
	assert(IsValid());

	// 루트가 없다면 새 루트 노드 생성
	if (!root)
	{
		//FBXSDK_printf("FbxScene::GetRootNode() 호출중...\n");
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
		GetNode(node);

		// 재귀 호출. 더 낮은 계층 단계로 내려감
		GetScene(node);
	}
}

void FBXLoad::Parser::GetNode(FbxNode* node)
{
	/*
	* 생각해보니 당장 Mesh 말고 내보낼 만한 게 없다
	* 2023-12-21
	*/

	FBXSDK_printf("\n> Paser::GetNode(FbxNode*) 호출중...\n");
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
	FBXSDK_printf("Paser::GetMesh(FbxNode*, std::vector<Mesh>&) 호출중...\n");
	assert(node);

	if (FbxMesh * fbxMesh{ node->GetMesh() }) // 원본 메시 **
	{
		if (fbxMesh->RemoveBadPolygons() < 0) return;

		// 모든 노드를 가능한 모두 삼각형 폴리곤으로 메시로 만드는 것
		FbxGeometryConverter geoConvertor{ _fbxManager };
		fbxMesh = static_cast<FbxMesh*>(geoConvertor.Triangulate(fbxMesh, true));
		if (!fbxMesh || fbxMesh->RemoveBadPolygons() < 0) return;

		// 새 메시 생성
		Mesh myMesh;
		myMesh.id = (int)meshes.size();
		myMesh.name = (node->GetName()[0] != '\0') ? node->GetName() : fbxMesh->GetName();

		/*** 메시 데이터 파싱 메서드 호출 ***/
		FBXSDK_printf("Parser::GetMeshData() 호출중...\n");
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

	/// *** 머터리얼 *** ///
	if(!GetMaterial(fbxMesh, myMesh)) return false;

	/// *** 포지션 파싱 *** ///
	if(!GetControlPoints(fbxMesh, myMesh)) return false;

	/// *** 노말 *** ///
	GetVertexNormals(fbxMesh, myMesh);

	/// *** 탄젠트 *** ///
	GetTangents(fbxMesh, myMesh);

	/// *** UV *** ///
	GetUVSet(fbxMesh, myMesh);

	FBXSDK_printf("---------------------------------\n");

	// 끗
	return true;
}

bool FBXLoad::Parser::GetControlPoints(FbxMesh* fbxMesh, Mesh& myMesh)
{
	// 정점 수
	const int numVertices{ fbxMesh->GetControlPointsCount() };
	FbxVector4* vertices{ fbxMesh->GetControlPoints() }; 
	FBXSDK_printf("\n# Vertex count: %d\n", numVertices);

	// 인덱스
	const int numIndices{ fbxMesh->GetPolygonVertexCount() };
	int* indeces{ fbxMesh->GetPolygonVertices() };
	myMesh.indices.resize(numIndices);
	FBXSDK_printf("\n# Indices count: %d\n", numIndices);

	assert(numVertices > 0 && vertices && numIndices > 0 && indeces);
	if(!(numVertices > 0 && vertices && numIndices > 0 && indeces)) return false;

	const int invalidId = -666;
	std::vector<int> refVertex(numVertices, invalidId); // 참조할 정점 인덱스 

	// 
	// 일반적으로
	// (정점의 인덱스 수) > (정점의 수)
	// 
	// 인덱스가 같은(포지션 값이 중복되는) 데이터의 파싱을 
	// 생략하는 작업인 거시다.
	// 

	for (int i{ 0 }; i < numIndices; i++) // 정점 인덱스 수 만큼 순회
	{
		const int idxVertex{ indeces[i] }; // 현재 인덱스

		FbxVector4 vertex = vertices[idxVertex];
		Vertex newVertex;
		newVertex.position = vertex;

		myMesh.indices[i] = i;
		myMesh.vertices.push_back(newVertex);

		// 이미 참조된 정점일 때?
		if (refVertex[idxVertex] != invalidId)
		{
			// 참조된 인덱스 업데이트 *** 
			myMesh.indices[i] = refVertex[idxVertex];
		}
		// 정점이 참조된 적이 없다?
		else
		{
			refVertex[idxVertex] = myMesh.indices[i]; // 참조될 정점 업데이트
		}

		FBXSDK_printf("## Vertex Position %-8d, index %-4d: ( %f, %f, %f )\n", i, myMesh.indices[i], vertex[0], vertex[1], vertex[2]);
	}

	// 삼각형 폴리곤 메시를 파싱하기 때문에 인덱스 사이즈가 3이 아니면 안된다. 
	assert(myMesh.indices.size() % 3 == 0);

	return true;
}

bool FBXLoad::Parser::GetMaterial(FbxMesh* fbxMesh, Mesh& myMesh)
{
	// 폴리곤 수
	const int numPolygon{ fbxMesh->GetPolygonCount() };
	if (numPolygon <= 0) return false;
	FBXSDK_printf("\n# Polygin count: %d\n", numPolygon);

	assert(numPolygon > 0);

	//
	// 폴리곤마다 중복되는 머터리얼이 있으므로 
	// 중복을 제거하는 작업을 한다.
	//

	FbxLayerElementArrayTemplate<int>* mtlsIndices;
	if (fbxMesh->GetMaterialIndices(&mtlsIndices))
	{
		for (int i{ 0 }; i < numPolygon; i++)
		{
			const int mtlIdex{ mtlsIndices->GetAt(i) };
			assert(mtlIdex >= 0);
			myMesh.materialIndices.emplace_back(mtlIdex);

			// materialUsed 안에 mtlIdex 가 없으면( `std::find`는 값을 못 찾으면 마지막 값을 반환한다.) 새로운 인덱스 추가
			if (std::find(myMesh.materialUsed.begin(), myMesh.materialUsed.end(), mtlIdex) == myMesh.materialUsed.end())
			{
				myMesh.materialUsed.emplace_back(mtlIdex); // materialUsed 안에 추가
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