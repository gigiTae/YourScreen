#include "EngineModulePCH.h"
#include "WorldManager.h"
#include "World.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "PathManager.h"
#include "EntityResource.h"
#include "CollisionManager.h"
#include "Transform.h"
#include "TagMapResource.h"
#include "TimeManager.h"
#include "EventManager.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "SoundManager.h"

ImpEngineModule::WorldManager::WorldManager()
    : _world(nullptr)
    , _typeManager(nullptr)
    , _currentWorldName{}
    , _resourceManager(nullptr)
    , _timeManager(nullptr)
    , _collisionManager(nullptr)
    , _eventManager(nullptr)
    , _cameraManager(nullptr)
    , _inputManager(nullptr)
{
}

ImpEngineModule::WorldManager::~WorldManager()
{
}


void ImpEngineModule::WorldManager::Initialize(const std::wstring& startWorld,
    ImpEngineModule::World* world,
    ImpEngineModule::ResourceManager* resMgr,
    CollisionManager* collisionManager,
    ImpEngineModule::TimeManager* timeMgr,
    EventManager* eventMgr,
    CameraManager* cameraMgr,
    ImpEngineModule::InputManager* inputManager)
{
    _currentWorldName = startWorld;
    _typeManager      = ImpReflection::TypeManager::GetInstance();
    _world            = world;
    _resourceManager  = resMgr;
    _collisionManager = collisionManager;
    _timeManager      = timeMgr;
    _eventManager     = eventMgr;
    _cameraManager    = cameraMgr;
    _inputManager     = inputManager;
}


void ImpEngineModule::WorldManager::Finalize()
{
}

void ImpEngineModule::WorldManager::LoadToEntityResource(
    const std::filesystem::path& path,
    ImpEngineModule::Entity* ent)
{
    /// Entity���ҽ��� �޾ƿ´�.
    EntityResource* entRes = _resourceManager->GetEntityResource(path);

    assert(entRes && "��ȿ���� �ʴ� ���ҽ��Դϴ�.");

    /// �̸������ص� Entity�� �����͸� �����Ѵ�.
    entRes->Clone(ent);
}

void ImpEngineModule::WorldManager::LoadEntity(const std::filesystem::path& path,
    Entity* ent) const
{
    /// Entity�� �̸��� ���ϸ�
    std::string name = path.filename().string();
    ImpStringHelper::RemoveSubstring(name, ".ent");

    ent->SetName(name);

    if (ent->GetName() == "Player1")
    {
        ent->SetID(ImpPlayer::left);
    }
    else if (ent->GetName() == "Player2")
    {
        ent->SetID(ImpPlayer::right);
    }


    std::ifstream readData(path);

    /// ���÷����� ����ؼ� Entity�� �ε��Ѵ�.
    ImpReflection::TypeManager* typeMgr = ImpReflection::TypeManager::GetInstance();

    if (readData.is_open())
    {
        std::string fileContent((std::istreambuf_iterator<char>(readData)),
            std::istreambuf_iterator<char>());

        size_t nowRead = 0, open = 0, close = 0;

        // ù��°���� Entity�� �������̴�.
        nowRead = fileContent.find('\n') + 1;

        std::string typeName = ImpStringHelper::GetTypeName(fileContent, nowRead);

        if (typeName.empty())
        {
            // ���� Entity�� ���
            return;
        }
        // ���÷����� �̿��ؼ� Entity ������ �ҷ��´�.
        do
        {
            TypeIndex index = typeMgr->GetTypeIndex(typeName);

            assert(index != ImpEngineModule::EXCEPTION);

            ImpReflection::Type* type = typeMgr->GetType(index);

            open  = ImpStringHelper::FindOpeningBrace(fileContent, nowRead);
            close = ImpStringHelper::FindClosingBrace(fileContent, nowRead);

            std::string sub = fileContent.substr(open, close - open - 1);

            void* component = type->Invoke(fileContent, open, close);

            ent->AddComponent(component, index);

            // Component }\nComponentName �̹Ƿ� +2
            nowRead  = close + 2;
            typeName = ImpStringHelper::GetTypeName(fileContent, nowRead);

        } while (!typeName.empty());
    }
    else
    {
        assert(!"�����б⿡ �����Ͽ����ϴ�.");
    }
}

