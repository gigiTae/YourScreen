#include "pch.h"
#include "ResourceManager.h" 
#include "Resource.h"

#include "Define.h"

//temp
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d11shader.h>
#include <d3dcompiler.h>
#include "../ImpLib/Inc/DDSTextureLoader.h"
#include "../ImpLib/Inc/WICTextureLoader.h"

#include "ImpDevice.h"
#include "ImpLoader.h"
#include "../ImpFBXLoader/IFBXLoader.h"
#include "../ImpASELoader/IASELoader.h"

#include "../ImpStructure/ImpMesh.h"

#include "Vertex.h"

//tepm
#include "ImpAnimationPlayer.h"

using namespace ImpGraphics;
using namespace ImpStructure;
using namespace DirectX;

#pragma region VBIB
GeometryBuffer::GeometryBuffer(ResourceManager* resourceManager)
	:Resource(resourceManager),
	_VBs(), _IBs(),
	_vertexSizes(), _indexSizes(), _size(0),
	_vertexType(),
	_nodes()
{

}

GeometryBuffer::~GeometryBuffer()
{
	for (UINT i = 0; i < _VBs.size(); i++)
	{
		ReleaseCOM(_VBs[i]);
		ReleaseCOM(_IBs[i]);
	}
}

void GeometryBuffer::Load(const std::wstring& path)
{
	// 만약 3글자 이상의 확장자를 쓸 거라면 마지막 / 위치를 찾아서 거기부터 잘라야 함 - 처리 완료
	std::vector<ImpStructure::ImpMesh*> meshes;
	ImpStructure::ImpMesh* mesh = nullptr;

	std::wstring fileExtension = path.substr(path.find_last_of(L".") + 1, path.length() - path.find_last_of(L".") + 1);
	if (fileExtension._Equal(L"imp"))
	{
		mesh = _resourceManager->GetImpLoader()->GetMesh(path);
		if (mesh == nullptr)
		{
			MessageBox(NULL, L"imp 파일 메시 로드에 실패했습니다.", path.c_str() , MB_ICONERROR);
		}

		CreateVBIB(mesh);
	}
	else if (fileExtension._Equal(L"grid"))
	{
        ImpStructure::ImpMesh* mesh = _resourceManager->GetGeometryMaker()->MakeGrid();

        CreateVBIB(mesh);

        delete mesh;
	}
	else if (fileExtension._Equal(L"ASE") || fileExtension._Equal(L"ase"))
	{
		size_t lastPoint = path.find_last_of(L".");
		size_t lastSlash = path.find_last_of(L"/");

		std::wstring fileName;
		if (lastSlash == std::string::npos)
		{
			lastSlash = path.find_last_of(L"\\");
			fileName = path.substr(lastSlash + 1, lastPoint - lastSlash - 1);
			fileName = L"Resources\\Mesh\\imp\\" + fileName + L".imp";
			mesh = _resourceManager->GetImpLoader()->GetMesh(fileName);

			if (mesh != nullptr)
			{
				CreateVBIB(mesh);
			}
		}
		else
		{
			fileName = path.substr(lastSlash + 1, lastPoint - lastSlash - 1);
			fileName = L"Resources/Mesh/imp/" + fileName + L".imp";
			mesh = _resourceManager->GetImpLoader()->GetMesh(fileName);

			if (mesh != nullptr)
			{
				CreateVBIB(mesh);
			}
		}

		if (mesh == nullptr)
		{
			const wchar_t* pathWchar = path.c_str();
			wchar_t* wchar = const_cast<wchar_t*>(pathWchar);

			char* pStr;
			int strSize = WideCharToMultiByte(CP_ACP, 0, wchar, -1, NULL, 0, NULL, NULL);
			pStr = new char[strSize];
			WideCharToMultiByte(CP_ACP, 0, wchar, -1, pStr, strSize, 0, 0);

			//mesh = _resourceManager->GetASELoader()->GetMesh(pStr, 0);
			meshes = _resourceManager->GetASELoader()->GetMeshAll(pStr);

			_resourceManager->GetImpLoader()->SaveMesh(mesh, fileName);

			for (UINT i = 0; i < meshes.size(); i++)
			{
				if (meshes[i]->_meshVertex.size() > 0)
				{
					CreateVBIB(meshes[i]);
				}
			}
			delete[] pStr;
			//CreateVBIB(mesh);
		}
	}
	else if (fileExtension._Equal(L"fbx"))
	{
		// fbx 말고 imp를 찾는다. Resource/mesh/imp 에서
		// 없으면 fbx를 로드한다.
		// 로드한 것을 imp로 저장한다. Resource/mesh/imp

	 	size_t lastPoint = path.find_last_of(L".");
		size_t lastSlash = path.find_last_of(L"/");
	 
		std::wstring fileName;
		if (lastSlash == std::string::npos)
		{
			lastSlash = path.find_last_of(L"\\");
			fileName = path.substr(lastSlash + 1, lastPoint - lastSlash - 1);
			fileName = L"Resources\\Mesh\\imp\\" + fileName + L".imp";
			mesh = _resourceManager->GetImpLoader()->GetMesh(fileName);
		}
		else
		{
			fileName = path.substr(lastSlash + 1, lastPoint - lastSlash - 1);
			fileName = L"Resources/Mesh/imp/" + fileName + L".imp";
			mesh = _resourceManager->GetImpLoader()->GetMesh(fileName);
		}

		if (mesh == nullptr)
		{
			const wchar_t* pathWchar = path.c_str();
			wchar_t* wchar = const_cast<wchar_t*>(pathWchar);

			char* pStr;
			int strSize = WideCharToMultiByte(CP_ACP, 0, wchar, -1, NULL, 0, NULL, NULL);
			pStr = new char[strSize];
			WideCharToMultiByte(CP_ACP, 0, wchar, -1, pStr, strSize, 0, 0);
			
			_resourceManager->GetFBXLoader()->SetPath(pStr);

 			mesh = _resourceManager->GetFBXLoader()->GetMesh();
			//ImpAnimationObject* temp = _resourceManager->GetFBXLoader()->GetAnimationObject();
			//_bones = _resourceManager->GetFBXLoader()->GetBoneList();
            //meshes = _resourceManager->GetFBXLoader()->GetMeshAll(pStr);

			//_resourceManager->GetAniamtionPlayer()->AddObject(temp);

			//test용 애니메이션 재생
            //_nodes.push_back(temp->_nodes[mesh->_meshName]);

			// fbx sdk가 weight의 합이 1이 안되어서 보정... 안하면... 폴리곤이 깨져...
			std::vector<float> sums;
			for (int i = 0; i < mesh->_meshVertex.size(); i++)
			{
                float sum = 0; 
                if (mesh->_meshVertex[i]->_weightIndices[0] != -1)
                {
                    sum += mesh->_meshVertex[i]->_weight.x;
                }
                if (mesh->_meshVertex[i]->_weightIndices[1] != -1)
                {
                    sum += mesh->_meshVertex[i]->_weight.y;
                }
                if (mesh->_meshVertex[i]->_weightIndices[2] != -1)
                {
                    sum += mesh->_meshVertex[i]->_weight.z;
                }
                if (mesh->_meshVertex[i]->_weightIndices[3] != -1)
                {
                    sum += 1 - (mesh->_meshVertex[i]->_weight.x +
                                mesh->_meshVertex[i]->_weight.y +
                                mesh->_meshVertex[i]->_weight.z);
                }

				if ((sum - 1.0f) < -0.000001f)
				{
                    mesh->_meshVertex[i]->_weight.x /= sum;
                    mesh->_meshVertex[i]->_weight.y /= sum;
                    mesh->_meshVertex[i]->_weight.z /= sum;
				}

                sums.push_back(sum);
			}

			delete[] pStr;
			_resourceManager->GetImpLoader()->SaveMesh(mesh, fileName);
		}

		CreateVBIB(mesh);
	}
	if (mesh != nullptr)
	{
		//CreateVBIB(mesh);
	}
	else
	{
		//MessageBox(NULL, L"Mesh가 nullptr입니다.", L"에러", MB_ICONERROR);
	}
	
	delete mesh;
	for (UINT i = 1; i < meshes.size(); i++)
	{
		//delete meshes[i];
	}
}

