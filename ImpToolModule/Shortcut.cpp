#include "ImpToolModulePCH.h"
#include "Shortcut.h"
#include "EntityViewer.h"
#include "Mesh.h"
#include "WorldViewer.h"
#include "ImpEditor.h"

ImpToolModule::Shortcut::Shortcut()
    : _copyEntity(nullptr)
    , _toolInputManager(nullptr)
    , _engineModule(nullptr)
    , _entityViewer(nullptr)
    , _isShowCopyBoundingVolume(true)
    , _graphics(nullptr)
    , _editor(nullptr)
    , _worldViewer(nullptr)
{
}

ImpToolModule::Shortcut::~Shortcut()
{
}

void ImpToolModule::Shortcut::Initialize(ImpEngineModule::EngineModule* engineMoudle,
    ImpToolModule::EntityViewer* entViewer,
    ImpEngineModule::InputManager* toolInput,
    ImpGraphics::IImpGraphicsEngine* graphics,
    ImpToolModule::WorldViewer* worldViewer,
    ImpToolModule::ImpEditor* editor)
{
    _engineModule     = engineMoudle;
    _entityViewer     = entViewer;
    _toolInputManager = toolInput;
    _graphics         = graphics;
    _worldViewer      = worldViewer;
    _editor           = editor;

    _engineModule->GetEventManager()->SubScribe<ImpEngineModule::Event::LoadedWorld>(
        this);
}


void ImpToolModule::Shortcut::Finalize()
{
    if (_debugMesh != nullptr)
    {
        delete _debugMesh;
    }
}

void ImpToolModule::Shortcut::Update()
{
    if (_toolInputManager->IsKeyState(Key::Ctrl, KeyState::Hold) &&
        _toolInputManager->IsKeyState(Key::C, KeyState::Tap))
    {
        Copy();
    }

    if (_toolInputManager->IsKeyState(Key::Ctrl, KeyState::Hold) &&
        _toolInputManager->IsKeyState(Key::V, KeyState::Tap))
    {
        Paste();
    }

    if (_editor->GetMode() == ToolMode::EditorMode)
    {
        // 로드
        if (_toolInputManager->IsKeyState(Key::Ctrl, KeyState::Hold) &&
            _toolInputManager->IsKeyState(Key::Z, KeyState::Tap))
        {
            std::wstring name = _engineModule->GetWorldManager()->GetWorldName();
            _engineModule->GetWorldManager()->ChangeWorld(name);
        }

        // 저장
        if (_toolInputManager->IsKeyState(Key::Ctrl, KeyState::Hold) &&
            _toolInputManager->IsKeyState(Key::S, KeyState::Tap))
        {
            _worldViewer->SaveWorld();
        }
    }


    ShowCopyBoundingVolume();

    if (_toolInputManager->IsKeyState(Key::Del, KeyState::Tap))
    {
        Delete();
    }
}

void ImpToolModule::Shortcut::Copy()
{
    ImpEngineModule::Entity* ent = _entityViewer->GetSelectEntity();

    if (ent == nullptr)
    {
        return;
    }

    if (_debugMesh != nullptr)
    {
        delete _debugMesh;
        _debugMesh = nullptr;
    }

    _copyEntity = ent;
    _debugMesh  = new Debug::Mesh(_graphics, _copyEntity);
}

void ImpToolModule::Shortcut::Paste()
{
    if (_copyEntity == nullptr)
    {
        return;
    }

    auto world = _engineModule->GetWorld();

    std::queue<ImpEngineModule::Entity*> entQueue;
    entQueue.push(_copyEntity);

    std::map<std::string, ImpEngineModule::Transform*> matchParent;

    while (!entQueue.empty())
    {
        ImpEngineModule::Entity* copyEnt = entQueue.front();
        entQueue.pop();

        std::shared_ptr<ImpEngineModule::Entity>
            newEntity = std::make_shared<ImpEngineModule::Entity>(*copyEnt);

        world->RegisterEntity(newEntity);

        // 부모가 있으면 연결한다.
        auto parent = copyEnt->GetComponent<ImpEngineModule::Transform>()->GetParent();

        if (parent != nullptr)
        {
            auto iter = matchParent.find(parent->GetEntity()->GetName());

            if (iter == matchParent.end())
            {
                newEntity->GetComponent<ImpEngineModule::Transform>()->SetParent(
                    parent);
            }
            else
            {
                newEntity->GetComponent<ImpEngineModule::Transform>()->SetParent(
                    iter->second);
            }
        }

        matchParent.insert({ copyEnt->GetName(),
            newEntity->GetComponent<ImpEngineModule::Transform>() });

        // 자식이 있으면 같이 복사한다.
        for (auto child :
            copyEnt->GetComponent<ImpEngineModule::Transform>()->GetChildren())
        {
            entQueue.push(child->GetEntity());
        }
    }
}

void ImpToolModule::Shortcut::ShowCopyBoundingVolume()
{
    if (!_isShowCopyBoundingVolume)
    {
        return;
    }

    if (_debugMesh == nullptr)
    {
        return;
    }

    _debugMesh->Update();

    _debugMesh->GetBoundingVolume();

    auto transform = _copyEntity->GetComponent<ImpEngineModule::Transform>();

    Vector3 boundingVolume = _debugMesh->GetBoundingVolume();

    if (boundingVolume == Vector3::Zero)
    {
        boundingVolume = Vector3::One;
    }
    else
    {
        boundingVolume *= 2.f;
    }

    Matrix rotM = Matrix::MakeFromQuaternion(transform->GetRotation());

    // 복사하는 오브젝트 이동 키
    if (_toolInputManager->IsKeyState(Key::Right, KeyState::Tap))
    {
        Vector3 movePos = Vector3(boundingVolume.x, 0.f, 0.f);
        movePos         = movePos.Transform(rotM);

        transform->AddPosition(movePos);
    }
    if (_toolInputManager->IsKeyState(Key::Left, KeyState::Tap))
    {
        Vector3 movePos = Vector3(-boundingVolume.x, 0.f, 0.f);
        movePos         = movePos.Transform(rotM);

        transform->AddPosition(movePos);
    }
    if (_toolInputManager->IsKeyState(Key::Up, KeyState::Tap))
    {
        Vector3 movePos = Vector3(0.f, 0.f, boundingVolume.z);
        movePos         = movePos.Transform(rotM);

        transform->AddPosition(movePos);
    }
    if (_toolInputManager->IsKeyState(Key::Down, KeyState::Tap))
    {
        Vector3 movePos = Vector3(0.f, 0.f, -boundingVolume.z);
        movePos         = movePos.Transform(rotM);

        transform->AddPosition(movePos);
    }
    if (_toolInputManager->IsKeyState(Key::PgUp, KeyState::Tap))
    {
        Vector3 movePos = Vector3(0.f, boundingVolume.y, 0.f);
        movePos         = movePos.Transform(rotM);

        transform->AddPosition(movePos);
    }
    if (_toolInputManager->IsKeyState(Key::PgDn, KeyState::Tap))
    {
        Vector3 movePos = Vector3(0.f, -boundingVolume.y, 0.f);
        movePos         = movePos.Transform(rotM);

        transform->AddPosition(movePos);
    }
}

void ImpToolModule::Shortcut::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::LoadedWorld& event)
{
    _copyEntity = nullptr;

    if (_debugMesh != nullptr)
    {
        delete _debugMesh;
        _debugMesh = nullptr;
    }
}

void ImpToolModule::Shortcut::Delete()
{
    _engineModule->GetWorld()->DestroyEntity(_entityViewer->GetSelectEntity());
    _entityViewer->SetSelectEntity(-1);
}
