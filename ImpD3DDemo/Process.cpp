#include "pch.h"
#include "Process.h"

#include "../ImpGraphicsEngine/IImpGraphicsEngine.h"
#include "GUIManager.h"
using namespace ImpGraphics;

Process::Process()
	: m_timer(nullptr),
	m_windowPosX(0), m_windowPosY(0),
	m_screenWidth(1920), m_screenHeight(1080),
	m_resizing(false),
	m_pRenderer(nullptr)
{
	CreateHWND(L"D3DEngine Demo", WS_OVERLAPPEDWINDOW, m_windowPosX, m_windowPosY, m_screenWidth, m_screenHeight);
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	m_pRenderer = EngineExporter::GetEngine();
	_guiManager = new ImpGraphics::GUIManager;
}

Process::~Process()
{
    //m_pRenderer->DeleteMeshObject(2);
    //m_pRenderer->DeleteMeshObject(5);
    m_pRenderer->DeleteMeshObject(6);
    m_pRenderer->DeleteMeshObject(7);
    m_pRenderer->DeleteMeshObject(8);
    m_pRenderer->DeleteMeshObject(11);
    m_pRenderer->DeleteMeshObject(100000);
    //m_pRenderer->DeleteMeshObject(100001);
	m_pRenderer->DeleteLight(0);
	m_pRenderer->DeleteLight(1);
	m_pRenderer->DeleteLight(2);
    m_pRenderer->DeleteUI(1);
    //m_pRenderer->DeleteUI(2);
	m_pRenderer->DeleteBoundingVolume(0);
	m_pRenderer->DeleteBoundingVolume(1);

	delete _guiManager;
	EngineExporter::DeleteEngine();
}