void GeometryBuffer::Load(const std::wstring& path, Vector3 center, Vector3 length)
{
	ImpStructure::ImpMesh* mesh = _resourceManager->GetGeometryMaker()->MakeBoxLineListMesh(center, length);

	CreateVBIB(mesh);

	delete mesh;
}

void GeometryBuffer::Load(const std::wstring& path, Vector3 center, float radius)
{
	ImpStructure::ImpMesh* mesh = _resourceManager->GetGeometryMaker()->MakeSphereLineListMesh(center, radius);

	CreateVBIB(mesh);

	delete mesh;
}

void GeometryBuffer::Load(const std::wstring& path,
                          Vector4 uiPos,
                          DirectX::XMFLOAT2 screenSize)
{
	ImpStructure::ImpMesh* mesh = _resourceManager->GetGeometryMaker()->MakeQuadPolygonMesh(uiPos, screenSize);

	CreateVBIB(mesh);

	delete mesh;
}

void GeometryBuffer::Load(const std::wstring& path, std::wstring& animationName)
{
    ImpStructure::ImpMesh* mesh = nullptr;

    size_t lastPoint = path.find_last_of(L".");
    size_t lastSlash = path.find_last_of(L"/");

    std::wstring fileName;
    if (lastSlash == std::string::npos)
    {
        lastSlash = path.find_last_of(L"\\");
        fileName  = path.substr(lastSlash + 1, lastPoint - lastSlash - 1);
        fileName  = L"Resources\\Mesh\\imp\\" + fileName + L".imp";
        mesh      = _resourceManager->GetImpLoader()->GetMesh(fileName);
    }
    else
    {
        fileName = path.substr(lastSlash + 1, lastPoint - lastSlash - 1);
        fileName = L"Resources/Mesh/imp/" + fileName + L".imp";
        mesh     = _resourceManager->GetImpLoader()->GetMesh(fileName);
    }

    if (mesh == nullptr)
    {
        const wchar_t* pathWchar = path.c_str();
        wchar_t* wchar           = const_cast<wchar_t*>(pathWchar);

        char* pStr;
        int strSize = WideCharToMultiByte(
            CP_ACP, 0, wchar, -1, NULL, 0, NULL, NULL);
        pStr = new char[strSize];
        WideCharToMultiByte(CP_ACP, 0, wchar, -1, pStr, strSize, 0, 0);

        _resourceManager->GetFBXLoader()->SetPath(pStr);

        mesh = _resourceManager->GetFBXLoader()->GetMesh();
        ImpAnimationObject* temp = _resourceManager->GetFBXLoader()->GetAnimationObject();
        //_bones = _resourceManager->GetFBXLoader()->GetBoneList();
        _animationsBones[animationName] = _resourceManager->GetFBXLoader()->GetBoneList();
        // meshes = _resourceManager->GetFBXLoader()->GetMeshAll(pStr);

        _resourceManager->GetAniamtionPlayer()->AddObject(animationName, temp);

        // test용 애니메이션 재생
        _nodes.push_back(temp->_nodes[mesh->_meshName]);

        // fbx sdk가 weight의 합이 1이 안되어서 보정... 안하면... 폴리곤이 깨져...
        std::vector<float> sums;
        for (int i = 0; i < mesh->_meshVertex.size(); i++)
        {
            float sum = 0;
            if (mesh->_meshVertex[i]->_weightIndices[0] != -1)
            {
                sum += mesh->_meshVertex[i]->_weight.x;
            }
            if (mesh->_meshVertex[i]->_weightIndices[1] != -1)
            {
                sum += mesh->_meshVertex[i]->_weight.y;
            }
            if (mesh->_meshVertex[i]->_weightIndices[2] != -1)
            {
                sum += mesh->_meshVertex[i]->_weight.z;
            }
            if (mesh->_meshVertex[i]->_weightIndices[3] != -1)
            {
                sum += 1 - (mesh->_meshVertex[i]->_weight.x +
                               mesh->_meshVertex[i]->_weight.y +
                               mesh->_meshVertex[i]->_weight.z);
            }

            if ((sum - 1.0f) < -0.000001f)
            {
                mesh->_meshVertex[i]->_weight.x /= sum;
                mesh->_meshVertex[i]->_weight.y /= sum;
                mesh->_meshVertex[i]->_weight.z /= sum;
            }

            sums.push_back(sum);
        }

        delete[] pStr;
        //_resourceManager->GetImpLoader()->SaveMesh(mesh, fileName);
    }

    CreateVBIB(mesh);

	delete mesh;
}

