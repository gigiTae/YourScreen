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
/// World �����͸� �����ϰ� �ε����� ����
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
    // EntityResouece�� Entity�� �ҷ��´�
    void LoadToEntityResource(const std::filesystem::path& path,
                              ImpEngineModule::Entity* ent);

    // Entity�� World�� �ٷ� �ҷ��´�(World Start ������ ȣ��)
    void LoadEntity(const std::filesystem::path& path, Entity* ent) const;

    // ���� �������� �ε��� Entity���� �޸𸮿� �ø���, ���� Entity��� World�� �����Ѵ�.
    void LoadWorld();

    // Entity���� �������θ� �ε��Ѵ�.
    void BuildEntityHierachy(const std::filesystem::path& path);

    // ���带 ��ȯ�Ѵ�.
    void ChangeWorld(const std::wstring& worldName);

    // Cube���� �ε��Ѵ�.
    void LoadCubeMap(const std::wstring& cubeMapTextPath);

    // TagMap �ε�
    void LoadTagMap(const std::wstring& tagMapTextPath);

    // ���� ������ �̸�����ȯ
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

    std::wstring _currentWorldName; // ���� ����
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