bool Process::Init(HINSTANCE hInstance)
{
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
	m_pRenderer->Initialize((int)hInstance, (int)m_hWnd, m_screenWidth, m_screenHeight);
#pragma warning(default : 4311)
#pragma warning(default : 4302)

	_guiManager->Init(reinterpret_cast<size_t>(m_hWnd), m_pRenderer->GetDevice(), m_pRenderer->GetDeviceContext());

	InputManager::GetInstance().Init(m_hWnd);
	m_timer = std::make_unique<GameTimer>();
	m_timer->Reset();

	MeshObjectInfo info;

// 	info._objectID = 2;
// 	//info._meshPath = L"Resources\\Mesh\\fbx\\dd.fbx";
//     info._meshPath = L"Resources/Mesh/fbx/untitled2.fbx";
// 	info._pisxelShaderPath = L"../ImpGraphicsEngine/FirstNormalPS.hlsl";
// 	info._vertexShaderPath = L"../ImpGraphicsEngine/FirstNormalVS.hlsl";
// 
  	//m_pRenderer->AddMeshObejct(info);

    info._objectID         = 5;
    info._meshPath         = L"Resources/Mesh/fbx/autoRig6.fbx";
    info._pisxelShaderPath = L"../ImpGraphicsEngine/FirstNormalPS.hlsl";
    info._vertexShaderPath = L"../ImpGraphicsEngine/FirstNormalVS.hlsl";

	//info._material.baseColor = L"Resources/Texture/Test/000000002405.dds";
	//info._material.metallicRoughness = L"Resources/Texture/Test/000000002407.dds";
	//info._material.emission = L"Resources/Texture/Test/black.dds";
	//info._material.normalMap = L"Resources/Texture/Test/000000002406_b.dds";

    //m_pRenderer->AddMeshObejct(info);

     info._objectID = 11;
    info._meshPath = L"Resources/Mesh/fbx/Main_Wall_L.fbx";
    // info._pisxelShaderPath = L"../ImpGraphicsEngine/ForwardPBRPS.hlsl";
    // info._vertexShaderPath = L"../ImpGraphicsEngine/ForwardPBRVS.hlsl";
    info._pisxelShaderPath = L"../ImpGraphicsEngine/FPUIMeshPS.hlsl";
    info._vertexShaderPath = L"../ImpGraphicsEngine/FPUIMeshVS.hlsl";
	
    info._material.baseColor =
        L"Resources/Texture/Test/Main_Wall_L_Material.001_BaseColor.png";
    info._material.metallicRoughness =
        L"Resources/Texture/Test/"
        L"Main_Wall_L_Material.001_OcclusionRoughnessMetallic.png";
    info._material.emission =
        L"Resources/Texture/Test/Main_Wall_L_Material.001_Emissive.png";
    info._material.normalMap =
        L"Resources/Texture/Test/Main_Wall_L_Material.001_Normal.png";

	m_pRenderer->AddMeshObejct(info);

	info._objectID         = 6;
    info._meshPath         = L"Resources/Mesh/fbx/capsule_joined_low.fbx";
    info._pisxelShaderPath = L"../ImpGraphicsEngine/ForwardPBRPS.hlsl";
    info._vertexShaderPath = L"../ImpGraphicsEngine/ForwardPBRVS.hlsl";
    info._pisxelShaderPath  = L"../ImpGraphicsEngine/FPUIMeshPS.hlsl";
    info._vertexShaderPath  = L"../ImpGraphicsEngine/FPUIMeshVS.hlsl";

    info._material.baseColor =
        L"Resources/Texture/Test/capsule_low_FOR LOW UV1_BaseColor.png";
    info._material.metallicRoughness =
        L"Resources/Texture/Test/capsule_low_FOR LOW UV1_OcclusionRoughnessMetallic.png";
    info._material.emission =
        L"Resources/Texture/Test/capsule_low_FOR LOW UV1_Emissive.png";
    info._material.normalMap =
        L"Resources/Texture/Test/capsule_low_FOR LOW UV1_Normal.png";
    //info._isOpaque = false;

    m_pRenderer->AddMeshObejct(info);

	info._objectID         = 7;
    info._meshPath         = L"Resources/Mesh/fbx/sphere_emp.fbx";
    info._pisxelShaderPath = L"../ImpGraphicsEngine/ForwardPBRPS.hlsl";
    info._vertexShaderPath = L"../ImpGraphicsEngine/ForwardPBRVS.hlsl";

    info._material.baseColor =
        L"Resources/Texture/Test/sphere_emp_DefaultMaterial_BaseColor.png";
    info._material.metallicRoughness =
        L"Resources/Texture/Test/sphere_emp_DefaultMaterial_OcclusionRoughnessMetallic.png";
    info._material.emission =
        L"Resources/Texture/Test/sphere_emp_DefaultMaterial_Emissive.png";
    info._material.normalMap =
        L"Resources/Texture/Test/sphere_emp_DefaultMaterial_Normal.png";
    info._isOpaque = false;

    m_pRenderer->AddMeshObejct(info);

	info._objectID         = 8;
    info._meshPath         = L"Resources/Mesh/fbx/capsule_joined_low.fbx";
    info._pisxelShaderPath = L"../ImpGraphicsEngine/ForwardPBRPS.hlsl";
    info._vertexShaderPath = L"../ImpGraphicsEngine/ForwardPBRVS.hlsl";

    info._material.baseColor =
        L"Resources/Texture/Test/capsule_low_FOR LOW UV1_BaseColor.png";
    info._material.metallicRoughness =
        L"Resources/Texture/Test/capsule_low_FOR LOW "
        L"UV1_OcclusionRoughnessMetallic.png";
    info._material.emission =
        L"Resources/Texture/Test/capsule_low_FOR LOW UV1_Emissive.png";
    info._material.normalMap =
        L"Resources/Texture/Test/capsule_low_FOR LOW UV1_Normal.png";
     info._isOpaque = false;

    m_pRenderer->AddMeshObejct(info);

	info._objectID         = 100000;
    info._meshPath         = L"Resources/Mesh/fbx/running0.fbx";
    info._pisxelShaderPath = L"../ImpGraphicsEngine/FPSkinnedMeshPS.hlsl";
    info._vertexShaderPath = L"../ImpGraphicsEngine/FPSkinnedMeshVS.hlsl";

    info._material.baseColor =
        L"Resources/Texture/Test/hacker_man_low_DefaultMaterial_BaseColor.png";
    info._material.metallicRoughness =
        L"Resources/Texture/Test/hacker_man_low_DefaultMaterial_OcclusionRoughnessMetallic.png";
    info._material.emission =
        L"Resources/Texture/Test/hacker_man_low_DefaultMaterial_Emissive.png";
    info._material.normalMap =
        L"Resources/Texture/Test/hacker_man_low_DefaultMaterial_Normal.png";
    info._isOpaque = false;

    m_pRenderer->AddSkeletalAnimationObject(info);

	//m_pRenderer->AddSkeletalAnimation(100000, L"Resources/Mesh/fbx/Chara_idel.fbx");
	m_pRenderer->AddSkeletalAnimation(100000, L"Resources/Mesh/fbx/swingToLand.fbx");
	//m_pRenderer->SetDefaultSkeltalAnimation(100000, L"Resources/Mesh/fbx/Chara_idel.fbx");
	m_pRenderer->PlaySkeletalAnimation(100000, L"Resources/Mesh/fbx/swingToLand.fbx");

    Matrix iden = Matrix::Identity();
    // m_pRenderer->UpdateMeshObject(2, iden);
    // m_pRenderer->UpdateMeshObject(5, iden);
    m_pRenderer->UpdateMeshObject(6, iden);
    m_pRenderer->UpdateMeshObject(7, iden);
    m_pRenderer->UpdateMeshObject(8, iden);
    m_pRenderer->UpdateMeshObject(11, iden);
    m_pRenderer->UpdateMeshObject(100000, iden);

	/// UI
     UIInfo uiinfo;
     uiinfo._UIID = 1;
     uiinfo._layer = 0;
     uiinfo._UIPos = Vector4{ 0, 0, 960, 1080 };
     uiinfo._texturePath = L"Resources/Texture/Test/hacking_screen_sans_purple_1.png";
    
     m_pRenderer->AddUI(1, uiinfo);

// 	uiinfo._UIID  = 2;
//     uiinfo._layer = 0;
//     uiinfo._UIPos = Vector4{ 1000, 200, 960, 1080 };
//     uiinfo._texturePath = L"Resources/Texture/Test/hack.jpg";
// 
// 	m_pRenderer->AddUI(2, uiinfo);

	Vector3 offset;
	//m_pRenderer->GetBoundingBoxLength(L"Resources/Mesh/fbx/Main_Wall_L.fbx", offset);

	// 카메라
	ImpGraphics::CameraInfo cameraInfo;

	cameraInfo._isPerspective = isPerspective;
	cameraInfo._fieldOfView = fov;
	cameraInfo._nearPlain = nearP;
	cameraInfo._farPlain = farP;

	m_pRenderer->SetCamera(cameraInfo);

    cameraInfo._isPerspective = isPerspective;
    cameraInfo._fieldOfView   = fov;
    cameraInfo._nearPlain     = nearP;
    cameraInfo._farPlain      = farP;

    m_pRenderer->Set2ndCamera(cameraInfo);

	// 라이트
	ImpGraphics::LightInfo lightInfo;

	lightInfo._lightID = 0;
	lightInfo._lightType = 0;
	lightInfo._color = { 1.0f, 1.0f, 1.0f, 1.0f };
	lightInfo._intensity = 1;
	
	m_pRenderer->AddLight(lightInfo);

	// Point Light
	lightInfo._lightID = 1;
	lightInfo._lightType = 2;
	lightInfo._color = { 1.0f, 1.0f, 1.0f, 1.0f };
	lightInfo._intensity = 1;

	m_pRenderer->AddLight(lightInfo);
    m_pRenderer->UpdateLight(1, Matrix::Identity());

	// Spot Light
    lightInfo._lightID   = 2;
    lightInfo._lightType = 1;
    lightInfo._color     = { 1.0f, 0.0f, 0.0f, 1.0f };
    lightInfo._intensity = 1;

	m_pRenderer->AddLight(lightInfo);
	m_pRenderer->UpdateLight(2, Matrix::Identity());

	//m_pRenderer->AddCubeBoundingVolume(0, ImpMath::Vector3{ 0, 0, 0 }, ImpMath::Vector3{ 1.3f, 1.3f, 1.3f });
	m_pRenderer->AddSphereBoundingVolume(0, 1.0f);
	m_pRenderer->AddCubeBoundingVolume(1, {1.0f, 1.0f, 1.0f});

	m_pRenderer->SetSkyBox(L"Resources/Texture/CubeMap/desertcube1024.dds");
	m_pRenderer->SetSkyBox(L"Resources/Texture/CubeMap/custom1.dds");

// 	UIInfo testuiinfo;
//     testuiinfo._UIPos = Vector4{ 0, 0, 1920, 1080 };
//     std::wstring first = L"Resources/Texture/Test/asdf/Untitled.00";
//     std::wstring last = L".jpg";
// 	for (int i = 10; i < 99; i++)
// 	{
//         testuiinfo._UIID        = i;
//         testuiinfo._layer = i;
//         testuiinfo._texturePath = first + std::to_wstring(i) + last;
// 		//testuiinfo._texturePath = L"Resources/Texture/Test/asdf/Untitled.0003.jpg";
// 		m_pRenderer->AddUI(i, testuiinfo);
// 	}


    //m_pRenderer->AddCubeBoundingVolume(10, tempVec3);

	return true;
}

