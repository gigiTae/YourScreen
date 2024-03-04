#include "pch.h"
#include "ImpD3D11Renderer.h"

#include "Define.h"

#include "ImpDevice.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "ImpFont.h"

#include "ImpCamera.h"

#include "IObject.h"
#include "MeshObject.h"
#include "Factory.h"
#include "ResourceManager.h"

// temp, todo: 팩토리에 라이트 만드는 함수도 넣어야 겠다
// temp
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "ImpAnimationPlayer.h"

// temp
#include "PickingManager.h"
#include "CullingManager.h"

#include "CollisionPrimitive.h"

#include <algorithm>

using namespace ImpGraphics;

// temp
using namespace DirectX;

ImpD3D11Renderer::ImpD3D11Renderer()
	: _hWnd(0),
	_screenWidth(0), _screenHeight(0),
	_device(nullptr), _renderTarget(nullptr),
	_camera(nullptr), _factory(nullptr)
    , _resourceManager(nullptr)
    , _quadLeftObjectForDeferred(nullptr)
    , _quadRightObjectForDeferred(nullptr)
    , _opaqueObjects()
    ,_GridObject(nullptr), _AxisObject(nullptr),
	_isOnGrid(false), _isOnAxis(false), _isOnWireFrame(false), _animationPlayer(nullptr)
    , _has2ndCamera(false)
    , _globalEmissionCoefficient(4)
{
	_device = new ImpDevice;
	_renderTarget = new ImpRenderTarget(_device);
	_renderState = new ImpRenderState;

	_font = new ImpFont;
}

ImpD3D11Renderer::~ImpD3D11Renderer()
{
	delete _opaqueObjects[-4];
	delete _opaqueObjects[-3];
	delete _opaqueObjects[-2];
	delete _opaqueObjects[-1];
    delete _testForward;
    delete _forwardOITQuadLeftObject;
    delete _forwardOITQuadRightObject;

	delete _boundingVolumeObjects[-2];

	delete _pickingManager;
    delete _cullingManager;

	delete _AxisObject;
	delete _GridObject;
    delete _quadLeftObjectForDeferred;
    delete _quadRightObjectForDeferred;
    delete _bloomPassQuadObject;

	delete _factory;
	delete _resourceManager;
	delete _animationPlayer;
	
	delete _camera;
	delete _cameraRight;

	delete _font;

	delete _renderState;
	delete _renderTarget;
	delete _device;
}