void ImpEngineModule::WorldManager::LoadWorld()
{
    // 0. ������ ��θ� ���´�.
    std::filesystem::path worldPath = PathManager::GetWorldPath(_currentWorldName);

    // 1.���ҽ� �Ŵ����� ���ؼ� LoadEntityList�� �޸� �߰��Ѵ�.
    std::filesystem::path loadEntityListPath = worldPath;
    loadEntityListPath += "/LoadEntityList.txt";
    _resourceManager->LoadEntityList(loadEntityListPath);

    // 2. StartEntity���� World�� �߰��Ѵ�.
    auto startEntityList = PathManager::GetStartEntityList(worldPath);

    // 3. Entity���� �����Ѵ�.
    for (auto& entPath : startEntityList)
    {
        LoadEntity(entPath, _world->CreateEntity());
    }

    // 4.������ Entity���� ���������� �����Ѵ�.
    for (auto& entPath : startEntityList)
    {
        BuildEntityHierachy(entPath);
    }

    // 5. ť����� �ε��Ѵ�.
    std::wstring cubePath = worldPath;
    cubePath += L"\\CubeMap.txt";
    LoadCubeMap(cubePath);

    // 6. TagMap�� �ε��Ѵ�
    std::wstring tagMapPath = worldPath;
    tagMapPath += L"\\TagMap.txt";
    LoadTagMap(tagMapPath);

    _eventManager->Emit<Event::LoadedWorld>({ _currentWorldName });
}

void ImpEngineModule::WorldManager::BuildEntityHierachy(
    const std::filesystem::path& path)
{
    std::string name = path.filename().string();
    ImpStringHelper::RemoveSubstring(name, ".ent");

    Entity* ent = _world->GetEntity(name);

    assert(ent != nullptr && "��ƼƼ �̸� �б� ����");

    std::ifstream readData(path);

    if (readData.is_open())
    {
        std::string info;
        std::getline(readData, info);

        // ���� �̸��� Parent< �ΰ�찡 ������?
        size_t start = info.find("Parent<");
        size_t end   = info.find_last_of(">");

        // �θ� ���� ���
        if (start == std::string::npos)
        {
            readData.close();
            return;
        }

        start = start + std::string("Parent<").length();

        std::string parentName = info.substr(start, end - start);

        Entity* parent = _world->GetEntity(parentName);

        assert(parent != nullptr);

        ent->GetComponent<Transform>()->SetParent(
            parent->GetComponent<Transform>());

        readData.close();
    }
}

void ImpEngineModule::WorldManager::ChangeWorld(const std::wstring& worldName)
{
    // 1. ���� ���带 �ʱ�ȭ�ϰ� �ʿ���� ���ҵ��� �޸𸮿��� �����Ѵ�.
    _world->Reset();
    _resourceManager->UnloadAllEntityResources();
    _collisionManager->Reset();
    SoundManager::GetInstance()->UnloadAllSound();

    // 2. ���� ���忡�� �ʿ��� ���ҽ����� �ε��Ѵ�.
    _currentWorldName = worldName;

    if (_currentWorldName == L"Exit")
    {
        _isEnd = true;
        return;
    }

    LoadWorld();

    // 3. ���带 �����Ѵ�.
    SoundManager::GetInstance()->LoadSoundList(_currentWorldName);
    _world->Start();
    _timeManager->Initialize();
    _inputManager->OffAllPadVibration();

    if (_cameraManager->GetSecondCamera())
    {
        _eventManager->Emit<Event::SetSecondCamera>({ true });
    }
    else
    {
        _eventManager->Emit<Event::SetSecondCamera>({ false });
    }
}

void ImpEngineModule::WorldManager::LoadCubeMap(const std::wstring& cubeMapTextPath)
{
    std::wifstream readData(cubeMapTextPath);

    if (readData.is_open())
    {
        std::wstring resourcePath = PathManager::GetResourcesPath();
        std::wstring cubeMapRelativePath;
        std::getline(readData, cubeMapRelativePath);

        _resourceManager->LoadCubeMap(resourcePath + cubeMapRelativePath);
        readData.close();
    }
}

void ImpEngineModule::WorldManager::LoadTagMap(const std::wstring& tagMapTextPath)
{
    _resourceManager->LoadTagMapResource(tagMapTextPath);
    _collisionManager->SetTagMap(
        _resourceManager->GetTagMapResource()->GetTagMap());
}
