#pragma once

namespace ImpEngineModule
{
class World;
class Entity;
class ResourceManager;
class CollisionManager;
class TimeManager;
class EventManager;
class CameraManager;
class InputManager;

/// <summary>
/// World 데이터를 저장하고 로드기능을 제공
/// </summary>
class WorldManager
{
public:
    WorldManager();
    ~WorldManager();

    void Initialize(const std::wstring& startWorld,
                    ImpEngineModule::World* world,
                    ImpEngineModule::ResourceManager* resMgr,
                    CollisionManager* collisionManager,
                    ImpEngineModule::TimeManager* timeMgr,
                    EventManager* eventMgr,
                    CameraManager* cameraMgr,
                    ImpEngineModule::InputManager* inputManager);

    void Finalize();

public:
    // EntityResouece로 Entity를 불러온다
    void LoadToEntityResource(const std::filesystem::path& path,
                              ImpEngineModule::Entity* ent);

    // Entity를 World에 바로 불러온다(World Start 시점에 호출)
    void LoadEntity(const std::filesystem::path& path, Entity* ent) const;

    // 월드 시작전에 로드할 Entity들을 메모리에 올리고, 시작 Entity들로 World를 구성한다.
    void LoadWorld();

    // Entity들의 계층구로를 로드한다.
    void BuildEntityHierachy(const std::filesystem::path& path);

    // 월드를 전환한다.
    void ChangeWorld(const std::wstring& worldName);

    // Cube맵을 로드한다.
    void LoadCubeMap(const std::wstring& cubeMapTextPath);

    // TagMap 로드
    void LoadTagMap(const std::wstring& tagMapTextPath);

    // 현재 월드의 이름을반환
    const std::wstring& GetWorldName() const
    {
        return _currentWorldName;
    }

    bool IsEnd()
    {
        return _isEnd;
    }

private:
    bool _isEnd =false;

    std::wstring _currentWorldName; // 현재 월드
    ImpEngineModule::World* _world;

    ImpEngineModule::TimeManager* _timeManager;
    ImpEngineModule::CollisionManager* _collisionManager;
    ImpEngineModule::ResourceManager* _resourceManager;
    ImpEngineModule::EventManager* _eventManager;
    ImpEngineModule::CameraManager* _cameraManager;
    ImpEngineModule::InputManager* _inputManager;

    ImpReflection::TypeManager* _typeManager;
};


} // namespace ImpEngineModule