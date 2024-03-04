#include "ImpGameEnginePCH.h"
#include "ImpToolEngine.h"
#include "CameraSystem.h"
#include "RenderingSystem.h"

#pragma comment(lib, "../ImpLib/ImpGraphicsEngine.lib")
#include "../ImpGraphicsEngine/IImpGraphicsEngine.h"

ImpGameEngine::ImpToolEngine::ImpToolEngine()
    : _graphicsEngine(nullptr)
    , _engineModule(nullptr)
    , _cameraSystem(nullptr)
    , _renderingSystem(nullptr)
    , _editor(nullptr)
{
}

ImpGameEngine::ImpToolEngine::~ImpToolEngine()
{
}

void ImpGameEngine::ImpToolEngine::Initialize(WindowInfomation info)
{
    _graphicsEngine = ImpGraphics::EngineExporter::GetEngine();

    _engineModule    = new ImpEngineModule::EngineModule();
    _cameraSystem    = new CameraSystem();
    _renderingSystem = new RenderingSystem();

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
    _cameraSystem->Initialize(_graphicsEngine, _engineModule);
    _renderingSystem->Initialize(_graphicsEngine, _engineModule);

    _editor = new ImpToolModule::ImpEditor();
    _editor->Initialize(info._hWnd,
        _graphicsEngine->GetDevice(),
        _graphicsEngine->GetDeviceContext(),
        _engineModule,
        _graphicsEngine);

    // 5. ī�޶� ����
    _cameraSystem->SetToolCamera(_editor->GetToolCamera());

    // 6. ���ӷ��� ����
    _engineModule->Start();
    _cameraSystem->Start();
}

void ImpGameEngine::ImpToolEngine::Process()
{
    MSG msg;

    bool isDone = false;
    while (!isDone)
    {
        // 1. Window Message
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

            // 1. ���� ����带 �����´�
            auto toolMode = _editor->GetMode();

            float dt = _engineModule->GetTimeManager()->Update();
            _engineModule->GetInputManager()->Update(dt);
            ImpEngineModule::SoundManager::GetInstance()->Update();

            using namespace ImpEngineModule;

            if (toolMode == ImpToolModule::ToolMode::EditorMode)
            {
                _engineModule->GetWorld()->Each<Transform>(
                    [dt](Entity* ent, ComponentHandle<Transform> transform)
                    {
                        if (transform->IsRoot())
                        {
                            transform->UpdateMatrix();
                        }
                    });
            }

            // 2. ��忡������ ������ �ٸ��� ����.
            if (toolMode == ImpToolModule::ToolMode::GamePlayMode)
            {
                _engineModule->GetCollisionManager()->Update(dt);
                _engineModule->GetWorld()->CleanUp();
                _engineModule->GetWorld()->Update(dt);
            }
            else if (toolMode == ImpToolModule::ToolMode::PauseMode)
            {
                _engineModule->GetWorld()->CleanUp();
                _engineModule->GetWorld()->Update(0.f);
            }

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

            _editor->NewFrame();
            _editor->Update();

            _graphicsEngine->BeginRender();
            _graphicsEngine->Render(
                ImpGraphics::IImpGraphicsEngine::RendererType::Deferred);

            if (_cameraSystem->OnSecondCamera())
            {
                _renderingSystem->UpdateRight();
                _graphicsEngine->Update2ndCamera(
                    _cameraSystem->GetSecondCameraMarix());
                _graphicsEngine->UpdateRight(dt);
                _graphicsEngine->BeginRenderRight();
                _graphicsEngine->RenderRight(
                    ImpGraphics::IImpGraphicsEngine::RendererType::Deferred);
            }

            _graphicsEngine->RenderUI();
            _editor->EndRender();
            _graphicsEngine->EndRender();

            _engineModule->PostUpdate();

            // �����÷��̸�� <-> ������� ��ȯ
             if (_editor->IsChangeMode())
            {
                ImpToolModule::ToolMode changedMode = _editor->GetMode();
                if (changedMode == ImpToolModule::ToolMode::EditorMode ||
                    changedMode == ImpToolModule::ToolMode::PauseMode)
                {
                    _engineModule->GetInputManager()->SetActive(false);
                }
                else
                {
                    _engineModule->GetInputManager()->SetActive(true);
                }
            }

            _cameraSystem->SetToolCamera(_editor->GetToolCamera());

            // ��������
            if (_engineModule->GetWorldManager()->IsEnd())
            {
                isDone = true;
            }
        }
    }
}

void ImpGameEngine::ImpToolEngine::Finalize()
{
    _editor->Finalize();

    _engineModule->Finalize();
    _renderingSystem->Finalize();
    _graphicsEngine->Finalize();

    delete _editor;

    // �޸� ����

    ImpGraphics::EngineExporter::DeleteEngine();

    delete _engineModule;
    delete _cameraSystem;
    delete _renderingSystem;
}