bool ImpD3D11Renderer::Initialize(int hinst, int hWnd, int screenWidth, int screenHeight)
{
#pragma warning(disable : 4312)
	_hWnd = reinterpret_cast<HWND>(hWnd);
#pragma warning(default : 4312)
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_device->Initialize(hWnd, screenWidth, screenHeight);
	_renderTarget->Initialize(screenWidth, screenHeight);
	_renderState->Initialize(_device->GetDevice());

	_font->Initialize(_device, _renderState);

	if (_has2ndCamera)
	{
        _camera = new ImpCamera(screenWidth / 2, screenHeight);
        _cameraRight = new ImpCamera(screenWidth / 2, screenHeight);
	}
	else
	{
        _camera      = new ImpCamera(screenWidth, screenHeight);
        _cameraRight = new ImpCamera(screenWidth / 2, screenHeight);
	}
	
	_animationPlayer = new ImpAnimationPlayer;
	_resourceManager = new ResourceManager(_device, _animationPlayer);
	// temp

	_pickingManager = new PickingManager(_camera, screenWidth, screenHeight);
    _cullingManager = new CullingManager();

	_factory = new Factory(_resourceManager);
	
	_AxisObject = _factory->CreateIObject<LineListObject>(L"Resources/Mesh/imp/Axis.imp", L"../ImpGraphicsEngine/FPDefaultVS.hlsl", L"../ImpGraphicsEngine/FPDefaultPS.hlsl");
	_AxisObject->SetTransformMatrix(XMMatrixIdentity());
	_GridObject = _factory->CreateIObject<LineListObject>(L".grid", L"../ImpGraphicsEngine/FPDefaultVS.hlsl", L"../ImpGraphicsEngine/FPDefaultPS.hlsl");
	_GridObject->SetTransformMatrix(XMMatrixIdentity());
	if (_has2ndCamera)
	{
        _quadLeftObjectForDeferred = _factory->CreateIObject<
            QuadObject>(L"Resources/Mesh/imp/LeftHalfQuad.imp",
                        L"../ImpGraphicsEngine/SecondPassVS.hlsl",
                        L"../ImpGraphicsEngine/SecondPassPS.hlsl");

		_forwardOITQuadLeftObject = _factory->CreateIObject<ForwardOITQuadObject>(
            L"Resources/Mesh/imp/LeftHalfQuad.imp",
            L"../ImpGraphicsEngine/ForwardQuadVS.hlsl",
            L"../ImpGraphicsEngine/ForwardQuadPS.hlsl");
	}
	else
	{
        _quadLeftObjectForDeferred = _factory->CreateIObject<
            QuadObject>(L"Resources/Mesh/imp/Quad.imp",
                        L"../ImpGraphicsEngine/SecondPassVS.hlsl",
                        L"../ImpGraphicsEngine/SecondPassPS.hlsl");

		_forwardOITQuadLeftObject = _factory->CreateIObject<ForwardOITQuadObject>(
            L"Resources/Mesh/imp/Quad.imp",
            L"../ImpGraphicsEngine/ForwardQuadVS.hlsl",
            L"../ImpGraphicsEngine/ForwardQuadPS.hlsl");
	}

	_quadRightObjectForDeferred = _factory->CreateIObject<
        QuadObject>(L"Resources/Mesh/imp/RightHalfQuad.imp",
                    L"../ImpGraphicsEngine/SecondPassVS.hlsl",
                    L"../ImpGraphicsEngine/SecondPassPS.hlsl");

	_forwardOITQuadRightObject = _factory->CreateIObject<ForwardOITQuadObject>(
        L"Resources/Mesh/imp/RightHalfQuad.imp",
        L"../ImpGraphicsEngine/ForwardQuadVS.hlsl",
        L"../ImpGraphicsEngine/ForwardQuadPS.hlsl");

	_bloomPassQuadObject = _factory->CreateIObject<
        BloomQuadObject>(L"Resources/Mesh/imp/Quad.imp",
                    L"../ImpGraphicsEngine/BloomPassVS.hlsl",
                    L"../ImpGraphicsEngine/BloomPassPS.hlsl");

	size_t objectID = -2;
	////_objects[objectID] = _factory->CreateObject<AnimationObject>(L"Resources/Mesh/ase/genji_max.ASE", L"../ImpGraphicsEngine/FPMeshVS.hlsl", L"../ImpGraphicsEngine/FPMeshPS.hlsl", L"Resources/Texture/Test/000000002405.dds");
	////_objects[objectID] = _factory->CreateObject<AnimationObject>(L"Resources/Mesh/ase/genji_max.ASE", L"../ImpGraphicsEngine/FPMeshVS.hlsl", L"../ImpGraphicsEngine/FPMeshPS.hlsl", L"Resources/Texture/Test/000000002405_reverse.dds");
	//_opaqueObjects[objectID] = _factory->CreateIObject<MeshObject>(L"Resources/Mesh/fbx/Standard_Run.fbx", L"../ImpGraphicsEngine/FPMeshVS.hlsl", L"../ImpGraphicsEngine/FPMeshPS.hlsl", L"Resources/Texture/Test/000000002405.dds");
	//_opaqueObjects[objectID] = _factory->CreateIObject<AnimationObject>(L"Resources/Mesh/fbx/dummy.fbx", L"../ImpGraphicsEngine/FPMeshVS.hlsl", L"../ImpGraphicsEngine/FPMeshPS.hlsl", L"Resources/Texture/Test/000000002405.dds");
	//_opaqueObjects[objectID] = _factory->CreateIObject<SkinnedMeshObject>(L"Resources/Mesh/fbx/test0215_3.fbx", L"../ImpGraphicsEngine/FPSkinnedMeshVS.hlsl", L"../ImpGraphicsEngine/FPSkinnedMeshPS.hlsl", L"Resources/Texture/Test/000000002405.dds");
	//_opaqueObjects[objectID] = _factory->CreateIObject<SkinnedMeshObject>(L"Resources/Mesh/fbx/running0.fbx", L"../ImpGraphicsEngine/FPSkinnedMeshVS.hlsl", L"../ImpGraphicsEngine/FPSkinnedMeshPS.hlsl", L"Resources/Texture/Test/000000002405.dds");
    //_opaqueObjects[objectID]->SetTransformMatrix(XMMatrixIdentity());
    //_cullingManager->AddObject(objectID, _objects[objectID]);
	//
	objectID = -3;
	//_testForward = _factory->CreateIObject<MeshObject>(L"Resources/Mesh/fbx/sphere.fbx", L"../ImpGraphicsEngine/ForwardMeshVS.hlsl", L"../ImpGraphicsEngine/ForwardMeshPS.hlsl");
    //_testForward->SetTransformMatrix(XMMatrixIdentity());
    //MaterialPath testMat;
    //testMat.baseColor  = L"Resources/Texture/Test/000000002405.dds";
    //testMat.metallicRoughness  = L"Resources/Texture/Test/000000002407.dds";
	//_objects[objectID] = _factory->CreateIObject<PBRMeshObject>(L"Resources/Mesh/ase/genji_max.ASE", L"../ImpGraphicsEngine/FPPBRMeshVS.hlsl", L"../ImpGraphicsEngine/FPPBRMeshPS.hlsl", testMat);
	//_objects[objectID]->SetTransformMatrix(XMMatrixIdentity());

	objectID = -4;
	//_objects[objectID] = _factory->CreateIObject<SkinnedMeshObject>(L"Resources/Mesh/ase/Cylinder.ASE", L"../ImpGraphicsEngine/FPSkinnedMeshVS.hlsl", L"../ImpGraphicsEngine/FPSkinnedMeshPS.hlsl", L"Resources/Texture/Test/000000002405.dds");
	//_objects[objectID]->SetTransformMatrix(XMMatrixIdentity());
	//_boundingVolumeObjects[objectID] = _factory->CreateBoundingVolumeObject<BoundingVolumeObject>(ImpMath::Vector3{ 0, 0, 0 }, ImpMath::Vector3{1.3f, 1.3f, 1.3f });

 	return true;
}