void GeometryBuffer::CreateVBIB(ImpStructure::ImpMesh* mesh)
{
	// 버텍스 포지션 따로 저장 (일단 픽킹용)
	std::vector<ImpMath::Vector3> position;
	for (UINT i = 0; i < mesh->_meshVertex.size(); i++)
	{
		position.push_back(ImpMath::Vector3(mesh->_meshVertex[i]->_localPos.x, mesh->_meshVertex[i]->_localPos.y, mesh->_meshVertex[i]->_localPos.z));
	}
	_positions.push_back(position);

	/// 버텍스 버퍼 생성
	if (mesh->_hasWeight)
	{
		std::vector<Vertex::SkinnedVertex> vertices;
		Vertex::SkinnedVertex temp{};
		_vertexSizes.push_back(sizeof(Vertex::SkinnedVertex));
		_vertexType = VertexType::SkinnedVertex;

		for (UINT i = 0; i < mesh->_meshVertex.size(); i++)
		{
			temp.Pos.x = mesh->_meshVertex[i]->_localPos.x;
			temp.Pos.y = mesh->_meshVertex[i]->_localPos.y;
			temp.Pos.z = mesh->_meshVertex[i]->_localPos.z;

			temp.Normal.x = mesh->_meshVertex[i]->_normal.x;
			temp.Normal.y = mesh->_meshVertex[i]->_normal.y;
			temp.Normal.z = mesh->_meshVertex[i]->_normal.z;

			temp.Tex.x = mesh->_meshVertex[i]->_u;
			temp.Tex.y = 1 - mesh->_meshVertex[i]->_v;

			temp.weight.x = mesh->_meshVertex[i]->_weight.x;
			temp.weight.y = mesh->_meshVertex[i]->_weight.y;
			temp.weight.z = mesh->_meshVertex[i]->_weight.z;

			temp.Tangent.x = mesh->_meshVertex[i]->_tangent.x;
            temp.Tangent.y = mesh->_meshVertex[i]->_tangent.y;
            temp.Tangent.z = mesh->_meshVertex[i]->_tangent.z;

			temp.BoneIndices[0] = mesh->_meshVertex[i]->_weightIndices[0];
			temp.BoneIndices[1] = mesh->_meshVertex[i]->_weightIndices[1];
			temp.BoneIndices[2] = mesh->_meshVertex[i]->_weightIndices[2];
			temp.BoneIndices[3] = mesh->_meshVertex[i]->_weightIndices[3];

			vertices.emplace_back(temp);
		}

		ID3D11Buffer* VB;

		D3D11_BUFFER_DESC vbd{};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex::SkinnedVertex) * (UINT)vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData{};
		vinitData.pSysMem = &vertices[0];
		HR(_resourceManager->GetDevice()->GetDevice()->CreateBuffer(&vbd, &vinitData, &VB));

		_VBs.push_back(VB);
	}
	else if (mesh->_hasColor && mesh->_hasTexture)
	{
		std::vector<Vertex::PosColorTex> vertices;
		Vertex::PosColorTex temp{};
		_vertexSizes.push_back(sizeof(Vertex::PosColorTex));
		_vertexType = VertexType::PosColorTex;

		for (UINT i = 0; i < mesh->_meshVertex.size(); i++)
		{
			temp.Pos.x = mesh->_meshVertex[i]->_localPos.x;
			temp.Pos.y = mesh->_meshVertex[i]->_localPos.y;
			temp.Pos.z = mesh->_meshVertex[i]->_localPos.z;

			temp.Color.x = mesh->_meshVertex[i]->_color.x;
			temp.Color.y = mesh->_meshVertex[i]->_color.y;
			temp.Color.z = mesh->_meshVertex[i]->_color.z;
			temp.Color.w = mesh->_meshVertex[i]->_color.w;

			temp.Tex.x = mesh->_meshVertex[i]->_u;
			temp.Tex.y = mesh->_meshVertex[i]->_v;

			vertices.emplace_back(temp);
		}

		ID3D11Buffer* VB;

		D3D11_BUFFER_DESC vbd{};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex::PosColorTex) * (UINT)vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData{};
		vinitData.pSysMem = &vertices[0];
		HR(_resourceManager->GetDevice()->GetDevice()->CreateBuffer(&vbd, &vinitData, &VB));

		_VBs.push_back(VB);
	}

	else if (mesh->_hasNormal && mesh->_hasTexture && mesh->_hasTangent)
	{
		std::vector<Vertex::MeshVertex> vertices;
		Vertex::MeshVertex temp{};
		_vertexSizes.push_back(sizeof(Vertex::MeshVertex));
		_vertexType = VertexType::MeshVertex;

		for (UINT i = 0; i < mesh->_meshVertex.size(); i++)
		{
			temp.Pos.x = mesh->_meshVertex[i]->_localPos.x;
			temp.Pos.y = mesh->_meshVertex[i]->_localPos.y;
			temp.Pos.z = mesh->_meshVertex[i]->_localPos.z;

			temp.Normal.x = mesh->_meshVertex[i]->_normal.x;
			temp.Normal.y = mesh->_meshVertex[i]->_normal.y;
			temp.Normal.z = mesh->_meshVertex[i]->_normal.z;

			temp.Tangent.x = mesh->_meshVertex[i]->_tangent.x;
			temp.Tangent.y = mesh->_meshVertex[i]->_tangent.y;
			temp.Tangent.z = mesh->_meshVertex[i]->_tangent.z;

			temp.Tex.x = mesh->_meshVertex[i]->_u;
			temp.Tex.y = 1 - mesh->_meshVertex[i]->_v;

			vertices.emplace_back(temp);
		}

		ID3D11Buffer* VB;

		D3D11_BUFFER_DESC vbd{};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex::MeshVertex) * (UINT)vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData{};
		vinitData.pSysMem = &vertices[0];
		HR(_resourceManager->GetDevice()->GetDevice()->CreateBuffer(&vbd, &vinitData, &VB));

		_VBs.push_back(VB);
	}

	else if (mesh->_hasNormal)
	{
		std::vector<Vertex::PosNormal> vertices;
		Vertex::PosNormal temp{};
		_vertexSizes.push_back(sizeof(Vertex::PosNormal));
		_vertexType = VertexType::PosNormal;

		for (size_t i = 0; i < mesh->_meshVertex.size(); i++)
		{
			temp.Pos.x = mesh->_meshVertex[i]->_localPos.x;
			temp.Pos.y = mesh->_meshVertex[i]->_localPos.y;
			temp.Pos.z = mesh->_meshVertex[i]->_localPos.z;

			temp.Normal.x = mesh->_meshVertex[i]->_normal.x;
			temp.Normal.y = mesh->_meshVertex[i]->_normal.y;
			temp.Normal.z = mesh->_meshVertex[i]->_normal.z;

			vertices.emplace_back(temp);
		}

		ID3D11Buffer* VB;

		D3D11_BUFFER_DESC vbd{};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex::PosNormal) * (UINT)vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData{};
		vinitData.pSysMem = &vertices[0];
		HR(_resourceManager->GetDevice()->GetDevice()->CreateBuffer(&vbd, &vinitData, &VB));

		_VBs.push_back(VB);
	}

	else if (mesh->_hasTexture)
	{
		std::vector<Vertex::QuadVertex> vertices;
		Vertex::QuadVertex temp{};
		_vertexSizes.push_back(sizeof(Vertex::QuadVertex));
		_vertexType = VertexType::QuadVertex;

		for (UINT i = 0; i < mesh->_meshVertex.size(); i++)
		{
			temp.Pos.x = mesh->_meshVertex[i]->_localPos.x;
			temp.Pos.y = mesh->_meshVertex[i]->_localPos.y;
			temp.Pos.z = mesh->_meshVertex[i]->_localPos.z;

			temp.Tex.x = mesh->_meshVertex[i]->_u;
			temp.Tex.y = mesh->_meshVertex[i]->_v;

			vertices.emplace_back(temp);
		}

		ID3D11Buffer* VB;

		D3D11_BUFFER_DESC vbd{};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex::QuadVertex) * (UINT)vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData{};
		vinitData.pSysMem = &vertices[0];
		HR(_resourceManager->GetDevice()->GetDevice()->CreateBuffer(&vbd, &vinitData, &VB));

		_VBs.push_back(VB);
	}

	else if (mesh->_hasColor)
	{
		std::vector<Vertex::PosColor> vertices;
		Vertex::PosColor temp{};
		_vertexSizes.push_back(sizeof(Vertex::PosColor));
		_vertexType = VertexType::PosColor;

		for (UINT i = 0; i < mesh->_meshVertex.size(); i++)
		{
			temp.Pos.x = mesh->_meshVertex[i]->_localPos.x;
			temp.Pos.y = mesh->_meshVertex[i]->_localPos.y;
			temp.Pos.z = mesh->_meshVertex[i]->_localPos.z;

			temp.Color.x = mesh->_meshVertex[i]->_color.x;
			temp.Color.y = mesh->_meshVertex[i]->_color.y;
			temp.Color.z = mesh->_meshVertex[i]->_color.z;
			temp.Color.w = mesh->_meshVertex[i]->_color.w;

			vertices.emplace_back(temp);
		}

		ID3D11Buffer* VB;

		D3D11_BUFFER_DESC vbd{};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex::PosColor) * (UINT)vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData{};
		vinitData.pSysMem = &vertices[0];
		HR(_resourceManager->GetDevice()->GetDevice()->CreateBuffer(&vbd, &vinitData, &VB));

		_VBs.push_back(VB);
	}

	/// 인덱스 버퍼를 생성한다.
	std::vector<UINT> indices;
	for (UINT i = 0; i < mesh->_meshFace.size(); i++)
	{
		indices.emplace_back(mesh->_meshFace[i]->vertexIndex[0]);
		indices.emplace_back(mesh->_meshFace[i]->vertexIndex[1]);
		indices.emplace_back(mesh->_meshFace[i]->vertexIndex[2]);
	}

	_indexSizes.push_back((UINT)indices.size());

	ID3D11Buffer* IB;

	D3D11_BUFFER_DESC ibd{};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * (UINT)indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData{};
	iinitData.pSysMem = &indices[0];
	HR(_resourceManager->GetDevice()->GetDevice()->CreateBuffer(&ibd, &iinitData, &IB));

	_IBs.push_back(IB);

	//temp
	_indices.push_back(indices);
}

