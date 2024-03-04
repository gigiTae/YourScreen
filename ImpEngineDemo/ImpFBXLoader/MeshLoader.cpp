#include "MeshLoader.h"
#include "../ImpStructure/ImpMath.h"
#include <set>
#include <assert.h>

FBXLoad::MeshLoader::MeshLoader(FbxManager *manager) : _manager(manager)
{
	assert(_manager);
}

ImpMesh *FBXLoad::MeshLoader::GetMesh(size_t i)
{
	if (i >= _meshes.size())
	{
		return nullptr;
	}

	return _meshes[i];
}

std::vector<ImpMesh *> FBXLoad::MeshLoader::GetMeshAll()
{
	return _meshes;
}

size_t FBXLoad::MeshLoader::GetMeshSize()
{
	return _meshes.size();
}

// Poligon는 세개의 Vertex 로 이루어져 있다.
// 설령 fbx 파일의 모델의 Face 가 삼각형이 아니더라도
// 아래 함수를 거치면 삼각형이 된다.
void FBXLoad::MeshLoader::LoadMesh(FbxNode *node)
{
	// Fbx Mesh 생성
	FbxMesh *mesh = (FbxMesh *)node->GetNodeAttribute();

    if (!mesh || mesh->RemoveBadPolygons() < 0)
        return;

	// 세로운 메시 추가
	_meshes.emplace_back(new ImpMesh());

	// 가능한 모든 점을 삼각형 폴리곤으로 바꿈
	{
		FbxGeometryConverter geoConvertor{_manager};
		mesh = static_cast<FbxMesh *>(geoConvertor.Triangulate(mesh, true));
	}

	FBXSDK_printf("Mesh Name: %s\n", (char *)node->GetName());
	//LoadControlsPoints(mesh);

	// 메시 파싱이 이루어진다.
	LoadPolygons(mesh);

	// DisplayMaterialMapping(mesh);

	// DisplayTexture(mesh);

	/// ------------------------------

	// 기타 추가할만한 mesh 데이터
	// DisplayMaterial(lMesh);
	// DisplayMaterialConnections(lMesh);
	// DisplayLink(lMesh);
	// DisplayShape(lMesh);
	// DisplayCache(lMesh);

	/// ------------------------------

	// --------
	// 파싱한 데이터 가공
	// --------

	// 메시 스플릿
	//SplitPerNormal(_meshes.back());

	// 다 사용한 Fbx Mesh  제거
	mesh->Destroy();
}

void FBXLoad::MeshLoader::LoadControlsPoints(FbxMesh *mesh)
{
	int i, controlPointsCount = mesh->GetControlPointsCount();
	FbxVector4 *cntrolPoints = mesh->GetControlPoints();

	// 추출할 데이터
	_ControlPositions.clear();

	FBXSDK_printf("    Control Points\n");

	for (i = 0; i < controlPointsCount; i++)
	{
		FBXSDK_printf("        Control Point %d\n", i);
		FBXSDK_printf("            Coordinates: %f, %f, %f\n", cntrolPoints[i][0], cntrolPoints[i][1],
					  cntrolPoints[i][2]);

		_ControlPositions.emplace_back(static_cast<float>(cntrolPoints[i][0]), static_cast<float>(cntrolPoints[i][1]),
									   static_cast<float>(cntrolPoints[i][2]));

		for (int j = 0; j < mesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal *eNormals = mesh->GetElementNormal(j);
			if (eNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "            Normal Vector: ");
				if (eNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
					FBXSDK_printf("        %s %f, %f, %f\n", header, eNormals->GetDirectArray().GetAt(i)[0],
								  eNormals->GetDirectArray().GetAt(i)[1], eNormals->GetDirectArray().GetAt(i)[2]);

			}
		}
	}

	FBXSDK_printf("\n");
}