void ImpD3D11Renderer::Update(float deltaTime)
{
	_deltaTime = deltaTime;

	_animationPlayer->Update(deltaTime);

	/// helper Object
	if (_isOnAxis)
	{
		_AxisObject->Update(_camera);
	}
	if (_isOnGrid)
	{
		_GridObject->Update(_camera);
	}

	for (const auto& ui : _UIObjects)
    {
        ui.second->Update(deltaTime);
    }

	/// 일반 Object
	for (const auto& object : _opaqueObjects)
	{
		if (object.second != nullptr)
		{
            object.second->Update(_camera);
		}
	}
    for (const auto& object : _transparentObjects)
    {
        if (object.second != nullptr)
        {
            object.second->Update(_camera);
        }
    }
	for (const auto& object : _boundingVolumeObjects)
	{
		object.second->Update(_camera);
	}

	_quadLeftObjectForDeferred->Update(_camera );
	_bloomPassQuadObject->Update(_camera);
    _forwardOITQuadLeftObject->Update(_camera);
}

void ImpGraphics::ImpD3D11Renderer::UpdateRight(float deltaTime)
{
    /// helper Object
    if (_isOnAxis)
    {
        _AxisObject->Update(_cameraRight);
    }
    if (_isOnGrid)
    {
        _GridObject->Update(_cameraRight);
    }

    /// 일반 Object
    for (const auto& object : _opaqueObjects)
    {
        if (object.second != nullptr)
        {
            object.second->Update(_cameraRight);
        }
    }
    for (const auto& object : _transparentObjects)
    {
        if (object.second != nullptr)
        {
            object.second->Update(_cameraRight);
        }
    }
    for (const auto& object : _boundingVolumeObjects)
    {
        if (object.second != nullptr)
        {
            object.second->Update(_cameraRight);
        }
    }

	_quadLeftObjectForDeferred->Update(_cameraRight);
    _quadRightObjectForDeferred->Update(_cameraRight);
    _forwardOITQuadLeftObject->Update(_cameraRight);
    _forwardOITQuadRightObject->Update(_cameraRight);
}

void ImpD3D11Renderer::BeginRender()
{
	ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();
	assert(deviceContext);
	
	_renderTarget->FirstPassBind();
	_renderTarget->FirstPassClear();
}

void ImpD3D11Renderer::Render()
{
	_device->GetDeviceContext()->OMSetDepthStencilState(_renderState->GetNormalDSS(), 0);

	for (const auto& object : _opaqueObjects)
	{
		if (object.second != nullptr)
		{
            if (object.first == 100000 || object.first == 100001)
			{
                object.second->Render(_device, _renderState, _renderTarget);
			}
            else if (_cullingManager->IsInsideFrustum(object.first, object.second))
            {
                object.second->Render(_device, _renderState, _renderTarget);
            }
		}
	}
	for (const auto& object : _boundingVolumeObjects)
	{
		object.second->Render(_device, _renderState, _renderTarget);
	}

	/// helper Object
	if (_isOnAxis)
	{
		_AxisObject->Render(_device, _renderState, _renderTarget);
	}
	if (_isOnGrid)
	{
		_GridObject->Render(_device, _renderState, _renderTarget);
	}
	
	// 2nd Pass
	_renderTarget->SecondPassBind();
	_renderTarget->SecondPassClear();

	/// 모든 오브젝트를 그리고 마지막에 두 번째 Pass를 처리한다.
    // culling 된 라이트는 안 보내기
    // _lights 복사본 만들고 컬링된거 없애고 넣기
    std::unordered_map<size_t, LightBase*> _lightCopy;
    for (const auto& light : _lights)
    {
        if (light.second->GetLightType() == 1 || light.second->GetLightType() == 2)
        {
            if (_cullingManager->IsInsideFrustum(light.first))
            {
                _lightCopy[light.first] = light.second;
            }
        }
        else
        {
            _lightCopy[light.first] = light.second;
        }
    }
    _quadLeftObjectForDeferred->SetLight(_lightCopy, _globalEmissionCoefficient);

    _quadLeftObjectForDeferred->Render(_device, _renderState, _renderTarget);

	_renderTarget->SecondPassUnbind();

	// Forward Object 그리기
    _renderTarget->ForwardPassBind();
    _renderTarget->ForwardPassClear();

    RenderTransparent();

    _renderTarget->ForwardPassUnbind();

	// Forward 합성
    _renderTarget->ForwardOITPassBind();
    _renderTarget->ForwardOITPassClear();

	_forwardOITQuadLeftObject->Render(_device, _renderState, _renderTarget);

	_renderTarget->ForwardOITPassUnbind();

	// bloom
	_bloomPassQuadObject->Render(_device, _renderState, _renderTarget);

    _renderTarget->ThreePassUnbind();
}