ImpMath::Matrix& GeometryBuffer::GetWorldTransform(UINT index)
{
	return _nodes[index]->_worldTransform;
}

void GeometryBuffer::AddAniamtion(const std::wstring& path,
    const std::wstring& animationName)
{
    size_t lastPoint = path.find_last_of(L".");
    size_t lastSlash = path.find_last_of(L"/");

    std::wstring fileName;

    const wchar_t* pathWchar = path.c_str();
    wchar_t* wchar           = const_cast<wchar_t*>(pathWchar);

    char* pStr;
    int strSize = WideCharToMultiByte(
        CP_ACP, 0, wchar, -1, NULL, 0, NULL, NULL);
    pStr = new char[strSize];
    WideCharToMultiByte(CP_ACP, 0, wchar, -1, pStr, strSize, 0, 0);

    _resourceManager->GetFBXLoader()->SetPath(pStr);

    ImpAnimationObject* temp = _resourceManager->GetFBXLoader()->GetAnimationObject();
    //_bones = _resourceManager->GetFBXLoader()->GetBoneList();
    _animationsBones[animationName] = _resourceManager->GetFBXLoader()->GetBoneList();
    // meshes = _resourceManager->GetFBXLoader()->GetMeshAll(pStr);

    _resourceManager->GetAniamtionPlayer()->AddObject(animationName, temp);

    delete[] pStr;
}

