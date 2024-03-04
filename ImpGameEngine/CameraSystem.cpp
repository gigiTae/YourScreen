#include "ImpGameEnginePCH.h"
#include "CameraSystem.h"

ImpGameEngine::CameraSystem::CameraSystem()
    : _mainCameraInfo{}
    , _engineModule(nullptr)
    , _graphicsEngine(nullptr)
    , _toolCamera(nullptr)
    , _onSecondCamera(false)
{
}

ImpGameEngine::CameraSystem::~CameraSystem()
{
}

void ImpGameEngine::CameraSystem::Initialize(
    ImpGraphics::IImpGraphicsEngine* graphicsEngine,
    ImpEngineModule::EngineModule* engineModule)
{
    using namespace ImpEngineModule;
    _graphicsEngine = graphicsEngine;
    _engineModule   = engineModule;

    EventManager* eventMgr = _engineModule->GetEventManager();

    _engineModule->GetEventManager()->SubScribe<ImpEngineModule::Event::SetSecondCamera>(
        this);
}

void ImpGameEngine::CameraSystem::SetToolCamera(ImpEngineModule::Entity* main)
{
    _toolCamera = main;

    if (_toolCamera == nullptr)
    {
        // 2번째 카메라가 존재하는 경우
        ImpEngineModule::Entity*
            secondCamera = _engineModule->GetCameraManager()->GetSecondCamera();

        if (secondCamera != nullptr)
        {
            SetSecondCameraInfo(secondCamera);

            _onSecondCamera = true;
            _graphicsEngine->Has2ndCamera(true);
            _graphicsEngine->Set2ndCamera(_secondCameraInfo);
        }
    }
    else
    {
        _graphicsEngine->Has2ndCamera(false);
        _onSecondCamera = false;
    }
}

void ImpGameEngine::CameraSystem::Start()
{
    _graphicsEngine->SetCamera(GetMainCameraInfo());

    if (_toolCamera)
    {
        return;
    }

    // 2번째 카메라가 존재하는 경우
    ImpEngineModule::Entity* secondCamera = _engineModule->GetCameraManager()
                                                ->GetSecondCamera();

    if (secondCamera != nullptr)
    {
        SetSecondCameraInfo(secondCamera);

        _onSecondCamera = true;
        _graphicsEngine->Has2ndCamera(true);
        _graphicsEngine->Set2ndCamera(_secondCameraInfo);
    }
}

const ImpGraphics::CameraInfo& ImpGameEngine::CameraSystem::GetMainCameraInfo()
{
    // Tool 카메라가 있는 경우
    if (_toolCamera)
    {
        SetMainCameraInfo(_toolCamera);
    }
    else
    {
        ImpEngineModule::Entity* mainCamera = _engineModule->GetCameraManager()
                                                  ->GetMainCamera();

        assert(mainCamera && "메인카메라가 존재하지 않습니다");

        SetMainCameraInfo(mainCamera);
    }

    return _mainCameraInfo;
}

ImpMath::Matrix ImpGameEngine::CameraSystem::GetMainCameraMarix() const
{
    if (_toolCamera == nullptr)
    {
        ImpEngineModule::Entity* mainCamera = _engineModule->GetCameraManager()
                                                  ->GetMainCamera();

        assert(mainCamera && "MainCamera가 존재하지 않습니다");

        ImpEngineModule::Transform*
            transform = mainCamera->GetComponent<ImpEngineModule::Transform>();
        Matrix cameraMat = Matrix::MakeFromQuaternion(transform->GetRotation()) *
                           Matrix::MakeTranslationMatrix(
                               transform->GetWorldPostiion());

        return cameraMat;
    }
    else
    {
        return _toolCamera->GetComponent<ImpEngineModule::Transform>()->GetWorldMatrix();
    }
}

ImpMath::Matrix ImpGameEngine::CameraSystem::GetSecondCameraMarix() const
{
    ImpEngineModule::Entity* secondCamera = _engineModule->GetCameraManager()
                                                ->GetSecondCamera();

    assert(secondCamera && "SecondCamera가 존재하지 않습니다");

    ImpEngineModule::Transform*
        transform    = secondCamera->GetComponent<ImpEngineModule::Transform>();
    Matrix cameraMat = Matrix::MakeFromQuaternion(transform->GetRotation()) *
                       Matrix::MakeTranslationMatrix(transform->GetWorldPostiion());

    return cameraMat;
}


void ImpGameEngine::CameraSystem::SetSecondCamera(bool onSecondCamera)
{
    _onSecondCamera = onSecondCamera;
}

void ImpGameEngine::CameraSystem::SetMainCameraInfo(ImpEngineModule::Entity* main)
{
    if (main == nullptr)
    {
        _toolCamera = nullptr;
        return;
    }

    ImpEngineModule::Camera* camera = main->GetComponent<ImpEngineModule::Camera>();
    ImpEngineModule::Transform*
        transform = main->GetComponent<ImpEngineModule::Transform>();

    if (camera == nullptr || transform == nullptr)
    {
        return;
    }

    Vector3 euler = transform->GetRotation().ToEuler();
    Vector3 degree(ImpMath::RadianToDegree(euler.x),
        ImpMath::RadianToDegree(euler.y),
        ImpMath::RadianToDegree(euler.z));

    _mainCameraInfo._nearPlain     = camera->GetNearPlain();
    _mainCameraInfo._farPlain      = camera->GerFarPlain();
    _mainCameraInfo._fieldOfView   = camera->GetFieldOfView();
    _mainCameraInfo._isPerspective = camera->IsPerspective();
}

void ImpGameEngine::CameraSystem::SetSecondCameraInfo(ImpEngineModule::Entity* second)
{
    ImpEngineModule::Camera* camera = second->GetComponent<ImpEngineModule::Camera>();

    assert(camera->IsSecond());

    ImpEngineModule::Transform*
        transform = second->GetComponent<ImpEngineModule::Transform>();

    if (camera == nullptr || transform == nullptr)
    {
        return;
    }

    Vector3 euler = transform->GetRotation().ToEuler();
    Vector3 degree(ImpMath::RadianToDegree(euler.x),
        ImpMath::RadianToDegree(euler.y),
        ImpMath::RadianToDegree(euler.z));

    _secondCameraInfo._nearPlain     = camera->GetNearPlain();
    _secondCameraInfo._farPlain      = camera->GerFarPlain();
    _secondCameraInfo._fieldOfView   = camera->GetFieldOfView();
    _secondCameraInfo._isPerspective = camera->IsPerspective();
}

void ImpGameEngine::CameraSystem::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::SetSecondCamera& event)
{
    if (event.onSecondCamera)
    {
        _onSecondCamera = true;
        _graphicsEngine->Has2ndCamera(true);

        ImpEngineModule::Entity*
            secondCamera = _engineModule->GetCameraManager()->GetSecondCamera();

        SetSecondCameraInfo(secondCamera);
        _graphicsEngine->Set2ndCamera(_secondCameraInfo);
    }
    else
    {
        _graphicsEngine->Has2ndCamera(false);
        _onSecondCamera = false;

        _graphicsEngine->SetCamera(GetMainCameraInfo());
    }
}