void ImpGraphics::ImpD3D11Renderer::RenderRight()
{
    _device->GetDeviceContext()->OMSetDepthStencilState(_renderState->GetNormalDSS(), 0);

    for (const auto& object : _opaqueObjects)
    {
		if (object.second != nullptr)
		{
            if (object.first == 100000 || object.first == 100001)
            {
                object.second->Render(_device, _renderState, _renderTarget);
            }
			else if (_cullingManager->IsInsideFrustumRight(object.first, object.second))
			{
                object.second->Render(_device, _renderState, _renderTarget);
			}
		}
    }
    for (const auto& object : _boundingVolumeObjects)
    {
        object.second->Render(_device, _renderState, _renderTarget);
    }

    /// helper Object
    if (_isOnAxis)
    {
        _AxisObject->Render(_device, _renderState, _renderTarget);
    }
    if (_isOnGrid)
    {
        _GridObject->Render(_device, _renderState, _renderTarget);
    }

    // 2nd Pass
    _renderTarget->SecondPassBind();

    /// 모든 오브젝트를 그리고 마지막에 두 번째 Pass를 처리한다.
    std::unordered_map<size_t, LightBase*> _lightCopy;
    for (const auto& light : _lights)
    {
        if (light.second->GetLightType() == 2 && light.second->GetLightType() == 1)
        {
            if (_cullingManager->IsInsideFrustumRight(light.first))
            {
                _lightCopy[light.first] = light.second;
            }
        }
        else
        {
            _lightCopy[light.first] = light.second;
        }
    }

    _quadRightObjectForDeferred->SetLight(_lights, _globalEmissionCoefficient);
    _quadRightObjectForDeferred->Render(_device, _renderState, _renderTarget);

    _renderTarget->SecondPassUnbind();

	// Forward Object 그리기
    _renderTarget->ForwardPassBind();
    _renderTarget->ForwardPassClear();

    RenderTransparentRight();

    _renderTarget->ForwardPassUnbind();

    // Forward 합성
    _renderTarget->ForwardOITPassBind();
    _renderTarget->ForwardOITPassClear();

    _forwardOITQuadRightObject->Render(_device, _renderState, _renderTarget);

    _renderTarget->ForwardOITPassUnbind();


	_bloomPassQuadObject->Render(_device, _renderState, _renderTarget);

    _renderTarget->ThreePassUnbind();
}

void ImpD3D11Renderer::SetInterpolation(size_t objectID, bool onOff)
{
    dynamic_cast<SkinnedMeshObject*>(_opaqueObjects[objectID])->_isOnInterpolation = onOff;
}

std::wstring ImpD3D11Renderer::GetCurAnimation(size_t objectID)
{
    return _animationPlayer->GetAnimation(objectID);
}

void ImpD3D11Renderer::PlaySkeletalAnimationNonLoop(size_t objectID, std::wstring fbx)
{
	SkinnedMeshObject* tempObject = dynamic_cast<SkinnedMeshObject*>(_opaqueObjects[objectID]);
    tempObject->SetAnimation(std::to_wstring(objectID) + fbx);
    _animationPlayer->PlayAnimation(objectID, std::to_wstring(objectID) + fbx);

	_animationPlayer->AddInterpolationObject(std::to_wstring(objectID) + fbx, tempObject);
    tempObject->_changingCount = 1;

	_animationPlayer->SetSkinningObject(objectID, tempObject);
}

void ImpD3D11Renderer::SetDefaultSkeltalAnimation(size_t objectID, std::wstring fbx)
{
	_animationPlayer->SetDefaultAnimation(objectID, std::to_wstring(objectID) + fbx);
}

void ImpD3D11Renderer::UpdateSkeletalAnimationSpeed(size_t objectID, std::wstring fbx, float speed)
{
    _animationPlayer->UpdatePlaySpeed(std::to_wstring(objectID) + fbx, speed);
}

void ImpD3D11Renderer::PlaySkeletalAnimation(size_t objectID, std::wstring fbx)
{
	SkinnedMeshObject* tempObject = dynamic_cast<SkinnedMeshObject*>(_opaqueObjects[objectID]);
    tempObject->SetAnimation(std::to_wstring(objectID) + fbx);
    _animationPlayer->PlayLoopAnimation(objectID, std::to_wstring(objectID) + fbx);

	_animationPlayer->AddInterpolationObject(std::to_wstring(objectID) + fbx, tempObject);
    tempObject->_changingCount = 1;

	_animationPlayer->SetSkinningObject(objectID, tempObject);
}

void ImpD3D11Renderer::AddSkeletalAnimation(size_t objectID, std::wstring fbx)
{
     _opaqueObjects[objectID]->GetGeometryBuffer()->AddAniamtion(fbx, std::to_wstring(objectID) + fbx);
}