#pragma endregion VBIB

#pragma region Shader
Shader::Shader(ResourceManager* resourceManager)
	:Resource(resourceManager),
	_inputLayout(nullptr),
	_vertexShader(nullptr),
	_pixelShader(nullptr)
{

}

Shader::~Shader()
{												
    ReleaseCOM(_meshConstantBuffer);
    ReleaseCOM(_quadConstantBuffer);
    ReleaseCOM(_worldViewProjConstantBuffer);
    ReleaseCOM(_skinnedMeshConstantBuffer);
    ReleaseCOM(_downSamplingVSConstantBuffer);
    ReleaseCOM(_UIConstantBuffer);
    ReleaseCOM(_UIAlphaConstantBuffer);
    ReleaseCOM(_alphaConstantBuffer);
    ReleaseCOM(_bloomQuadConstantBuffer);
    ReleaseCOM(_emssionCoefficientConstantBuffer);

	ReleaseCOM(_pixelShader);
	ReleaseCOM(_vertexShader);
	ReleaseCOM(_inputLayout)
}

void Shader::Load(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, VertexType vertexType)
{
	// 일단 hlsl이든 cso가 들어오든 cso를 확인한다.
	// cso가 없을 경우 hlsl을 컴파일 해서 cso로 넣는다.

	// Debug에서는 무조건 hlsl을 컴파일해서 쓰고, hlsl이 없다면 Resource에서 cso를 찾는다
	// Release에서는 무조건 cso만 쓴다.

	DWORD shaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;
	shaderFlag |= D3DCOMPILE_DEBUG;
	shaderFlag |= D3DCOMPILE_SKIP_OPTIMIZATION;

	ID3D11Device* device = _resourceManager->GetDevice()->GetDevice();

	ID3D10Blob* error = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	bool isExistVSHLSL = true;
	if (FAILED(D3DCompileFromFile(vertexShaderPath.c_str(), nullptr, nullptr, "VS", "vs_5_0", shaderFlag, 0,
		&vertexShaderBuffer, &error)))
	{
		if (error)
		{
			//MessageBox(NULL, L"Vertex Shader 컴파일을 실패했습니다.", vertexShaderPath.c_str(), MB_ICONERROR);
			isExistVSHLSL = false;
		}
		else
		{
			//MessageBox(NULL, L"파일을 찾을 수 없습니다.", vertexShaderPath.c_str(), MB_ICONERROR);
			isExistVSHLSL = false;
		}
	}

	ID3D10Blob* pixelShaderBuffer = nullptr;
	bool isExistPSHLSL = true;
	if (FAILED(D3DCompileFromFile(pixelShaderPath.c_str(), nullptr, nullptr, "PS", "ps_5_0", shaderFlag, 0,
		&pixelShaderBuffer, &error)))
	{
		if (error)
		{
			//MessageBox(NULL, L"Pixel Shader 컴파일을 실패했습니다.", pixelShaderPath.c_str(), MB_ICONERROR);
			isExistPSHLSL = false;
		}
		else
		{
			//MessageBox(NULL, L"파일을 찾을 수 없습니다.", pixelShaderPath.c_str(), MB_ICONERROR);
			isExistPSHLSL = false;
		}
	}

	std::vector<char> compiledShaderVS;
	std::vector<char> compiledShaderPS;
	/// Create Vertex Shader
	if (!isExistVSHLSL)
	{
		MessageBox(NULL, L"Vertex Shader 컴파일을 실패했습니다 cso를 로드합니다.", vertexShaderPath.c_str(), MB_ICONERROR);

		size_t lastPoint = vertexShaderPath.find_last_of(L".");
		size_t lastSlash = vertexShaderPath.find_last_of(L"/");

		std::wstring fileName;
		if (lastPoint == std::string::npos)
		{
			fileName = vertexShaderPath;
		}
		else
		{
			fileName = vertexShaderPath.substr(lastSlash + 1, lastPoint - lastSlash - 1);
		}

		std::wstring filePath = L"Resources/Shader/cso/" + fileName + L".cso";
		std::ifstream fin(filePath, std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		UINT size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		compiledShaderVS.resize(size);

		fin.read(&compiledShaderVS[0], size);
		fin.close();

		HR(device->CreateVertexShader(&compiledShaderVS[0], size,
			nullptr, &_vertexShader));
	}
	else
	{
		device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(), nullptr, &_vertexShader);
	}
	/// Create Pixel Shader
	if (!isExistPSHLSL)
	{
		MessageBox(NULL, L"Pixel Shader 컴파일을 실패했습니다 cso를 로드합니다.", pixelShaderPath.c_str(), MB_ICONERROR);

		size_t lastPoint = pixelShaderPath.find_last_of(L".");
		size_t lastSlash = pixelShaderPath.find_last_of(L"/");

		std::wstring fileName;
		if (lastPoint == std::string::npos)
		{
			fileName = pixelShaderPath;
		}
		else
		{
			fileName = pixelShaderPath.substr(lastSlash + 1, lastPoint - lastSlash - 1);
		}

		std::wstring filePath = L"Resources/Shader/cso/" + fileName + L".cso";
		std::ifstream fin(filePath, std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		UINT size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		compiledShaderPS.resize(size);

		fin.read(&compiledShaderPS[0], size);
		fin.close();

		HR(device->CreatePixelShader(&compiledShaderPS[0], size,
			nullptr, &_pixelShader));
	}
	else
	{
		device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
			pixelShaderBuffer->GetBufferSize(), nullptr, &_pixelShader);
	}

	/// 아무래도 상수 버퍼를 여기서 만들면 안 될 거 같은데...?
		/// 일단 옮겼지만...
	D3D11_BUFFER_DESC constantBuffer{};
	constantBuffer.Usage = D3D11_USAGE_DYNAMIC;
	constantBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBuffer.MiscFlags = 0;
	constantBuffer.StructureByteStride = 0;

	/// Set Input Layout
	if (vertexType == VertexType::PosNormal)
	{
		if (isExistVSHLSL)
		{
			device->CreateInputLayout(InputLayoutDesc::PosNormal, 2, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &_inputLayout);
		}
		else
		{
			device->CreateInputLayout(InputLayoutDesc::PosNormal, 2, &compiledShaderVS[0],
				compiledShaderVS.size(), &_inputLayout);
		}

		constantBuffer.ByteWidth = sizeof(MeshConstantBuffer);
	}
	else if (vertexType == VertexType::MeshVertex)
	{
		if (isExistVSHLSL)
		{
			device->CreateInputLayout(InputLayoutDesc::MeshVertex, 4, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &_inputLayout);
		}
		else
		{
			device->CreateInputLayout(InputLayoutDesc::MeshVertex, 4, &compiledShaderVS[0],
				compiledShaderVS.size(), &_inputLayout);
		}

		constantBuffer.ByteWidth = sizeof(MeshConstantBuffer);
	}
	else if (vertexType == VertexType::QuadVertex)
	{
		if (isExistVSHLSL)
		{
			device->CreateInputLayout(InputLayoutDesc::QuadVertex, 3, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &_inputLayout);
		}
		else
		{
			device->CreateInputLayout(InputLayoutDesc::QuadVertex, 3, &compiledShaderVS[0],
				compiledShaderVS.size(), &_inputLayout);
		}

		constantBuffer.ByteWidth = sizeof(QuadConstantBuffer);
	}
	else if (vertexType == VertexType::PosColor)
	{
		if (isExistVSHLSL)
		{
			device->CreateInputLayout(InputLayoutDesc::PosColor, 2, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &_inputLayout);
		}
		else
		{
			device->CreateInputLayout(InputLayoutDesc::PosColor, 2, &compiledShaderVS[0],
				compiledShaderVS.size(), &_inputLayout);
		}

		constantBuffer.ByteWidth = sizeof(WorldViewProjConstantBuffer);
	}
	else if (vertexType == VertexType::PosColorTex)
	{
		if (isExistVSHLSL)
		{
			device->CreateInputLayout(InputLayoutDesc::PosColorTex, 3, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &_inputLayout);
		}
		else
		{
			device->CreateInputLayout(InputLayoutDesc::PosColorTex, 3, &compiledShaderVS[0],
				compiledShaderVS.size(), &_inputLayout);
		}

		constantBuffer.ByteWidth = sizeof(MeshConstantBuffer);
	}
	else if (vertexType == VertexType::SkinnedVertex)
	{
		if (isExistVSHLSL)
		{
			device->CreateInputLayout(InputLayoutDesc::SkinnedVertex, 6, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &_inputLayout);
		}
		else
		{
			device->CreateInputLayout(InputLayoutDesc::SkinnedVertex, 6, &compiledShaderVS[0],
				compiledShaderVS.size(), &_inputLayout);
		}

		constantBuffer.ByteWidth = sizeof(SkinnedMeshConstantBuffer);
	}

	ReleaseCOM(vertexShaderBuffer);
	ReleaseCOM(pixelShaderBuffer);

	// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 쉐이더 상수 버퍼에 접근할 수 있게 한다.
    constantBuffer.ByteWidth = sizeof(MeshConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_meshConstantBuffer);

	constantBuffer.ByteWidth = sizeof(QuadConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_quadConstantBuffer);

	constantBuffer.ByteWidth = sizeof(WorldViewProjConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_worldViewProjConstantBuffer);

	constantBuffer.ByteWidth = sizeof(SkinnedMeshConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_skinnedMeshConstantBuffer);

	constantBuffer.ByteWidth = sizeof(DownSamplingVSConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_downSamplingVSConstantBuffer);

	constantBuffer.ByteWidth = sizeof(UIConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_UIConstantBuffer);

	constantBuffer.ByteWidth = sizeof(UIAlphaConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_UIAlphaConstantBuffer);

	constantBuffer.ByteWidth = sizeof(AlphaConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_alphaConstantBuffer);

	constantBuffer.ByteWidth = sizeof(bloomQuadConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_bloomQuadConstantBuffer);

	constantBuffer.ByteWidth = sizeof(EmissionCoefficientConstantBuffer);
    device->CreateBuffer(&constantBuffer, nullptr, &_emssionCoefficientConstantBuffer);
}

