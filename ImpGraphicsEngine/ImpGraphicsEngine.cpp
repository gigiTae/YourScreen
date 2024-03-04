#include "pch.h"
#include "ImpGraphicsEngine.h"

#include "ImpD3D11Renderer.h"

ImpGraphics::ImpGraphicsEngine::ImpGraphicsEngine()
	:_renderer(nullptr)
{
	_renderer = new ImpGraphics::ImpD3D11Renderer;
}

ImpGraphics::ImpGraphicsEngine::~ImpGraphicsEngine()
{
	delete _renderer;
}

bool ImpGraphics::ImpGraphicsEngine::Initialize(int hinst, int hWnd, int screenWidth, int screenHeight)
{
	_renderer->Initialize(hinst, hWnd, screenWidth, screenHeight);

	return true;
}

void ImpGraphics::ImpGraphicsEngine::Update(float deltaTime)
{
	_renderer->Update(deltaTime);
}

void ImpGraphics::ImpGraphicsEngine::BeginRender()
{
	_renderer->BeginRender();
}

void ImpGraphics::ImpGraphicsEngine::Render(RendererType rendererType)
{
	_renderer->Render();
}

void ImpGraphics::ImpGraphicsEngine::EndRender()
{
	_renderer->EndRender();
}

void ImpGraphics::ImpGraphicsEngine::Finalize()
{
	_renderer->Finalize();
}

void ImpGraphics::ImpGraphicsEngine::SetClientSize(int width, int height)
{
	_renderer->SetClientSize(width, height);
}

void ImpGraphics::ImpGraphicsEngine::UpdateCamera(Matrix transformMatrix)
{
	_renderer->UpdateCamera(transformMatrix);
}

void ImpGraphics::ImpGraphicsEngine::SetCamera(CameraInfo cameraInfo)
{
	_renderer->SetCamera(cameraInfo);
}

void ImpGraphics::ImpGraphicsEngine::Set2ndCamera(CameraInfo cameraInfo)
{
    _renderer->Set2ndCamera(cameraInfo);
}

void ImpGraphics::ImpGraphicsEngine::Update2ndCamera(Matrix transformMatrix)
{
    _renderer->Update2ndCamera(transformMatrix);
}

void ImpGraphics::ImpGraphicsEngine::Has2ndCamera(bool has2ndCamera)
{
    _renderer->Has2ndCamera(has2ndCamera);
}

void ImpGraphics::ImpGraphicsEngine::UpdateRight(float deltaTime)
{
    _renderer->UpdateRight(deltaTime);
}

void ImpGraphics::ImpGraphicsEngine::BeginRenderRight()
{
    _renderer->BeginRenderRight();
}

void ImpGraphics::ImpGraphicsEngine::RenderRight(RendererType rendererType)
{
    _renderer->RenderRight();
}

void ImpGraphics::ImpGraphicsEngine::AddLight(LightInfo lightInfo)
{
	_renderer->AddLight(lightInfo);
}

void ImpGraphics::ImpGraphicsEngine::UpdateLight(size_t lightID, Matrix transformMatrix)
{
	_renderer->UpdateLight(lightID, transformMatrix);
}

void ImpGraphics::ImpGraphicsEngine::SetLight(size_t lightID, LightInfo lightInfo)
{
	_renderer->SetLight(lightID, lightInfo);
}

void ImpGraphics::ImpGraphicsEngine::DeleteLight(size_t lightID)
{
	_renderer->DeleteLight(lightID);
}

void ImpGraphics::ImpGraphicsEngine::AddMeshObejct(MeshObjectInfo meshObjectInfo)
{
	_renderer->AddMeshObejct(meshObjectInfo);
}

void ImpGraphics::ImpGraphicsEngine::UpdateMeshObject(size_t objectID, Matrix transformMatrix)
{
    _renderer->UpdateMeshObject(objectID, transformMatrix);
}

void ImpGraphics::ImpGraphicsEngine::DeleteMeshObject(size_t objectID)
{
	_renderer->DeleteMeshObject(objectID);
}

void ImpGraphics::ImpGraphicsEngine::AddSkeletalAnimationObject(MeshObjectInfo meshObjectInfo)
{
    _renderer->AddSkeletalAnimationObject(meshObjectInfo);
}

void ImpGraphics::ImpGraphicsEngine::AddSkeletalAnimation(size_t objectID,
    std::wstring fbx)
{
    _renderer->AddSkeletalAnimation(objectID, fbx);
}

void ImpGraphics::ImpGraphicsEngine::PlaySkeletalAnimation(size_t objectID,
    std::wstring fbx)
{
    _renderer->PlaySkeletalAnimation(objectID, fbx);
}

void ImpGraphics::ImpGraphicsEngine::UpdateSkeletalAnimationSpeed(size_t objectID,
    std::wstring fbx,
    float speed)
{
    _renderer->UpdateSkeletalAnimationSpeed(objectID, fbx, speed);
}

