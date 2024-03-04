#include "ImpGameEnginePCH.h"
#include "ImpGameEngine.h"
#include "CameraSystem.h"
#include "RenderingSystem.h"

#pragma comment(lib, "../ImpLib/ImpGraphicsEngine.lib")
#include "../ImpGraphicsEngine/IImpGraphicsEngine.h"

ImpGameEngine::ImpGameEngine::ImpGameEngine()
    : _graphicsEngine(nullptr)
    , _engineModule(nullptr)
    , _cameraSystem(nullptr)
    , _renderingSystem(nullptr)
{
}

ImpGameEngine::ImpGameEngine::~ImpGameEngine()
{
}

void ImpGameEngine::ImpGameEngine::Initialize(WindowInfomation info)
{
    _graphicsEngine = ImpGraphics::EngineExporter::GetEngine();

    _engineModule    = std::make_unique<ImpEngineModule::EngineModule>();
    _cameraSystem    = std::make_unique<CameraSystem>();
    _renderingSystem = std::make_unique<RenderingSystem>();

    // 1. �׷��Ƚ� ���� �ʱ�ȭ
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
    _graphicsEngine->Initialize(reinterpret_cast<int>(info._hInstance),
        reinterpret_cast<int>(info._hWnd),
        info._screenWidth,
        info._screenHeight);
#pragma warning(default : 4311)
#pragma warning(default : 4302)

    // 2. ���� ���� ����
    const std::wstring startWorld = L"Loby";

    // 3. EngineModule �ʱ�ȭ
    _engineModule->Initialize(info._hWnd, startWorld);

    // 4. System �ʱ�ȭ
    _cameraSystem->Initialize(_graphicsEngine, _engineModule.get());
    _renderingSystem->Initialize(_graphicsEngine, _engineModule.get());

    // 5. ���ӷ��� ����
    _engineModule->Start();
    _cameraSystem->Start();
}

void ImpGameEngine::ImpGameEngine::Process()
{
    MSG msg;

    bool isDone = false;
    while (!isDone)
    {
        // 1. Window Mess
        //
        // +age
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                isDone = true;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // ResizeWindow
            if (WindowInfomation::resizeHegiht != 0 &&
                WindowInfomation::resizeWidth != 0)
            {
                _graphicsEngine->SetClientSize(WindowInfomation::resizeWidth,
                    WindowInfomation::resizeHegiht);

                WindowInfomation::resizeHegiht = 0;
                WindowInfomation::resizeWidth  = 0;
            }
            float dt = _engineModule->Update();

            if (!_cameraSystem->OnSecondCamera())
            {
                _renderingSystem->Update();
            }
            else
            {
                _renderingSystem->UpdateLeft();
            }

            _graphicsEngine->UpdateCamera(_cameraSystem->GetMainCameraMarix());

            _graphicsEngine->Update(dt);

            _graphicsEngine->BeginRender();
            _graphicsEngine->Render(
                ImpGraphics::IImpGraphicsEngine::RendererType::Forward);

            if (_cameraSystem->OnSecondCamera())
            {
                _renderingSystem->UpdateRight();
                _graphicsEngine->Update2ndCamera(
                    _cameraSystem->GetSecondCameraMarix());
                _graphicsEngine->UpdateRight(dt);
                _graphicsEngine->BeginRenderRight();
                _graphicsEngine->RenderRight(
                    ImpGraphics::IImpGraphicsEngine::RendererType::Forward);
            }

            _graphicsEngine->RenderUI();
            _graphicsEngine->EndRender();

            _engineModule->PostUpdate();

            // ��������
            if (_engineModule->GetWorldManager()->IsEnd())
            {
                isDone = true;
            }
        }
    }
}

void ImpGameEngine::ImpGameEngine::Finalize()
{
    _engineModule->Finalize();
    _renderingSystem->Finalize();
    _graphicsEngine->Finalize();

    // �޸� ����
    ImpGraphics::EngineExporter::DeleteEngine();
}