void Shader::SetConstantBuffer(MeshConstantBuffer constantBufferData)
{
	ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

	XMStoreFloat4x4(&constantBufferData.transform, XMMatrixTranspose(XMLoadFloat4x4(&constantBufferData.transform)));
	XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(XMLoadFloat4x4(&constantBufferData.world)));

	// 상수 버퍼의 내용을 쓸 수 있도록 잠근다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(_meshConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	MeshConstantBuffer* dataPtr = reinterpret_cast<MeshConstantBuffer*>(mappedResource.pData);

	// 상수 버퍼에 행렬을 복사한다.
	//XMStoreFloat4x4(&dataPtr->transform, transformMatrix.transform);
	//XMStoreFloat4x4(&dataPtr->worldInvTranspose, transformMatrix.worldInvTranspose);

	dataPtr->world = constantBufferData.world;
	dataPtr->transform = constantBufferData.transform;
	dataPtr->worldInvTranspose = constantBufferData.worldInvTranspose;

	// 상수 버퍼의 잠금을 푼다.
    deviceContext->Unmap(_meshConstantBuffer, 0);

	// 정점 쉐이더에서의 상수 버퍼의 위치를 설정한다.
	unsigned bufferOffset = 0;

	// 마지막으로 정점 쉐이더의 상수 버퍼를 바뀐 값으로 바꾼다.
    deviceContext->VSSetConstantBuffers(bufferOffset, 1, &_meshConstantBuffer);
	//deviceContext->PSSetConstantBuffers(bufferOffset, 1, &_constantBuffer);
}