void ImpD3D11Renderer::AddSkeletalAnimationObject(MeshObjectInfo meshObjectInfo)
{
    IObject* tempObject;
    tempObject = _factory->CreateIObject<SkinnedMeshObject>(meshObjectInfo._meshPath,
        meshObjectInfo._vertexShaderPath,
        meshObjectInfo._pisxelShaderPath,
        meshObjectInfo._material, meshObjectInfo._objectID);
    _cullingManager->AddObject(meshObjectInfo._objectID, tempObject);

    _opaqueObjects[meshObjectInfo._objectID] = tempObject;
}

void ImpD3D11Renderer::SetObjectEmissionCoefficient(size_t objectID, float emission)
{
    if (_opaqueObjects[objectID] != nullptr)
    {
        _opaqueObjects[objectID]->SetEmissionCoefficient(emission);
    }
    else
    {
        _transparentObjects[objectID]->SetEmissionCoefficient(emission);
    }

	_quadLeftObjectForDeferred->SetEmissionCoefficient(emission);
	_quadRightObjectForDeferred->SetEmissionCoefficient(emission);
}

void ImpD3D11Renderer::SetWorldEmissionCoefficient(float emission)
{
    _globalEmissionCoefficient = emission;
}

void ImpD3D11Renderer::SetTransparentObjectAlpha(size_t objectID, float customAlpha)
{
    dynamic_cast<ForwardObject*>(_transparentObjects[objectID])->SetAlpha(customAlpha);
}

void ImpD3D11Renderer::EndRender()
{
	IDXGISwapChain* swapChain = _device->GetSwapChain();
	assert(swapChain);

	HR(swapChain->Present(0, 0));
}

void ImpD3D11Renderer::Finalize()
{

}

//비율 확인하기
void ImpD3D11Renderer::SetClientSize(int width, int height)
{
	_screenWidth = width;
	_screenHeight = height;

	_renderTarget->OnResize(width, height);
	if (_has2ndCamera)
	{
        _camera->SetScreenSize(width / 2, height);
        _cameraRight->SetScreenSize(width / 2, height);
	}
	else
	{
        _camera->SetScreenSize(width, height);
        _cameraRight->SetScreenSize(width / 2, height);
	}

	_pickingManager->SetClientSize(width, height);
	
	// temp culling
	XMFLOAT4X4 projTM = _camera->GetProjTM();
    XMMATRIX projTMXM = XMLoadFloat4x4(&projTM);
    _cullingManager->SetViewFrustum(&projTMXM);

	if (_has2ndCamera)
	{
        projTM   = _cameraRight->GetProjTM();
        projTMXM = XMLoadFloat4x4(&projTM);
        _cullingManager->SetViewFrustumRight(&projTMXM);
	}
}

void* ImpD3D11Renderer::GetDevice()
{
	return _device->GetDevice();
}

void* ImpD3D11Renderer::GetDeviceContext()
{
	return _device->GetDeviceContext();
}

void ImpD3D11Renderer::UpdateMeshObject(size_t objectID, Matrix transformMatrix)
{
	DirectX::XMMATRIX xmTransformMatrix(&transformMatrix._11);
	if (_opaqueObjects[objectID] != nullptr)
	{
        _opaqueObjects[objectID]->SetTransformMatrix(xmTransformMatrix);
	}
	else
	{
        _transparentObjects[objectID]->SetTransformMatrix(xmTransformMatrix);
	}
}

void ImpD3D11Renderer::AddMeshObejct(MeshObjectInfo meshObjectInfo)
{
    IObject* tempObject;
	if (meshObjectInfo._material.baseColor != L"")
	{
		if (meshObjectInfo._isOpaque)
		{
            tempObject = _factory->CreateIObject<PBRMeshObject>(
                meshObjectInfo._meshPath,
                meshObjectInfo._vertexShaderPath,
                meshObjectInfo._pisxelShaderPath,
                meshObjectInfo._material);
		}
		else
		{
            tempObject = _factory->CreateIObject<ForwardObject>(
                meshObjectInfo._meshPath,
                meshObjectInfo._vertexShaderPath,
                meshObjectInfo._pisxelShaderPath,
                 meshObjectInfo._material);
		}
	}
	else
	{
        tempObject = _factory->CreateIObject<MeshObject>(
						meshObjectInfo._meshPath,
                        meshObjectInfo._vertexShaderPath,
                        meshObjectInfo._pisxelShaderPath);
	}
    _cullingManager->AddObject(meshObjectInfo._objectID, tempObject);

	if (meshObjectInfo._isOpaque)
	{
        _opaqueObjects[meshObjectInfo._objectID] = tempObject;
	}
	else
	{
        _transparentObjects[meshObjectInfo._objectID] = tempObject;
	}
}

void ImpD3D11Renderer::DeleteMeshObject(size_t objectID)
{
	IObject* temp = _opaqueObjects[objectID];
	if (temp == nullptr)
	{
        temp = _transparentObjects[objectID];
		if (temp == nullptr)
		{
			MessageBox(NULL, L"삭제하려는 오브젝트가 존재하지 않습니다.", L"에러", MB_ICONERROR);
			return;
		}
		else
		{
            delete temp;
            _transparentObjects.erase(objectID);

            _cullingManager->DeleteObject(objectID);
		}
	}
	else
	{
        delete temp;
        _opaqueObjects.erase(objectID);

        _cullingManager->DeleteObject(objectID);
	}
}