void FBXLoad::MeshLoader::LoadPolygons(FbxMesh *mesh)
{
	int i, j, polygonCount = mesh->GetPolygonCount();
	FbxVector4 *controlPoints = mesh->GetControlPoints();
	char header[100];

	// Fbx의 Poligon 은 ImpFace 와 같다.
	// 따라서 _meshFace 의 사이즈를 Face 크기만큼 잡아준다.

	_meshes.back()->_meshFace.resize(polygonCount);
	_meshes.back()->_meshVertex.resize(polygonCount * 3);

	FBXSDK_printf("    Polygons\n");

	int vertexId{0};

	for (i = 0; i < polygonCount; i++)
	{
		FBXSDK_printf("        Polygon %d\n", i);
		int l;

		for (l = 0; l < mesh->GetElementPolygonGroupCount(); l++)
		{
			FbxGeometryElementPolygonGroup *ePolgrp = mesh->GetElementPolygonGroup(l);
			switch (ePolgrp->GetMappingMode())
			{
			case FbxGeometryElement::eByPolygon:
				if (ePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
				{
					FBXSDK_sprintf(header, 100, "        Assigned to group: ");
					int polyGroupId = ePolgrp->GetIndexArray().GetAt(i);
					FBXSDK_printf("%s %d\n", header, polyGroupId);
					break;
				}
			default:
				// any other mapping modes don't make sense
				FBXSDK_printf("        \"unsupported group assignment\"\n");
				break;
			}
		}

		// 폴리곤의 사이즈는 무조건 3 이다.
		int polygonSize = mesh->GetPolygonSize(i);

		// 페이스를 생성한다.
		_meshes.back()->_meshFace[i] = new ImpFace();

		for (j = 0; j < polygonSize; j++)
		{
			// 정점을 생성한다.
			_meshes.back()->_meshVertex[i * 3 + j] = new ImpVertex();

			/// ----------------
			/// Vertex Position
			/// ----------------

			// 포지션을 담는 변수를 준비한다.
			FbxVector4 localPosition;

			int controlPointIndex = mesh->GetPolygonVertex(i, j);
			if (controlPointIndex < 0)
			{
				FBXSDK_printf("            Coordinates: Invalid index found!\n");
				continue;
			}
			else
			{
				localPosition = controlPoints[controlPointIndex];
				FBXSDK_printf("            Coordinates: %f, %f, %f\n", localPosition[0], localPosition[1],
							  localPosition[2]);

				// 위치값을 담는다.
				_meshes.back()->_meshVertex[i * 3 + j]->_localPos = {static_cast<float>(localPosition[0]),
																	 static_cast<float>(localPosition[1]),
																	 static_cast<float>(localPosition[2])};

				// 정점 데이터 값을 담는다
				_meshes.back()->_meshFace[i]->vertexIndex[j] = controlPointIndex;
			}

			/// --------------
			/// Vertex Color
			/// --------------

			// 컬러값을 담는 변수를 준비한다.
			FbxColor vertexColor;

			int eColorCount = mesh->GetElementVertexColorCount();

			// 컬러값 데이터 유무 표시한다.
			if (eColorCount)
				_meshes.back()->_hasColor = true;

			for (l = 0; l < eColorCount; l++)
			{
				_meshes.back()->_hasColor = true;
				FbxGeometryElementVertexColor *leVtxc = mesh->GetElementVertexColor(l);
				FBXSDK_sprintf(header, 100, "            Color vertex: ");

				switch (leVtxc->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect: {
						vertexColor = leVtxc->GetDirectArray().GetAt(controlPointIndex);
						FBXSDK_printf("%s %f, %f, %f, %f\n", header, vertexColor[0], vertexColor[1], vertexColor[2],
									  vertexColor[3]);
					}
					break;
					case FbxGeometryElement::eIndexToDirect: {
						int id = leVtxc->GetIndexArray().GetAt(controlPointIndex);
						vertexColor = leVtxc->GetDirectArray().GetAt(id);
						FBXSDK_printf("%s %f, %f, %f, %f\n", header, vertexColor[0], vertexColor[1], vertexColor[2],
									  vertexColor[3]);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex: {
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect: {
						vertexColor = leVtxc->GetDirectArray().GetAt(vertexId);
						FBXSDK_printf("%s %f, %f, %f, %f\n", header, vertexColor[0], vertexColor[1], vertexColor[2],
									  vertexColor[3]);
					}
					break;
					case FbxGeometryElement::eIndexToDirect: {
						int id = leVtxc->GetIndexArray().GetAt(vertexId);
						vertexColor = leVtxc->GetDirectArray().GetAt(id);
						FBXSDK_printf("%s %f, %f, %f, %f\n", header, vertexColor[0], vertexColor[1], vertexColor[2],
									  vertexColor[3]);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:	 // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:		 // doesn't make much sense for UVs
					break;
				}

				// 컬러 값을 담는다.
				_meshes.back()->_meshVertex[i * 3 + j]->_color = {
					static_cast<float>(vertexColor[0]), static_cast<float>(vertexColor[1]),
					static_cast<float>(vertexColor[2]), static_cast<float>(vertexColor[3])};

			} // for eColorCount

			/// ----
			/// UV
			/// ----

			// UV 데이터를 담는 변수를 준비한다.
			FbxVector2 uv;

			for (l = 0; l < mesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV *leUV = mesh->GetElementUV(l);
				FBXSDK_sprintf(header, 100, "            Texture UV: ");

				switch (leUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect: {
						uv = leUV->GetDirectArray().GetAt(controlPointIndex);
						FBXSDK_printf("%s %f, %f\n", header, uv[0], uv[1]);
					}
					break;
					case FbxGeometryElement::eIndexToDirect: {
						int id = leUV->GetIndexArray().GetAt(controlPointIndex);
						uv = leUV->GetDirectArray().GetAt(id);
						FBXSDK_printf("%s %f, %f\n", header, leUV->GetDirectArray().GetAt(id)[0],
									  leUV->GetDirectArray().GetAt(id)[1]);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex: {
					int lTextureUVIndex = mesh->GetTextureUVIndex(i, j);
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect: {
						uv = leUV->GetDirectArray().GetAt(lTextureUVIndex);
						FBXSDK_printf("%s %f, %f\n", header, uv[0], uv[1]);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:	 // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:		 // doesn't make much sense for UVs
					break;
				}

				// uv 좌표값을 담는다.
				_meshes.back()->_meshVertex[i * 3 + j]->_u = static_cast<float>(uv[0]);
				_meshes.back()->_meshVertex[i * 3 + j]->_v = static_cast<float>(uv[1]);

			} // for PolygonSize

			/// --------------
			/// Vertex Normal
			/// --------------

			// 노말 값을 담는 변수를 미리 준비한다.
			FbxVector4 normal;

			int normalCount = mesh->GetElementNormalCount();

			// 노말 값의 유무 표시한다.
			if (normalCount)
				_meshes.back()->_hasNormal = true;

			for (l = 0; l < normalCount; ++l)
			{
				FbxGeometryElementNormal *eNormal = mesh->GetElementNormal(l);
				FBXSDK_sprintf(header, 100, "            Normal: ");

				if (eNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (eNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect: {
						normal = eNormal->GetDirectArray().GetAt(vertexId);
						FBXSDK_printf("%s %f, %f, %f\n", header, normal[0], normal[1], normal[2]);
						int a = 3;
					}
					break;
					case FbxGeometryElement::eIndexToDirect: {
						int id = eNormal->GetIndexArray().GetAt(vertexId);
						normal = eNormal->GetDirectArray().GetAt(id);
						FBXSDK_printf("%s %f, %f, %f\n", header, normal[0], normal[1], normal[2]);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}

					// 노말 값을 담는다.
					_meshes.back()->_meshVertex[i * 3 + j]->_normal = {
						static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])};

				} // for normalCount
			}

			/// ---------
			/// Tangent
			/// ---------

			// 탄젠트 값을 담는 변수를 준비한다.
			FbxVector4 tangent;

			int tangentCount = mesh->GetElementTangentCount();

			// 탄젠트 값의 유무를 표시한다.
			if (tangentCount)
				_meshes.back()->_hasTangent = true;

			for (l = 0; l < tangentCount; ++l)
			{
				FbxGeometryElementTangent *eTangent = mesh->GetElementTangent(l);
				FBXSDK_sprintf(header, 100, "            Tangent: ");

				if (eTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (eTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect: {
						tangent = eTangent->GetDirectArray().GetAt(vertexId);
						FBXSDK_printf("%s %f, %f, %f\n", header, tangent[0], tangent[1], tangent[2]);
					}
					break;
					case FbxGeometryElement::eIndexToDirect: {
						int id = eTangent->GetIndexArray().GetAt(vertexId);
						tangent = eTangent->GetDirectArray().GetAt(id);
						FBXSDK_printf("%s %f, %f, %f\n", header, tangent[0], tangent[1], tangent[2]);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

				// 탄젠트 값을 담는다.
				_meshes.back()->_meshVertex[i * 3 + j]->_tangent = {
					static_cast<float>(tangent[0]), static_cast<float>(tangent[1]), static_cast<float>(tangent[2])};

			} // for tangentCount

			/// ---------
			/// Binormal
			/// ---------

			// 바이노말 값을 담는 변수를 준비한다.

			FbxVector4 bitangent;

			for (l = 0; l < mesh->GetElementBinormalCount(); ++l)
			{
				FbxGeometryElementBinormal *eBinormal = mesh->GetElementBinormal(l);

				FBXSDK_sprintf(header, 100, "            Binormal: ");
				if (eBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (eBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect: {
						bitangent = eBinormal->GetDirectArray().GetAt(vertexId);
						FBXSDK_printf("%s %f, %f, %f\n", header, bitangent[0], bitangent[1], bitangent[2]);
					}
					break;
					case FbxGeometryElement::eIndexToDirect: {
						int id = eBinormal->GetIndexArray().GetAt(vertexId);
						bitangent = eBinormal->GetDirectArray().GetAt(id);
						FBXSDK_printf("%s %f, %f, %f\n", header, bitangent[0], bitangent[1], bitangent[2]);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

				// 바이노말 값을 담는다.
				_meshes.back()->_meshVertex[i * 3 + j]->_bitangent = {static_cast<float>(bitangent[0]),
																	  static_cast<float>(bitangent[1]),
																	  static_cast<float>(bitangent[2])};

			} // for Binormal Count

			FBXSDK_printf("            >>> Vertex ID: %d\n\n", vertexId);

			vertexId++;
		} // for polygonSize
	}	  // for polygonCount

	// check visibility for the edges of the mesh
	for (int l = 0; l < mesh->GetElementVisibilityCount(); ++l)
	{
		FbxGeometryElementVisibility *leVisibility = mesh->GetElementVisibility(l);
		FBXSDK_sprintf(header, 100, "    Edge Visibility : ");
		// DisplayString(header);
		switch (leVisibility->GetMappingMode())
		{
		default:
			break;
			// should be eByEdge
		case FbxGeometryElement::eByEdge:
			// should be eDirect
			for (j = 0; j != mesh->GetMeshEdgeCount(); ++j)
			{
				FBXSDK_printf("        Edge %d\n", j);
				FBXSDK_printf("              Edge visibility: %s\n",
							  leVisibility->GetDirectArray().GetAt(j) ? "true" : "false");
			}

			break;
		}
	}
}

void FBXLoad::MeshLoader::DisplayMaterialMapping(FbxMesh *pMesh)
{
	const char *lMappingTypes[] = {"None",		 "By Control Point", "By Polygon Vertex",
								   "By Polygon", "By Edge",			 "All Same"};
	const char *lReferenceMode[] = {"Direct", "Index", "Index to Direct"};

	int lMtrlCount = 0;
	FbxNode *lNode = NULL;
	if (pMesh)
	{
		lNode = pMesh->GetNode();
		if (lNode)
			lMtrlCount = lNode->GetMaterialCount();

		for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
		{
			FbxGeometryElementMaterial *leMat = pMesh->GetElementMaterial(l);
			if (leMat)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "    Material Element %d: ", l);
				FBXSDK_printf("%s\n", header);

				FBXSDK_printf("           Mapping: %s\n", lMappingTypes[leMat->GetMappingMode()]);
				FBXSDK_printf("           ReferenceMode: %s\n", lReferenceMode[leMat->GetReferenceMode()]);

				int lMaterialCount = 0;
				FbxString lString;

				if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect ||
					leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				{
					lMaterialCount = lMtrlCount;
				}

				if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex ||
					leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				{
					int i;

					lString = "           Indices: ";

					int lIndexArrayCount = leMat->GetIndexArray().GetCount();
					for (i = 0; i < lIndexArrayCount; i++)
					{
						lString += leMat->GetIndexArray().GetAt(i);

						if (i < lIndexArrayCount - 1)
						{
							lString += ", ";
						}
					}

					lString += "\n";

					FBXSDK_printf(lString);
				}
			}
		}
	}
}

void FBXLoad::MeshLoader::DisplayTexture(FbxGeometry *geometry)
{
	int materialIndex;
	FbxProperty property;
	if (geometry->GetNode() == NULL)
		return;

	int nbMat = geometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (materialIndex = 0; materialIndex < nbMat; materialIndex++)
	{
		FbxSurfaceMaterial *material = geometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(materialIndex);
		bool displayHeader = true;

		// go through all the possible textures
		if (material)
		{
			int textureIndex;
			//_meshes.back()->_hasTexture = true;
			//_meshes.back()->_meshFace->textureIndex[j];
			FBXSDK_FOR_EACH_TEXTURE(textureIndex)
			{
				property = material->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
				int textureCount = property.GetSrcObjectCount<FbxTexture>();

				for (int j = 0; j < textureCount; ++j)
				{
					// Here we have to check if it's layeredtextures, or just textures:
					FbxLayeredTexture *layeredTexture = property.GetSrcObject<FbxLayeredTexture>(j);
					if (layeredTexture)
					{
						FBXSDK_printf("    Layered Texture: %d\n", j);
						int nbTextures = layeredTexture->GetSrcObjectCount<FbxTexture>();
						for (int k = 0; k < nbTextures; ++k)
						{
							FbxTexture *texture = layeredTexture->GetSrcObject<FbxTexture>(k);
							if (texture)
							{

								if (displayHeader)
								{
									FBXSDK_printf("    Textures connected to Material %d\n", materialIndex);
									displayHeader = false;
								}

								// NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
								// Why is that?  because one texture can be shared on different layered textures and
								// might have different blend modes.

								FbxLayeredTexture::EBlendMode lBlendMode;
								layeredTexture->GetTextureBlendMode(k, lBlendMode);
								FBXSDK_printf("    Textures for %s\n", property.GetName().Buffer());
								FBXSDK_printf("        Texture %d\n", k);
								// DisplayTextureInfo(lTexture, (int)lBlendMode);
							}
						}
					}
					else
					{
						// no layered texture simply get on the property
						FbxTexture *texture = property.GetSrcObject<FbxTexture>(j);
						if (texture)
						{
							// display connected Material header only at the first time
							if (displayHeader)
							{
								FBXSDK_printf("    Textures connected to Material %d", materialIndex);
								displayHeader = false;
							}

							FBXSDK_printf("    Textures for %s\n", property.GetName().Buffer());
							FBXSDK_printf("        Texture %d\n", j);
							// DisplayTextureInfo(texture, -1);
						}
					}
				}
			}

		} // end if(lMaterial)

	} // end for lMaterialIndex
}
