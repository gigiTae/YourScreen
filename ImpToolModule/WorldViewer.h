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
/// 월드의 관련된 정보를 표시하는 GUI
/// </summary>
class WorldViewer:
    ImpEngineModule::EventSubscriber<ImpEngineModule::Event::LoadedWorld>
{
public:
    WorldViewer();
    ~WorldViewer();

public:
    /// 초기화
    void Initialize(ImpEngineModule::EngineModule* engineModule,
        ImpGraphics::IImpGraphicsEngine* graphicsEngine,
        ImpEditor* editor,
        Shortcut* shortcut,
        Debug::DebugViewer* debugViewer,
        ToolCamera* toolCamera);

    /// 월드의 GUI를 업데이트한다.
    void Update();

    /// 월드 저장
    void SaveWorld();

    void Finalize();

private:
    void ShowMode();

    void ShowEnviromentMenu();

    /// 에디트 메뉴
    void EditMenu();

    /// 현재 월드를 저장한다.
    void SaveWorldButton();
    /// 로드 월드 버튼
    void LoadWorldButton();
    /// 월드 생성버튼
    void CreateWorldButton();

    /// 새로운 월드를 생성한다.
    void CreateWorld();

    /// 리소스 저장;
    void SaveResources(const std::filesystem::path& path);

    /// 컴포넌트 정보를 순회하면서 들고있는 경로 정보를 벡터에 담는다.
    void FindPathResource(void* object,
        ImpReflection::Type* type,
        std::vector<std::wstring>& resources,
        std::queue<std::wstring>& entResources);

    // 현재 월드의 이름을 표시한다.
    void ShowWorldNameToText();

    // 엔티티 리소를 저장한다
    void SaveEntityResources(const std::vector<std::wstring>& pathResources,
        const std::filesystem::path& currentWorldPath);

    // 기즈모 세팅
    void SetGizmo();

    // 사운드 저장
    void SaveSoundList(const std::vector<std::wstring>& pathResources,
        const std::filesystem::path& currentWorldPath);

    // 큐브맵 세이브
    void SaveCubeMap(const std::filesystem::path& worldPath);

    // TagMap 세이브
    void SaveTagMap(const std::filesystem::path& worldPath);

    // 로드 월드 콜백
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