void ImpD3D11Renderer::SetGrid(bool isOn)
{
	_isOnGrid = isOn;
}

void ImpD3D11Renderer::SetAxis(bool isOn)
{
	_isOnAxis = isOn;
}

void ImpD3D11Renderer::SetWireFrame(bool isOn)
{
	_isOnWireFrame = isOn;
	_renderState->SetWireFrame(isOn);
}

void ImpD3D11Renderer::AddLight(LightInfo lightInfo)
{
	LightBase* light;
	switch (lightInfo._lightType)
	{
		case 0:
			light = new DirectionalLight;
			break;
		case 1:
            light = new SpotLight;
            _cullingManager->AddLight(lightInfo._lightID, lightInfo._range);
			// Todo culling
			break;
		case 2:
			light = new PointLight;
            _cullingManager->AddLight(lightInfo._lightID, lightInfo._range);
			break;
		default:
			light = nullptr;
			break;
	}

	light->SetLight(lightInfo);

	_lights[lightInfo._lightID] = light;
}

void ImpD3D11Renderer::UpdateLight(size_t lightID, Matrix transformMatrix)
{
	_lights[lightID]->Update(transformMatrix);
    if (_lights[lightID]->GetLightType() == 1 || _lights[lightID]->GetLightType() == 2)
	{
		_cullingManager->UpdateLight(lightID, {transformMatrix._41, transformMatrix._42, transformMatrix._43});
	}
}

void ImpD3D11Renderer::DeleteLight(size_t lightID)
{
	LightBase* temp = _lights[lightID];
	if (temp == nullptr)
	{
		MessageBox(NULL, L"삭제하려는 라이트가 존재하지 않습니다.", L"에러", MB_ICONERROR);
		return;
	}

	// culling Point Light
    if (_lights[lightID]->GetLightType() == 1 || _lights[lightID]->GetLightType() == 2)
	{
        _cullingManager->DeleteLight(lightID);
	}

	delete temp;
	_lights.erase(lightID);
}

void ImpD3D11Renderer::SetLight(size_t lightID, LightInfo lightInfo)
{
	_lights[lightID]->SetLight(lightInfo);
}

void ImpD3D11Renderer::UpdateCamera(Matrix transformMatrix)
{
	_camera->Update(transformMatrix);
    _cullingManager->UpdateCameraPosition(_camera->GetPosition(), _camera->GetRotationQut());
}

void ImpD3D11Renderer::SetCamera(CameraInfo cameraInfo)
{
	_camera->SetCamera(cameraInfo);

	// temp culling
    XMFLOAT4X4 projTM = _camera->GetProjTM();
    XMMATRIX projTMXM = XMLoadFloat4x4(&projTM);
    _cullingManager->SetViewFrustum(&projTMXM);
}

void ImpD3D11Renderer::SetSkyBox(std::wstring cubeMapPath)
{
	size_t objectID = -1;
	IObject* temp = _opaqueObjects[objectID];
	if (temp != nullptr)
	{
		delete temp;
		_opaqueObjects.erase(objectID);
	}

	_opaqueObjects[objectID] = _factory->CreateIObject<SkyBoxObject>(L"Resources/Mesh/imp/a.fbx", L"../ImpGraphicsEngine/FPSkyBoxVS.hlsl", L"../ImpGraphicsEngine/FPSkyBoxPS.hlsl", cubeMapPath);
	_opaqueObjects[objectID]->SetTransformMatrix(XMMatrixIdentity());
}

void ImpD3D11Renderer::AddCubeBoundingVolume(size_t boundingVolumeID, /*Vector3 center,*/ Vector3 length)
{
	_boundingVolumeObjects[boundingVolumeID] = _factory->CreateBoundingVolumeObject<BoundingVolumeObject>(Vector3(0.0f, 0.0f, 0.0f), length);
}

void ImpD3D11Renderer::AddSphereBoundingVolume(size_t boundingVolumeID, /*Vector3 center,*/ float radius)
{
	_boundingVolumeObjects[boundingVolumeID] = _factory->CreateBoundingVolumeObject<BoundingVolumeObject>(Vector3(0.0f, 0.0f, 0.0f), radius);
}

void ImpD3D11Renderer::UpdateBoundingVolume(size_t boundingVolumeID, Matrix transformMatrix, Vector4 color)
{
	_boundingVolumeObjects[boundingVolumeID]->SetColor(color);

	DirectX::XMMATRIX xmTransformMatrix(&transformMatrix._11);
	_boundingVolumeObjects[boundingVolumeID]->SetTransformMatrix(xmTransformMatrix);
}

void ImpD3D11Renderer::DeleteBoundingVolume(size_t boundingVolumeID)
{
	BoundingVolumeObject* temp = _boundingVolumeObjects[boundingVolumeID];
	if (temp == nullptr)
	{
		MessageBox(NULL, L"삭제하려는 오브젝트가 존재하지 않습니다.", L"에러", MB_ICONERROR);
		return;
	}

	delete temp;
	_boundingVolumeObjects.erase(boundingVolumeID);
}