void ImpGraphics::ImpGraphicsEngine::SetDefaultSkeltalAnimation(size_t objectID,
    std::wstring fbx)
{
    _renderer->SetDefaultSkeltalAnimation(objectID, fbx);
}

void ImpGraphics::ImpGraphicsEngine::PlaySkeletalAnimationNonLoop(size_t objectID,
    std::wstring fbx)
{
    _renderer->PlaySkeletalAnimationNonLoop(objectID, fbx);
}

std::wstring ImpGraphics::ImpGraphicsEngine::GetCurAnimation(size_t objectID)
{
    return _renderer->GetCurAnimation(objectID);
}

void ImpGraphics::ImpGraphicsEngine::SetInterpolation(size_t objectID, bool onOff)
{
    _renderer->SetInterpolation(objectID, onOff);
}

void ImpGraphics::ImpGraphicsEngine::SetTransparentObjectAlpha(size_t objectID,
    float customAlpha)
{
    _renderer->SetTransparentObjectAlpha(objectID, customAlpha);
}

void ImpGraphics::ImpGraphicsEngine::SetWorldEmissionCoefficient(float emission)
{
    _renderer->SetWorldEmissionCoefficient(emission);
}

void ImpGraphics::ImpGraphicsEngine::SetObjectEmissionCoefficient(size_t objectID, float emission)
{
    _renderer->SetObjectEmissionCoefficient(objectID, emission);
}

void ImpGraphics::ImpGraphicsEngine::AddCubeBoundingVolume(size_t _boundingVolumeID, Vector3 length)
{
	_renderer->AddCubeBoundingVolume(_boundingVolumeID, length);
}

void ImpGraphics::ImpGraphicsEngine::AddSphereBoundingVolume(size_t _boundingVolumeID, float radius)
{
	_renderer->AddSphereBoundingVolume(_boundingVolumeID, radius);
}

void ImpGraphics::ImpGraphicsEngine::UpdateBoundingVolume(size_t boundingVolumeID, Matrix transformMatrix, Vector4 color)
{
	_renderer->UpdateBoundingVolume(boundingVolumeID, transformMatrix, color);
}

void ImpGraphics::ImpGraphicsEngine::DeleteBoundingVolume(size_t boundingVolumeID)
{
	_renderer->DeleteBoundingVolume(boundingVolumeID);
}

size_t ImpGraphics::ImpGraphicsEngine::GetPickingObject(long mouseX, long mouseY)
{
	return _renderer->GetPickingObject(mouseX, mouseY);
}

void ImpGraphics::ImpGraphicsEngine::SetGrid(bool isOn)
{
	_renderer->SetGrid(isOn);
}

void ImpGraphics::ImpGraphicsEngine::SetAxis(bool isOn)
{
	_renderer->SetAxis(isOn);
}

void ImpGraphics::ImpGraphicsEngine::SetWireFrame(bool isOn)
{
	_renderer->SetWireFrame(isOn);
}

void ImpGraphics::ImpGraphicsEngine::SetSkyBox(std::wstring cubeMapPath)
{
	_renderer->SetSkyBox(cubeMapPath);
}

void ImpGraphics::ImpGraphicsEngine::AddUI(size_t uiID, UIInfo uiInfo)
{
    _renderer->AddUI(uiID, uiInfo);
}

void ImpGraphics::ImpGraphicsEngine::DeleteUI(size_t uiID)
{
    _renderer->DeleteUI(uiID);
}

void ImpGraphics::ImpGraphicsEngine::UpdateUI(size_t uiID, float x, float y, float alpha)
{
    _renderer->UpdateUI(uiID, x, y, alpha);
}

void ImpGraphics::ImpGraphicsEngine::SetUI(size_t uiID, UIInfo uiInfo)
{
    _renderer->SetUI(uiID, uiInfo);
}

void ImpGraphics::ImpGraphicsEngine::SetUILayer(size_t uiID, size_t layer)
{
    _renderer->SetUILayer(uiID, layer);
}

void ImpGraphics::ImpGraphicsEngine::RenderUI()
{
    _renderer->RenderUI();
}

void ImpGraphics::ImpGraphicsEngine::SetUINoise(size_t uiID, bool isOn)
{
    _renderer->SetUINoise(uiID, isOn);
}

void ImpGraphics::ImpGraphicsEngine::SetUIXRatio(size_t uiID, float ratio)
{
    _renderer->SetUIXRatio(uiID, ratio);
}

void* ImpGraphics::ImpGraphicsEngine::GetDevice()
{
	return _renderer->GetDevice();
}

void* ImpGraphics::ImpGraphicsEngine::GetDeviceContext()
{
	return _renderer->GetDeviceContext();
}

ImpMath::Vector3 ImpGraphics::ImpGraphicsEngine::GetBoundingBoxLength(std::wstring meshPath, Vector3& offset)
{
    return _renderer->GetBoundingBoxLength(meshPath, offset);
}