#include "pch.h"
#include "Builder.h"
#include "IImpGraphicsEngine.h"

#include "Material.h"

using namespace ImpGraphics;

ImpGraphics::SkyBoxObject* Builder::CreateSkyBoxObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath, std::wstring texturePath)
{
	std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(meshPath);
	std::shared_ptr<Texture> texture = _resourceManager->GetResource<Texture>(texturePath);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(vertexShaderPath, pixelShaderPath, VertexType::PosNormal);

	SkyBoxObject* skyBoxObject = new SkyBoxObject(vbib, shader, texture);

	return skyBoxObject;
}

ImpGraphics::MeshObject* Builder::CreateMeshObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath, std::wstring texturePath)
{
	std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(meshPath);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(vertexShaderPath, pixelShaderPath, VertexType::MeshVertex);

	MeshObject* meshObject;
	if (texturePath == L"")
	{
		meshObject = new MeshObject(vbib, shader);
	}
	else
	{
		std::shared_ptr<Texture> texture = _resourceManager->GetResource<Texture>(texturePath);
		meshObject = new MeshObject(vbib, shader, texture);
	}

	return meshObject;
}

ImpGraphics::PBRMeshObject* Builder::CreatePBRMeshObject(
    std::wstring meshPath,
    std::wstring vertexShaderPath,
    std::wstring pixelShaderPath,
    MaterialPath& material)
{
    std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(meshPath);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(vertexShaderPath, pixelShaderPath, VertexType::MeshVertex);

	PBRMeshObject* meshObject;

    Material* metarial = new Material(_resourceManager, material);
    meshObject         = new PBRMeshObject(vbib, shader, metarial);

	return meshObject;
}

ImpGraphics::LineListObject* Builder::CreateLineListObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
{
	std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(meshPath);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(vertexShaderPath, pixelShaderPath, VertexType::PosColor);

	LineListObject* lineListObject = new LineListObject(vbib, shader);

	return lineListObject;
}

ImpGraphics::QuadObject* Builder::CreateQuadObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath)
{
	std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(meshPath);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(vertexShaderPath, pixelShaderPath, VertexType::QuadVertex);
	std::shared_ptr<Texture> texture = _resourceManager->GetResource<Texture>(L"Resources/Texture/CubeMap/testIrr2.dds");

	QuadObject* quadObejct = new QuadObject(vbib, shader, texture);

	return quadObejct;
}

ImpGraphics::AnimationObject* Builder::CreateAnimationObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath, std::wstring texturePath /*= L""*/)
{
	std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(meshPath);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(vertexShaderPath, pixelShaderPath, VertexType::MeshVertex);

	AnimationObject* meshObject;
	if (texturePath == L"")
	{
		meshObject = new AnimationObject(vbib, shader);
	}
	else
	{
		std::shared_ptr<Texture> texture = _resourceManager->GetResource<Texture>(texturePath);
		meshObject = new AnimationObject(vbib, shader, texture);
	}

	return meshObject;
}

ImpGraphics::BoundingVolumeObject* Builder::CreateBoundingVolumeObject(Vector3 center, Vector3 length)
{
	BoundingVolumeObject* boundingVolumeObject = nullptr;

	std::wstring lengthString = std::to_wstring(length.x) +
                                std::to_wstring(length.y) +
                                std::to_wstring(length.z);
    std::shared_ptr<GeometryBuffer> geometryBuffer = _resourceManager->GetResource<
        GeometryBuffer>(lengthString + L".cube", center, length);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(L"../ImpGraphicsEngine/FPBoundingVolumeVS.hlsl", L"../ImpGraphicsEngine/FPDefaultPS.hlsl", VertexType::PosColor);

	return new BoundingVolumeObject(geometryBuffer, shader);
}