size_t ImpD3D11Renderer::GetPickingObject(long mouseX, long mouseY)
{
	// 레이를 만들어야 함
	// 바운딩 볼륨을 만들고
	// 충돌 했는 지 체크한다음
	// 충돌한 바운딩 볼륨의 오브젝트를 가져와서
	// 폴리곤 단위로 픽킹을 한다음
	// 맞다면 픽킹한 오브젝트의 인덱스를 반환해줌

	pickingObjectNum = _pickingManager->GetPickingObject(_opaqueObjects, mouseX, mouseY);
	if (pickingObjectNum == -1)
	{
		pickingObjectNum = _pickingManager->GetPickingObject(_transparentObjects, mouseX, mouseY);
	}
	return pickingObjectNum;
}

void ImpD3D11Renderer::DrawDebugData()
{
	//_font->DrawTextColor(0, 14, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, (TCHAR*)L"Description: %s", "asdf");
	_font->DrawTextColor(14, 14, XMFLOAT4{ 0.3f, 0.1f, 0.3f, 1.0f }, (TCHAR*)L"Client Resolution: %dx%d", _screenWidth , _screenHeight);
	_font->DrawTextColor(14, 28, XMFLOAT4{ 0.3f, 0.1f, 0.3f, 1.0f }, (TCHAR*)L"DeltaTime: %f", _deltaTime);
	_font->DrawTextColor(14, 42, XMFLOAT4{ 0.3f, 0.1f, 0.3f, 1.0f }, (TCHAR*)L"FPS: %f", 1 / _deltaTime);
	_font->DrawTextColor(14, 56, XMFLOAT4{ 0.3f, 0.1f, 0.3f, 1.0f }, (TCHAR*)L"Num Of Object: %d (-1. SkyBox, -2. Gengi)", _opaqueObjects.size());
	_font->DrawTextColor(14, 70, XMFLOAT4{ 0.3f, 0.1f, 0.3f, 1.0f }, (TCHAR*)L"Num Of BoundingVolume: %d", _boundingVolumeObjects.size());
	_font->DrawTextColor(14, 84, XMFLOAT4{ 0.3f, 0.1f, 0.3f, 1.0f }, (TCHAR*)L"PickingObjectNum: %d", pickingObjectNum);
}

void ImpD3D11Renderer::Set2ndCamera(CameraInfo cameraInfo)
{
    _cameraRight->SetCamera(cameraInfo);

	// temp culling
    XMFLOAT4X4 projTM = _cameraRight->GetProjTM();
    XMMATRIX projTMXM = XMLoadFloat4x4(&projTM);
    _cullingManager->SetViewFrustumRight(&projTMXM);
}

void ImpGraphics::ImpD3D11Renderer::Update2ndCamera(Matrix transformMatrix)
{
    _cameraRight->Update(transformMatrix);
    _cullingManager->UpdateCameraPositionRight(_cameraRight->GetPosition(),
                                               _cameraRight->GetRotationQut());
}

void ImpD3D11Renderer::Has2ndCamera(bool has2ndCamera)
{
	if (has2ndCamera)
	{
        _camera->SetScreenSize(_screenWidth / 2, _screenHeight);
        _quadLeftObjectForDeferred->SetGeometryBuffer(
            _resourceManager->GetResource<GeometryBuffer>(L"Resources/Mesh/imp/LeftHalfQuad.imp"));
        _forwardOITQuadLeftObject->SetGeometryBuffer(
            _resourceManager->GetResource<GeometryBuffer>(L"Resources/Mesh/imp/"
                                                          L"LeftHalfQuad.imp"));
	}
	else
	{
        _camera->SetScreenSize(_screenWidth, _screenHeight);
        _quadLeftObjectForDeferred->SetGeometryBuffer(
            _resourceManager->GetResource<GeometryBuffer>(L"Resources/Mesh/imp/Quad.imp"));
        _forwardOITQuadLeftObject->SetGeometryBuffer(
            _resourceManager->GetResource<GeometryBuffer>(L"Resources/Mesh/imp/"
                                                          L"Quad.imp"));
	}

    _has2ndCamera = has2ndCamera;
}

void ImpGraphics::ImpD3D11Renderer::BeginRenderRight()
{
    ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();
    assert(deviceContext);

    _renderTarget->FirstPassBind();
    _renderTarget->FirstPassClear();
}

void ImpD3D11Renderer::AddUI(size_t uiID, UIInfo uiInfo)
{
	//Vector4 uiPos, DirectX::XMFLOAT2 screenSize, size_t layer, std::wstring texturePath
    IUIObject* tempUIObject = _factory->CreateIUIObject<UIObject>(uiInfo._UIPos, DirectX::XMFLOAT2{static_cast<float>(_screenWidth), static_cast<float>(_screenHeight)}, uiInfo._layer, uiInfo._texturePath);

	tempUIObject->SetOffset((uiInfo._UIPos.z / _screenWidth), (uiInfo._UIPos.w / _screenHeight));

	tempUIObject->SetWidth(uiInfo._UIPos.z);
	tempUIObject->SetHeight(uiInfo._UIPos.w);
    tempUIObject->SetScreenHeight(_screenHeight);
    tempUIObject->SetScreenWidth(_screenWidth);

	_UIObjects[uiID] = tempUIObject;
}