void Shader::SetConstantBuffer(QuadConstantBuffer constantBufferData)
{
	ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

	// 상수 버퍼의 내용을 쓸 수 있도록 잠근다. 
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(_quadConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	QuadConstantBuffer* dataPtr = reinterpret_cast<QuadConstantBuffer*>(mappedResource.pData);

	// 상수 버퍼에 행렬을 복사한다.
	//XMStoreFloat4x4(&dataPtr->transform, transformMatrix.transform);
	//XMStoreFloat4x4(&dataPtr->worldInvTranspose, transformMatrix.worldInvTranspose);

	for (int i = 0; i < 3; i++)
	{
        dataPtr->directionalData[i] = constantBufferData.directionalData[i];
	}
    for (int i = 0; i < 30; i++)
    {
        dataPtr->pointData[i] = constantBufferData.pointData[i];
    }
    for (int i = 0; i < 10; i++)
    {
        dataPtr->spotData[i] = constantBufferData.spotData[i];
    }
	dataPtr->eyePos = constantBufferData.eyePos;
    dataPtr->numOfDirectionalLight = constantBufferData.numOfDirectionalLight;
    dataPtr->numOfPointLight       = constantBufferData.numOfPointLight;
    dataPtr->numOfSpotLight        = constantBufferData.numOfSpotLight;
    dataPtr->globalEmissionCoefficient        = constantBufferData.globalEmissionCoefficient;
    dataPtr->objectEmissionCoefficient = constantBufferData.objectEmissionCoefficient;
    dataPtr->one = DirectX::XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };

	// 상수 버퍼의 잠금을 푼다.
    deviceContext->Unmap(_quadConstantBuffer, 0);

	//deviceContext->VSSetConstantBuffers(bufferOffset, 1, &_constantBuffer);
    deviceContext->PSSetConstantBuffers(0, 1, &_quadConstantBuffer);
}

void Shader::SetConstantBuffer(WorldViewProjConstantBuffer constantBufferData)
{
	ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

	XMStoreFloat4x4(&constantBufferData.worldViewProj, XMMatrixTranspose(XMLoadFloat4x4(&constantBufferData.worldViewProj)));

	// 상수 버퍼의 내용을 쓸 수 있도록 잠근다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(_worldViewProjConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	WorldViewProjConstantBuffer* dataPtr = reinterpret_cast<WorldViewProjConstantBuffer*>(mappedResource.pData);

	// 상수 버퍼에 행렬을 복사한다.
	//XMStoreFloat4x4(&dataPtr->transform, transformMatrix.transform);
	//XMStoreFloat4x4(&dataPtr->worldInvTranspose, transformMatrix.worldInvTranspose);

	dataPtr->worldViewProj = constantBufferData.worldViewProj;
	dataPtr->color = constantBufferData.color;

	// 상수 버퍼의 잠금을 푼다.
    deviceContext->Unmap(_worldViewProjConstantBuffer, 0);

	// 정점 쉐이더에서의 상수 버퍼의 위치를 설정한다.
	unsigned bufferOffset = 0;

	// 마지막으로 정점 쉐이더의 상수 버퍼를 바뀐 값으로 바꾼다.
	//deviceContext->VSSetConstantBuffers(bufferOffset, 1, &_constantBuffer);
	deviceContext->VSSetConstantBuffers(bufferOffset, 1, &_worldViewProjConstantBuffer);
}

void Shader::SetConstantBuffer(SkinnedMeshConstantBuffer constantBufferData)
{
	ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

	XMStoreFloat4x4(&constantBufferData.transform, XMMatrixTranspose(XMLoadFloat4x4(&constantBufferData.transform)));
	XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(XMLoadFloat4x4(&constantBufferData.world)));

	for (UINT i = 0; i < 100; i++)
	{
		XMStoreFloat4x4(&constantBufferData.boneTransform[i], XMMatrixTranspose(XMLoadFloat4x4(&constantBufferData.boneTransform[i])));
		XMStoreFloat4x4(&constantBufferData.preBoneTransform[i], XMMatrixTranspose(XMLoadFloat4x4(&constantBufferData.preBoneTransform[i])));
	}

	// 상수 버퍼의 내용을 쓸 수 있도록 잠근다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(_skinnedMeshConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	SkinnedMeshConstantBuffer* dataPtr = reinterpret_cast<SkinnedMeshConstantBuffer*>(mappedResource.pData);

	// 상수 버퍼에 행렬을 복사한다.
	dataPtr->world = constantBufferData.world;
	dataPtr->transform = constantBufferData.transform;
	dataPtr->worldInvTranspose = constantBufferData.worldInvTranspose;

	// temp
	for (UINT i = 0; i < 100; i++)
	{
		dataPtr->boneTransform[i] = constantBufferData.boneTransform[i];
        dataPtr->preBoneTransform[i] = constantBufferData.preBoneTransform[i];
	}
    dataPtr->changingCount = constantBufferData.changingCount;
    dataPtr->maxChanging   = constantBufferData.maxChanging;
    dataPtr->isOnInterpolation = constantBufferData.isOnInterpolation;

	// 상수 버퍼의 잠금을 푼다.
    deviceContext->Unmap(_skinnedMeshConstantBuffer, 0);

	// 정점 쉐이더에서의 상수 버퍼의 위치를 설정한다.
	unsigned bufferOffset = 0;

	// 마지막으로 정점 쉐이더의 상수 버퍼를 바뀐 값으로 바꾼다.
	deviceContext->VSSetConstantBuffers(bufferOffset, 1, &_skinnedMeshConstantBuffer);
	//deviceContext->PSSetConstantBuffers(bufferOffset, 1, &_constantBuffer);
}

