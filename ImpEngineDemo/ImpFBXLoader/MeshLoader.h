/// MeshLoader.h : ControlPoint, Polgon, Material ���� �����͸� �ٷ�� Ŭ���� �Դϴ�.
/// 
/// 2023-12-29

#pragma once

#include <fbxsdk.h>
#include "../ImpStructure/ImpMesh.h"

using namespace ImpStructure;

namespace FBXLoad
{

class MeshLoader
{
public:
	MeshLoader(FbxManager* manager);

	ImpMesh* GetMesh(size_t i);
	std::vector<ImpMesh*> GetMeshAll();
	size_t GetMeshSize();

	void LoadMesh(FbxNode* node);

private:
	// ���� ������
	void LoadControlsPoints(FbxMesh* mesh);
	// ������ ������
	void LoadPolygons(FbxMesh* mesh);
	// ���͸��� ������
	void DisplayMaterialMapping(FbxMesh* mesh);

	//void DisplayMaterial(FbxMesh* mesh);
	void DisplayTexture(FbxGeometry* mesh);
	//void DisplayMaterialConnections(FbxGeometry* mesh);
	//void DisplayLink(FbxGeometry* mesh);
	//void DisplayShape(FbxGeometry* mesh);
	//void DisplayCache(FbxGeometry* mesh);

	/// <summary>
	///  �ܺο��� �޾� ���� Fbx Manager
	/// </summary>
	FbxManager* _manager{ nullptr };

	std::vector<Vector3> _ControlPositions;

	/// <summary>
	/// ���������� �ܺη� ������ ������
	/// </summary>
	std::vector<ImpMesh*> _meshes;
};
}