void Process::Loop()
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
 			Update();
 			Render();
		}
	}
}

void Process::Finalize()
{
	_guiManager->Finalize();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Process::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	if (ImGui_ImplWin32_WndProcHandler(m_hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
		case WM_SIZE:
		{
			if (m_pRenderer == nullptr)
			{
				return 0;
			}
			m_pRenderer->SetClientSize(LOWORD(lParam), HIWORD(lParam));
		}

		case WM_PAINT:
			hdc = BeginPaint(m_hWnd, &ps);
			EndPaint(m_hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

void Process::UpdateTimer()
{
	// 매 프레임 시간을 계산한다.
	m_timer->Tick();

	// 매 프레임의 deltaTime
	m_timer->DeltaTime();

	/// 타이머를 기반으로 FPS, millisecond per frame (1프레임당 elapsedTime = deltaTime)을 계산 해 준다.

	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((m_timer->TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void Process::Update()
{
	InputManager::GetInstance().Update();
	UpdateTimer();
	/// 카메라 사용 예시
	Matrix m = Matrix::MakeRotationMatrixRollPitchYaw(rot) * Matrix::MakeTranslationMatrix(pos);
	Matrix mm = Matrix::MakeRotationMatrixRollPitchYaw(rot2) * Matrix::MakeTranslationMatrix(pos2);
	m_pRenderer->UpdateCamera(m);
	//m_pRenderer->UpdateLight(0, m);
	//m_pRenderer->UpdateLight(0, Matrix::Identity());
	m_pRenderer->UpdateLight(2, Matrix::Identity());
    m_pRenderer->Update2ndCamera(mm);

	static Matrix temptemp = Matrix::Identity();
	//temptemp._41 += 1.0f;

	//m_pRenderer->UpdateMeshObject(2, temptemp);
	m_pRenderer->UpdateBoundingVolume(0, Matrix::Identity(), { 1.0f, 0.0f, 1.0f, 1.0f });
	m_pRenderer->UpdateBoundingVolume(1, Matrix::Identity(), { 1.0f, 0.0f, 1.0f, 1.0f });
	//m_pRenderer->UpdateBoundingVolume(10, Matrix::Identity(), { 1.0f, 0.0f, 1.0f, 1.0f });

    static float testalpha = 0;
    testalpha += 0.001f;
	if (testalpha > 1.f)
	{
        testalpha = 0;
	}

    //m_pRenderer->UpdateUI(1, 960, 0, 1);

	//m_pRenderer->UpdateCamera(cameraInfo);
	///

	const float speed = 1.10f;
	if (InputManager::GetInstance().IsGetKey('W'))
	{
		Walk(speed);
	}
	if (InputManager::GetInstance().IsGetKey('S'))
	{
		Walk(-speed);
	}
	if (InputManager::GetInstance().IsGetKey('D'))
	{
		Strafe(speed);
	}
	if (InputManager::GetInstance().IsGetKey('A'))
	{
		Strafe(-speed);
	}
	if (InputManager::GetInstance().IsGetKey('E'))
	{
		WorldUpDown(speed);
	}
	if (InputManager::GetInstance().IsGetKey('Q'))
	{
		WorldUpDown(-speed);
	}

	if (InputManager::GetInstance().IsGetKey('I'))
    {
        Walk2(speed);
    }
    if (InputManager::GetInstance().IsGetKey('K'))
    {
        Walk2(-speed);
    }
    if (InputManager::GetInstance().IsGetKey('L'))
    {
        Strafe2(speed);
    }
    if (InputManager::GetInstance().IsGetKey('J'))
    {
        Strafe2(-speed);
    }

	if (InputManager::GetInstance().IsGetKey('V'))
	{
		static Matrix lightMat = Matrix::Identity();
		//lightMat._42 += speed / 20;
		
		lightMat._11 = 0.001f;
		lightMat._22 = 0.001f;
		lightMat._33 = 0.001f;

		//m_pRenderer->UpdateLight(1, lightMat);
		//m_pRenderer->UpdateLight(1, lightMat);
        m_pRenderer->UpdateMeshObject(6, lightMat);
	}
    if (InputManager::GetInstance().IsGetKey('N'))
    {
        static Matrix lightMat = Matrix::Identity();
        // lightMat._42 += speed / 20;

        lightMat._11 = 0.001f;
        lightMat._22 = 0.001f;
        lightMat._33 = 0.001f;
        lightMat._41 = 30;

        // m_pRenderer->UpdateLight(1, lightMat);
        // m_pRenderer->UpdateLight(1, lightMat);
        m_pRenderer->UpdateMeshObject(8, lightMat);
    }
    if (InputManager::GetInstance().IsGetKeyDown('P'))
    {
        _has2ndCamera = !_has2ndCamera;
		m_pRenderer->Has2ndCamera(_has2ndCamera);
    }

	if (InputManager::GetInstance().IsGetKey('Y'))
	{
		ImpGraphics::LightInfo lightInfo;

		static float tempIntensity = 0.0f;

		lightInfo._lightID = 0;
		lightInfo._lightType = 0;
		lightInfo._color = { 1.0f, 1.0f, 1.0f, 1.0f };
		tempIntensity += 0.001f;
		lightInfo._intensity = tempIntensity;

		m_pRenderer->SetLight(0, lightInfo);
	}

	static Matrix temp22 = Matrix::Identity();
	if (InputManager::GetInstance().IsGetKey('U'))
	{
		_rotMatrix = ImpMath::Matrix::MakeRotationMatrixRollPitchYaw(Vector3(0.02f, 0.04f, 0.4f));

		temp22 *= _rotMatrix;

		m_pRenderer->UpdateLight(0, temp22);
	}
    if (InputManager::GetInstance().IsGetKeyDown('O'))
    {
		m_pRenderer->PlaySkeletalAnimation(100000, L"Resources/Mesh/fbx/Chara_idel.fbx");   
    }
	if (InputManager::GetInstance().IsGetKeyDown('I'))
    {
		//m_pRenderer->PlaySkeletalAnimationNonLoop(100000, L"Resources/Mesh/fbx/running0.fbx");   
		m_pRenderer->PlaySkeletalAnimationNonLoop(100000, L"Resources/Mesh/fbx/running0.fbx");   
    }
	if (InputManager::GetInstance().IsGetKey('M'))
    {
//         static bool isOn = true;
//         isOn             = !isOn;
//         m_pRenderer->SetUINoise(1, isOn);
        static float time = 0;
        time += 0.5f;
        static size_t index = 10;
		if (time > 1.0f)
		{
            time = 0;
            index++;
		}
		if (index > 98)
		{
            index = 10;
		}

		m_pRenderer->SetUILayer(index, 2);
        m_pRenderer->UpdateUI(index, 0, 0, 1);

    }
	if (InputManager::GetInstance().IsGetKeyDown('C'))
	{
        m_pRenderer->SetInterpolation(100000, false);
	}
    if (InputManager::GetInstance().IsGetKeyDown('B'))
    {
        static float speed = 0.5f;
        speed += 0.1f;
        m_pRenderer->UpdateSkeletalAnimationSpeed(100000, L"Resources/Mesh/fbx/running0.fbx", speed);  
    }

	if (InputManager::GetInstance().IsGetKey(VK_RBUTTON))
	{
		float dx = (5.25f * static_cast<float>(InputManager::GetInstance().GetDeltaPosition().x) * (3.141592f / 180.0f));
		float dy = (5.25f * static_cast<float>(InputManager::GetInstance().GetDeltaPosition().y) * (3.141592f / 180.0f));
		
		//RotateY(dx);
		Pitch(dy);
		Yaw(dx);
	}

	//클릭 들어오면 getpickingObject
	if (InputManager::GetInstance().IsGetKeyDown(VK_LBUTTON))
	{
		size_t num = m_pRenderer->GetPickingObject(InputManager::GetInstance().GetMousePosition().x, InputManager::GetInstance().GetMousePosition().y);
		int a = 3;
	}

	_guiManager->Update(m_pRenderer);

	/// 엔진 업데이트
	m_pRenderer->Update(m_timer->DeltaTime());

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}
}

void Process::Render()
{
	/// 그리기를 준비한다.
 	m_pRenderer->BeginRender();
 
 	/// 엔진만의 그리기를 한다.
 	m_pRenderer->Render(IImpGraphicsEngine::RendererType::Forward);
 
	if (_has2ndCamera)
	{
        m_pRenderer->UpdateRight(m_timer->DeltaTime());

        m_pRenderer->BeginRenderRight();
        m_pRenderer->RenderRight(IImpGraphicsEngine::RendererType::Forward);
	}

	m_pRenderer->RenderUI();
	_guiManager->Render();
 	/// 그리기를 끝낸다.
 	m_pRenderer->EndRender();
}

void Process::Strafe(float distance)
{
	//mPosition = XMFLOAT3(mRight.x * d + mPosition.x, mRight.y * d + mPosition.y, mRight.z * d + mPosition.z);
	_rotMatrix = ImpMath::Matrix::MakeRotationMatrixRollPitchYaw(rot);

	pos = ImpMath::Vector3(_rotMatrix._11 * distance + pos.x,
		_rotMatrix._12 * distance + pos.y, _rotMatrix._13 * distance + pos.z);
}

void Process::Walk(float distance)
{
	//mPosition = XMFLOAT3(mLook.x * d + mPosition.x, mLook.y * d + mPosition.y, mLook.z * d + mPosition.z);
	_rotMatrix = ImpMath::Matrix::MakeRotationMatrixRollPitchYaw(rot);

	pos = ImpMath::Vector3(_rotMatrix._31 * distance + pos.x,
		_rotMatrix._32 * distance + pos.y, _rotMatrix._33 * distance + pos.z);
}

void Process::WorldUpDown(float distance)
{
	_rotMatrix = ImpMath::Matrix::MakeRotationMatrixRollPitchYaw(rot);

	pos = ImpMath::Vector3(_rotMatrix._21 * distance + pos.x,
		_rotMatrix._22 * distance + pos.y, _rotMatrix._23 * distance + pos.z);
}

void Process::Yaw(float angle)
{
	ImpMath::Vector3 rotation = rot;
	_rotMatrix = ImpMath::Matrix::MakeRotationMatrixRollPitchYaw(rotation);

	ImpMath::Vector3 up{ 0, 1, 0 };
	up = up.Normalize();

	rotation += up * angle;
	rot = rotation;
}

void Process::Pitch(float angle)
{
	ImpMath::Vector3 rotation = rot;
	_rotMatrix = ImpMath::Matrix::MakeRotationMatrixRollPitchYaw(rotation);

	ImpMath::Vector3 right{ 1, 0, 0 };
	right = right.Normalize();

	rotation += right * angle;
	rot = rotation;
}

void Process::Roll(float angle)
{
	ImpMath::Vector3 rotation = rot;
	_rotMatrix = ImpMath::Matrix::MakeRotationMatrixRollPitchYaw(rotation);

	ImpMath::Vector3 look{ 0, 0, 1 };
	look = look.Normalize();

	rotation += look * angle;
	rot = rotation;
}

void Process::Strafe2(float distance)
{
    // mPosition = XMFLOAT3(mRight.x * d + mPosition.x, mRight.y * d + mPosition.y, mRight.z * d + mPosition.z);
    _rotMatrix2 = ImpMath::Matrix::MakeRotationMatrixRollPitchYaw(rot2);

    pos2 = ImpMath::Vector3(_rotMatrix2._11 * distance + pos2.x,
                           _rotMatrix2._12 * distance + pos2.y,
                           _rotMatrix2._13 * distance + pos2.z);
}

void Process::Walk2(float distance)
{
    // mPosition = XMFLOAT3(mLook.x * d + mPosition.x, mLook.y * d + mPosition.y, mLook.z * d + mPosition.z);
    _rotMatrix2 = ImpMath::Matrix::MakeRotationMatrixRollPitchYaw(rot2);

    pos2 = ImpMath::Vector3(_rotMatrix2._31 * distance + pos2.x,
                           _rotMatrix2._32 * distance + pos2.y,
                           _rotMatrix2._33 * distance + pos2.z);
}