void Shader::SetConstantBuffer(DownSamplingVSConstantBuffer constantBufferData)
{
    ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

    //  상수 버퍼의 내용을 쓸 수 있도록 잠근다.
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    deviceContext->Map(_downSamplingVSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    //  상수 버퍼의 데이터에 대한 포인터를 가져온다.
    DownSamplingVSConstantBuffer* dataPtr = reinterpret_cast<DownSamplingVSConstantBuffer*>(
        mappedResource.pData);

    //  상수 버퍼에 행렬을 복사한다.
    // XMStoreFloat4x4(&dataPtr->transform, transformMatrix.transform);
    // XMStoreFloat4x4(&dataPtr->worldInvTranspose, transformMatrix.worldInvTranspose);

    dataPtr->screenSize = constantBufferData.screenSize;

    //  상수 버퍼의 잠금을 푼다.
    deviceContext->Unmap(_downSamplingVSConstantBuffer, 0);

    //  정점 쉐이더에서의 상수 버퍼의 위치를 설정한다.
    unsigned bufferOffset = 0;

    //  마지막으로 정점 쉐이더의 상수 버퍼를 바뀐 값으로 바꾼다.
    // deviceContext->VSSetConstantBuffers(bufferOffset, 1, &_constantBuffer);
    deviceContext->VSSetConstantBuffers(bufferOffset, 1, &_downSamplingVSConstantBuffer);
}

void Shader::SetConstantBuffer(UIConstantBuffer constantBufferData)
{
    ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

    //  상수 버퍼의 내용을 쓸 수 있도록 잠근다.
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    deviceContext->Map(_UIConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    //  상수 버퍼의 데이터에 대한 포인터를 가져온다.
    UIConstantBuffer* dataPtr = reinterpret_cast<UIConstantBuffer*>(
        mappedResource.pData);

    dataPtr->x = constantBufferData.x;
    dataPtr->y = constantBufferData.y;
    dataPtr->ratio = constantBufferData.ratio;

    //  상수 버퍼의 잠금을 푼다.
    deviceContext->Unmap(_UIConstantBuffer, 0);

    //  정점 쉐이더에서의 상수 버퍼의 위치를 설정한다.
    unsigned bufferOffset = 0;

    //  마지막으로 정점 쉐이더의 상수 버퍼를 바뀐 값으로 바꾼다.
    // deviceContext->VSSetConstantBuffers(bufferOffset, 1, &_constantBuffer);
    deviceContext->VSSetConstantBuffers(bufferOffset, 1, &_UIConstantBuffer);
}

void Shader::SetConstantBuffer(UIAlphaConstantBuffer constantBufferData)
{
    ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    deviceContext->Map(_UIAlphaConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    UIAlphaConstantBuffer* dataPtr = reinterpret_cast<UIAlphaConstantBuffer*>(
        mappedResource.pData);

    dataPtr->alpha = constantBufferData.alpha;
    dataPtr->fStregth = constantBufferData.fStregth;
    dataPtr->uvDir    = constantBufferData.uvDir;

	dataPtr->isNoise = constantBufferData.isNoise;
    dataPtr->isPlus = constantBufferData.isPlus;
    dataPtr->xRatio  = constantBufferData.xRatio;

    deviceContext->Unmap(_UIAlphaConstantBuffer, 0);

    deviceContext->PSSetConstantBuffers(0, 1, &_UIAlphaConstantBuffer);
}

void Shader::SetConstantBuffer(AlphaConstantBuffer constantBufferData)
{
    ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    deviceContext->Map(_alphaConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    AlphaConstantBuffer* dataPtr = reinterpret_cast<AlphaConstantBuffer*>(
        mappedResource.pData);

    dataPtr->alpha = constantBufferData.alpha;

    deviceContext->Unmap(_alphaConstantBuffer, 0);

    deviceContext->PSSetConstantBuffers(1, 1, &_alphaConstantBuffer);
}

void Shader::SetConstantBuffer(bloomQuadConstantBuffer constantBufferData)
{
    ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    deviceContext->Map(_bloomQuadConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    bloomQuadConstantBuffer* dataPtr = reinterpret_cast<bloomQuadConstantBuffer*>(
        mappedResource.pData);

    //dataPtr->uvDir    = DirectX::XMFLOAT2{0.3f,0.3f};

    //  상수 버퍼의 잠금을 푼다.
    deviceContext->Unmap(_bloomQuadConstantBuffer, 0);

    //  정점 쉐이더에서의 상수 버퍼의 위치를 설정한다.
    unsigned bufferOffset = 0;

    //  마지막으로 정점 쉐이더의 상수 버퍼를 바뀐 값으로 바꾼다.
    deviceContext->PSSetConstantBuffers(0, 1, &_bloomQuadConstantBuffer);
}

void Shader::SetConstantBuffer(EmissionCoefficientConstantBuffer constantBufferData)
{
    ID3D11DeviceContext* deviceContext = _resourceManager->GetDevice()->GetDeviceContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    deviceContext->Map(_emssionCoefficientConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    EmissionCoefficientConstantBuffer* dataPtr = reinterpret_cast<EmissionCoefficientConstantBuffer*>(
        mappedResource.pData);

    dataPtr->objectEmissionCoefficient = constantBufferData.objectEmissionCoefficient;

    deviceContext->Unmap(_emssionCoefficientConstantBuffer, 0);

    deviceContext->PSSetConstantBuffers(2, 1, &_emssionCoefficientConstantBuffer);
}

#pragma endregion Shader

Texture::Texture(ResourceManager* resourceManager)
	:Resource(resourceManager),
	_textureSRV(nullptr)
{
}

Texture::~Texture()
{
            	_textureSRV->Release();
}

void Texture::Load(const std::wstring& texturePath)
{
	std::wstring fileExtension = texturePath.substr(texturePath.find_last_of(L".") + 1, texturePath.length() - texturePath.find_last_of(L".") + 1);

	if (fileExtension == L"dds")
	{
		HR(CreateDDSTextureFromFile(_resourceManager->GetDevice()->GetDevice(),
		_resourceManager->GetDevice()->GetDeviceContext(),
		texturePath.c_str(),
		nullptr, &_textureSRV));
	}
	else
	{
		HR(CreateWICTextureFromFile(_resourceManager->GetDevice()->GetDevice(),
		_resourceManager->GetDevice()->GetDeviceContext(),
		texturePath.c_str(),
		nullptr, &_textureSRV));
	}
}