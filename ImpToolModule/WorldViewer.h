#pragma once

namespace ImpToolModule::Debug
{
class DebugViewer;
}

namespace ImpToolModule
{
class ImpEditor;
class ToolCamera;
class Shortcut;

/// <summary>
/// ������ ���õ� ������ ǥ���ϴ� GUI
/// </summary>
class WorldViewer:
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::LoadedWorld>
{
public:
    WorldViewer();
    ~WorldViewer();

public:
    /// �ʱ�ȭ
    void Initialize(ImpEngineModule::EngineModule* engineModule,
        ImpGraphics::IImpGraphicsEngine* graphicsEngine,
        ImpEditor* editor,
        Shortcut* shortcut,
        Debug::DebugViewer* debugViewer,
        ToolCamera* toolCamera);

    /// ������ GUI�� ������Ʈ�Ѵ�.
    void Update();

    /// ���� ����
    void SaveWorld();

    void Finalize();

private:
    void ShowMode();

    void ShowEnviromentMenu();

    /// ����Ʈ �޴�
    void EditMenu();

    /// ���� ���带 �����Ѵ�.
    void SaveWorldButton();
    /// �ε� ���� ��ư
    void LoadWorldButton();
    /// ���� ������ư
    void CreateWorldButton();

    /// ���ο� ���带 �����Ѵ�.
    void CreateWorld();

    /// ���ҽ� ����;
    void SaveResources(const std::filesystem::path& path);

    /// ������Ʈ ������ ��ȸ�ϸ鼭 ����ִ� ��� ������ ���Ϳ� ��´�.
    void FindPathResource(void* object,
        ImpReflection::Type* type,
        std::vector<std::wstring>& resources,
        std::queue<std::wstring>& entResources);

    // ���� ������ �̸��� ǥ���Ѵ�.
    void ShowWorldNameToText();

    // ��ƼƼ ���Ҹ� �����Ѵ�
    void SaveEntityResources(const std::vector<std::wstring>& pathResources,
        const std::filesystem::path& currentWorldPath);

    // ����� ����
    void SetGizmo();

    // ���� ����
    void SaveSoundList(const std::vector<std::wstring>& pathResources,
        const std::filesystem::path& currentWorldPath);

    // ť��� ���̺�
    void SaveCubeMap(const std::filesystem::path& worldPath);

    // TagMap ���̺�
    void SaveTagMap(const std::filesystem::path& worldPath);

    // �ε� ���� �ݹ�
    void Receive(ImpEngineModule::World* world,
        const ImpEngineModule::Event::LoadedWorld& event) override;


private:
    ImpEditor* _editor               = nullptr;
    Shortcut* _shortcut              = nullptr;
    ToolCamera* _toolCamera          = nullptr;
    Debug::DebugViewer* _debugViewer = nullptr;

    ImpEngineModule::World* _world                     = nullptr;
    ImpEngineModule::WorldManager* _worldManager       = nullptr;
    ImpEngineModule::ResourceManager* _resourceManager = nullptr;
    ImpEngineModule::TimeManager* _timeManager         = nullptr;
    ImpEngineModule::EventManager* _eventManager       = nullptr;
    ImpGraphics::IImpGraphicsEngine* _graphicsEngine   = nullptr;

    ImpEngineModule::Physics::TagMap _tagMap{};
    ImpReflection::EnumClassType<ImpEngineModule::Physics::Tag>* _tagType = nullptr;

    std::wstring _currentWorldName;
    std::string _inputWorldName;
    bool _isOpenCreatePopup = false;
    bool _isSaveWorld       = false;

    bool _onAxis               = true;
    bool _onGrid               = true;
    bool _onWireFrame          = false;
    bool _onCollider           = true;
    bool _onLightGizumo        = false;
    bool _onCameraGizumo       = false;
    bool _onCopyBoundingVolume = true;

    bool _isOpenTagWindow = false;

    bool _isOpenCubeMapWindow  = false;
    bool _isCloseCubeMapWindow = false;
};


} // namespace ImpToolModule