void ImpD3D11Renderer::DeleteUI(size_t uiID)
{
    IUIObject* temp = _UIObjects[uiID];
    if (temp == nullptr)
    {
        MessageBox(NULL,
                   L"삭제하려는 UI가 존재하지 않습니다.",
                   L"에러",
                   MB_ICONERROR);
        return;
    }

    delete temp;
    _UIObjects.erase(uiID);
}

void ImpD3D11Renderer::UpdateUI(size_t uiID, float x, float y, float alpha)
{
    float tempOffsetX = 0;
    float tempOffsetY = 0;

	_UIObjects[uiID]->GetOffset(tempOffsetX, tempOffsetY);

    _UIObjects[uiID]->SetXY(x / _screenWidth * 2 - 1 + tempOffsetX, (y / _screenHeight) * 2 - 1 + tempOffsetY, (float)_screenWidth / _screenHeight);
    _UIObjects[uiID]->SetAlpha(alpha);
}

void ImpD3D11Renderer::SetUI(size_t uiID, UIInfo uiInfo)
{
    uiInfo._UIPos.x             = 0;
    uiInfo._UIPos.y            = 0;
	std::wstring lengthString = std::to_wstring(_screenWidth) +
                                std::to_wstring(_screenHeight) +
                                std::to_wstring(uiInfo._UIPos.x) +
                                std::to_wstring(uiInfo._UIPos.y) +
                                std::to_wstring(uiInfo._UIPos.z) +
                                std::to_wstring(uiInfo._UIPos.w);

	std::shared_ptr<GeometryBuffer> geometryBuffer = _resourceManager->GetResource<GeometryBuffer>(lengthString + L".quad", uiInfo._UIPos, DirectX::XMFLOAT2{static_cast<float>(_screenWidth), static_cast<float>(_screenHeight)});
	std::shared_ptr<Texture> texture = _resourceManager->GetResource<Texture>(uiInfo._texturePath);

    _UIObjects[uiID]->SetLayer(uiInfo._layer);
    _UIObjects[uiID]->SetGeometryBuffer(geometryBuffer);
    _UIObjects[uiID]->SetTexture(texture);
}

void ImpD3D11Renderer::SetUIXRatio(size_t uiID, float ratio)
{
	if (ratio < 0)
	{
        return;
	}
    _UIObjects[uiID]->SetXSize(ratio);
}

void ImpD3D11Renderer::SetUINoise(size_t uiID, bool isOn)
{
    _UIObjects[uiID]->SetNoise(isOn);
}

void ImpD3D11Renderer::SetUILayer(size_t uiID, size_t layer)
{
    _UIObjects[uiID]->SetLayer(layer);
}

ImpMath::Vector3 ImpGraphics::ImpD3D11Renderer::GetBoundingBoxLength(std::wstring meshPath, Vector3& offset)
{
    std::shared_ptr<GeometryBuffer> geometryBuffer = _resourceManager->GetResource<GeometryBuffer>(meshPath);

	std::unique_ptr<Box> tempBox = make_unique<Box>(geometryBuffer->GetVertexPositions());
    offset = tempBox->_center;

	return tempBox->_extents;
}

void ImpD3D11Renderer::RenderTransparentRight()
{
	_device->GetDeviceContext()->OMSetDepthStencilState(_renderState->GetNormalDSS(), 0);

    for (const auto& object : _transparentObjects)
    {
		if (object.second != nullptr)
		{
            if (_cullingManager->IsInsideFrustumRight(object.first, object.second))
            {
				dynamic_cast<ForwardObject*>(object.second)->SetLight(_lights, _globalEmissionCoefficient);
                object.second->Render(_device, _renderState, _renderTarget);
            }
		}
    }
}

void ImpD3D11Renderer::RenderTransparent()
{
    _device->GetDeviceContext()->OMSetDepthStencilState(_renderState->GetNormalDSS(), 0);

    for (const auto& object : _transparentObjects)
    {
		if (object.second != nullptr)
		{
            if (_cullingManager->IsInsideFrustum(object.first, object.second))
            {
				dynamic_cast<ForwardObject*>(object.second)->SetLight(_lights, _globalEmissionCoefficient);
                object.second->Render(_device, _renderState, _renderTarget);
            }
		}
    }
}

void ImpD3D11Renderer::RenderUI()
{
    std::vector<IUIObject*> uiObject; 
	for (const auto& ui : _UIObjects)
	{
        uiObject.push_back(ui.second);
	}

	std::sort(uiObject.begin(), uiObject.end(), [](IUIObject* a, IUIObject* b) { return a->GetLayer() < b->GetLayer(); });

	for (const auto& ui : uiObject)
	{
        ui->Render(_device, _renderState, _renderTarget);
	}

	#ifdef _DEBUG
    DrawDebugData();
	#endif
}