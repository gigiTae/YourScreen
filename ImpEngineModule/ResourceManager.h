#pragma once

namespace ImpEngineModule
{
class EntityResource;
class CubeMap;
class TagMapResource;

class EventManager;

/// <summary>
/// ���ҽ��� �����ϴ� Ŭ����
/// </summary>
class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    /// �ʱ�ȭ
    void Initailize(const std::wstring startWorld, EventManager* eventManager);

    /// ����
    void Finalize();

    /// Entity���ҽ��� ��η� ��´�.
    EntityResource* GetEntityResource(const std::wstring& path);

    /// ���ҽ��� Ȯ���ڿ� �´� ���ҽ��� �ε��Ѵ�.
    void LoadResource(const std::wstring& path);

    /// �ε��� Entity���� ���� List�� �ε��Ѵ�.
    void LoadEntityList(const std::wstring& path);

    /// Entity ���ҽ��� �ε��Ѵ�.
    void LoadEntityResource(const std::wstring& path);

    /// ��� EntityResourc�� ��ε��Ѵ�.
    void UnloadAllEntityResources();

    /// ���� ť����� ��ȯ�Ѵ�.
    CubeMap* GetCubeMap()
    {
        return _cubeMap;
    }

    /// ť����� �ε��Ѵ�.
    void LoadCubeMap(const std::wstring& path);

    /// �±׸� ���ҽ��� �ε��Ѵ�.
    void LoadTagMapResource(const std::wstring& path);

    // �±׸� ���ҽ� ��ȯ
    TagMapResource* GetTagMapResource()
    {
        return _tagMapResource;
    }

    const std::unordered_map<std::wstring, std::unique_ptr<EntityResource>>& GetEntityResources()
        const;

private:
    const size_t entityResourcesID = 990000000000000000;

    std::vector<std::filesystem::path> _worldDataList;
    std::unordered_map<std::wstring, std::unique_ptr<EntityResource>> _entityResources;

    EventManager* _eventManager;
    CubeMap* _cubeMap;
    TagMapResource* _tagMapResource;
};


} // namespace ImpEngineModule