ImpGraphics::BoundingVolumeObject* Builder::CreateBoundingVolumeObject(Vector3 center, float radius)
{
	BoundingVolumeObject* boundingVolumeObject = nullptr;

	std::shared_ptr<GeometryBuffer> geometryBuffer = _resourceManager->GetResource<
        GeometryBuffer>(std::to_wstring(radius) + L".sphere", center, radius);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(L"../ImpGraphicsEngine/FPBoundingVolumeVS.hlsl", L"../ImpGraphicsEngine/FPDefaultPS.hlsl", VertexType::PosColor);

	return new BoundingVolumeObject(geometryBuffer, shader);
}

ImpGraphics::SkinnedMeshObject* Builder::CreateSkinnedMeshObject(
    std::wstring meshPath,
    std::wstring vertexShaderPath,
    std::wstring pixelShaderPath,
    MaterialPath& material,
	size_t objectID)
{
	std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(meshPath, std::to_wstring(objectID) + meshPath);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(vertexShaderPath, pixelShaderPath, VertexType::SkinnedVertex);

	SkinnedMeshObject* meshObject;
    Material* metarial = new Material(_resourceManager, material);
    meshObject = new SkinnedMeshObject(vbib, shader, metarial);

	return meshObject;
}

ImpGraphics::BloomQuadObject* Builder::CreateBloomQuadObject(
    std::wstring meshPath,
    std::wstring vertexShaderPath,
    std::wstring pixelShaderPath)
{
    std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(
        meshPath);
    std::shared_ptr<Shader> shader = _resourceManager->GetResource<
        Shader>(vertexShaderPath, pixelShaderPath, VertexType::QuadVertex);
    std::shared_ptr<Texture> texture = _resourceManager->GetResource<Texture>(
        L"Resources/Texture/Test/noise.jpeg");
    BloomQuadObject* quadObejct = new BloomQuadObject(vbib, shader, texture);

    return quadObejct;
}

ImpGraphics::UIObject* Builder::CreateUIObject(Vector4 uiPos,
                                               DirectX::XMFLOAT2 screenSize,
                                               size_t layer, std::wstring texturePath)
{
	UIObject* boundingVolumeObject = nullptr;

	uiPos.x                   = 0;
	uiPos.y                   = 0;
    std::wstring lengthString = std::to_wstring(screenSize.x) +
                                std::to_wstring(screenSize.y) +
                                std::to_wstring(uiPos.x) + std::to_wstring(uiPos.y) +
                                std::to_wstring(uiPos.z) + std::to_wstring(uiPos.w) + texturePath;

	std::shared_ptr<GeometryBuffer> geometryBuffer = _resourceManager->GetResource<GeometryBuffer>(lengthString + L".quad", uiPos, screenSize);
	std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(L"../ImpGraphicsEngine/UIVS.hlsl", L"../ImpGraphicsEngine/UIPS.hlsl", VertexType::QuadVertex);
    std::shared_ptr<Texture> texture = _resourceManager->GetResource<Texture>(texturePath);

	return new UIObject(geometryBuffer, shader, texture, layer);
}

ImpGraphics::ForwardObject* Builder::CreateForwardObject(std::wstring meshPath,
    std::wstring vertexShaderPath,
    std::wstring pixelShaderPath,
    MaterialPath& material)
{
    std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(
        meshPath);
    std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(
        vertexShaderPath, pixelShaderPath, VertexType::MeshVertex);

    ForwardObject* meshObject;

    Material* metarial = new Material(_resourceManager, material);
    meshObject         = new ForwardObject(vbib, shader, metarial);

    return meshObject;
}

ImpGraphics::ForwardOITQuadObject* Builder::CreateForwardOITQuadObject(
    std::wstring meshPath,
    std::wstring vertexShaderPath,
    std::wstring pixelShaderPath)
{
    std::shared_ptr<GeometryBuffer> vbib = _resourceManager->GetResource<GeometryBuffer>(
        meshPath);
    std::shared_ptr<Shader> shader = _resourceManager->GetResource<Shader>(
        vertexShaderPath, pixelShaderPath, VertexType::QuadVertex);

    ForwardOITQuadObject* quadObejct = new ForwardOITQuadObject(vbib, shader);

    return quadObejct;
}
