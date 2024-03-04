#include "ImpToolModulePCH.h"
#include "WorldViewer.h"
#include "ImpEditor.h"
#include "DebugViewer.h"
#include "ToolCamera.h"
#include "Shortcut.h"

ImpToolModule::WorldViewer::WorldViewer()
{
}

ImpToolModule::WorldViewer::~WorldViewer()
{
}


void ImpToolModule::WorldViewer::Initialize(
    ImpEngineModule::EngineModule* engineModule,
    ImpGraphics::IImpGraphicsEngine* graphicsEngine,
    ImpEditor* editor,
    Shortcut* shortcut,
    Debug::DebugViewer* debugViewer,
    ToolCamera* toolCamera)
{
    _toolCamera       = toolCamera;
    _debugViewer      = debugViewer;
    _editor           = editor;
    _graphicsEngine   = graphicsEngine;
    _world            = engineModule->GetWorld();
    _resourceManager  = engineModule->GetResourceManager();
    _worldManager     = engineModule->GetWorldManager();
    _timeManager      = engineModule->GetTimeManager();
    _eventManager     = engineModule->GetEventManager();
    _currentWorldName = _worldManager->GetWorldName();
    _shortcut         = shortcut;

    _eventManager->SubScribe(this);

    ImpReflection::TypeManager* typeManager = ImpReflection::TypeManager::GetInstance();
    _tagType = dynamic_cast<ImpReflection::EnumClassType<ImpEngineModule::Physics::Tag>*>(
        typeManager->GetType(
            std::type_index(typeid(ImpEngineModule::Physics::Tag))));
}


void ImpToolModule::WorldViewer::Update()
{
    _currentWorldName = _worldManager->GetWorldName();

    if (ImGui::BeginMainMenuBar())
    {
        EditMenu();
        ShowEnviromentMenu();

        SetGizmo();
        ShowMode();
        ShowWorldNameToText();

        ImGui::EndMainMenuBar();
    }
}

void ImpToolModule::WorldViewer::ShowWorldNameToText()
{
    std::string showText = "FPS " + std::to_string(_timeManager->GetFPS()) +
                           " <";
    std::filesystem::path worldName(_currentWorldName);
    showText += worldName.string();
    showText += ">";

    float nameX = ImGui::GetWindowSize().x -
                  ImGui::CalcTextSize(showText.c_str()).x;

    ImGui::SetCursorPosX(nameX);
    ImGui::PushStyleColor(ImGuiCol_Text, ImGuiColor::SPRING_GREEN);
    ImGui::Text(showText.c_str());
    ImGui::PopStyleColor();
}


void ImpToolModule::WorldViewer::ShowMode()
{
    std::string modeName;
    if (_editor->GetMode() == ToolMode::EditorMode)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGuiColor::YELLOW);
        modeName = "EditorMode";
    }
    else if (_editor->GetMode() == ToolMode::GamePlayMode)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGuiColor::SPRING_GREEN);
        modeName = "GamePlayMode";
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGuiColor::RED);
        modeName = "PauseMode";
    }

    if (_toolCamera->IsActive())
    {
        modeName += " [Tool Camera]";
    }
    else
    {
        modeName += " [Game Camera]";
    }

    ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f -
                         ImGui::CalcTextSize(modeName.c_str()).x * 0.5f);

    ImGui::Text(modeName.c_str());
    ImGui::PopStyleColor();
}

void ImpToolModule::WorldViewer::ShowEnviromentMenu()
{
    if (ImGui::BeginMenu("Environment"))
    {
        if (ImGui::MenuItem("CubMap##button"))
        {
            _isOpenCubeMapWindow = !_isOpenCubeMapWindow;
        }

        if (ImGui::MenuItem("TagMap##button"))
        {
            _isOpenTagWindow = !_isOpenTagWindow;
        }

        ImGui::EndMenu();
    }

    // CubeMapWindow
    if (_isOpenCubeMapWindow && ImGui::Begin("CubeMap", &_isOpenCubeMapWindow))
    {

        ImGui::Text("Please provide the CubeMap path here!");

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload(
                "ResourcePath");

            if (payLoad)
            {
                const wchar_t* receiveData = static_cast<const wchar_t*>(
                    payLoad->Data);

                // 확장자를 확인한다
                std::filesystem::path path(receiveData);

                _resourceManager->LoadCubeMap(path);
            }
        }
        ImGui::End();
    }

    // TagMap Window
    if (_isOpenTagWindow && ImGui::Begin("TagMap", &_isOpenTagWindow))
    {
        if (ImGui::Button("Reset##empty", ImVec2(133, 25)))
        {
            _tagMap.Reset();
        }

        const auto& enumMembers = _tagType->GetEnumMembers();

        for (size_t i = 0; i < enumMembers.size(); ++i)
        {
            ImGui::SameLine();
            std::string row = std::to_string(i) + "##" + enumMembers[i]._name;
            ImGui::Button(row.c_str(), ImVec2(25, 25));
        }

        for (size_t i = 0; i < enumMembers.size(); ++i)
        {
            std::string name = enumMembers[i]._name;
            ImGui::Button(name.c_str(), ImVec2(100, 25));
            ImGui::SameLine();

            std::string index = std::to_string(i) + "##col" + std::to_string(i);
            ImGui::Button(index.c_str(), ImVec2(25, 25));
            for (int j = 0; j < enumMembers.size(); ++j)
            {
                bool isCheck = _tagMap.tagCheck[i][j];

                std::string buttonName;
                if (isCheck)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGuiColor::RED);
                    buttonName = "O";
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGuiColor::DARK_GRAY);
                    buttonName = "X";
                }
                buttonName += "##" + enumMembers[i]._name + std::to_string(i) +
                              std::to_string(j);
                ImGui::SameLine();
                if (ImGui::Button(buttonName.c_str(), ImVec2(25, 25)))
                {
                    _tagMap.tagCheck[i][j].flip();
                    _tagMap.tagCheck[j][i] = _tagMap.tagCheck[i][j];
                }
                ImGui::PopStyleColor();
            }
        }

        ImGui::End();
    }
}

void ImpToolModule::WorldViewer::EditMenu()
{
    if (_editor->GetMode() == ToolMode::EditorMode)
    {
        if (ImGui::BeginMenu("File"))
        {
            CreateWorldButton();
            LoadWorldButton();
            SaveWorldButton();
            ImGui::EndMenu();
        }

        if (_isOpenCreatePopup)
        {
            ImGui::OpenPopup("Enter World Name");

            if (ImGui::BeginPopupModal(
                    "Enter World Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::InputText("World Name", &_inputWorldName);

                if (ImGui::Button("Create"))
                {
                    // 월드를 생성한다.
                    ImGui::CloseCurrentPopup();
                    _isOpenCreatePopup = false;
                    CreateWorld();
                }

                ImGui::SameLine();

                // ccad
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                    _isOpenCreatePopup = false;
                }

                ImGui::EndPopup();
            }
        }
    }
}

void ImpToolModule::WorldViewer::SaveWorldButton()
{
    if (ImGui::BeginMenu("Save"))
    {
        if (ImGui::Button("SaveWorld"))
        {
            SaveWorld();
            _isSaveWorld = true;
        }

        ImGui::EndMenu();
    }

    // 세이브에 성공하면 팝업창 띄우기
    if (_isSaveWorld)
    {
        ImGui::SetNextWindowSize(ImVec2(200.f, 50.f));
        ImGui::OpenPopup("Save Successed!!");

        if (ImGui::BeginPopupModal(
                "Save Successed!!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
                _isSaveWorld = false;
            }

            ImGui::EndPopup();
        }
    }
}

void ImpToolModule::WorldViewer::LoadWorldButton()
{
    using namespace ImpEngineModule;

    if (ImGui::BeginMenu("Load"))
    {
        auto worldList = PathManager::GetWorldDataList();

        for (auto& path : worldList)
        {
            std::string worldName = path.filename().string();

            if (ImGui::MenuItem(worldName.c_str()))
            {
                _worldManager->ChangeWorld(path.filename());
            }
        }

        ImGui::EndMenu();
    }
}

void ImpToolModule::WorldViewer::CreateWorldButton()
{
    if (ImGui::BeginMenu("Create"))
    {
        if (ImGui::MenuItem("CreateWorld"))
        {
            _isOpenCreatePopup = true;
        }

        ImGui::EndMenu();
    }
}

void ImpToolModule::WorldViewer::CreateWorld()
{
    // 1. 월드를 폴더를 생성한다.
    using namespace ImpEngineModule;
    namespace fs = std::filesystem;

    auto path = PathManager::GetWorldDataPath();
    path += "/";
    path += _inputWorldName;

    fs::create_directory(path);

    // LoadEntityList.txt 파일생성
    fs::path listPath = path;
    listPath += "/LoadEntityList.txt";
    std::wofstream loadList(listPath);

    // StartEntity 폴더 생성
    path += "/StartEntity";
    fs::create_directory(path);

    _currentWorldName = fs::path(_inputWorldName);
    _worldManager->ChangeWorld(_currentWorldName);
}

void ImpToolModule::WorldViewer::SaveWorld()
{
    using namespace ImpEngineModule;
    namespace fs = std::filesystem;

    /// 1.월드 경로를 생성한다.
    fs::path worldPath = PathManager::GetWorldDataPath();
    worldPath += "/";
    worldPath += _currentWorldName;

    /// 2. 큐브맵 저장
    SaveCubeMap(worldPath);

    /// 3. TagMap 저장
    SaveTagMap(worldPath);

    /// 3. 리소스들을 저장한다.
    SaveResources(worldPath);

    /// 4. 원래있던 Entity들을 지우고 새로 생성한다.
    worldPath += "/StartEntity/";

    for (const auto& entry : fs::directory_iterator(worldPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".ent")
        {
            fs::remove(entry.path());
        }
    }

    for (const std::shared_ptr<Entity>& ent : _world->GetEntities())
    {
        if (ent->GetState() == Entity::EntityState::Active)
        {
            Save::SaveEntity(worldPath, ent.get());
        }
    }
}

void ImpToolModule::WorldViewer::Finalize()
{
    _eventManager->UnSubScribe<ImpEngineModule::Event::LoadedWorld>(this);
}

void ImpToolModule::WorldViewer::SaveCubeMap(const std::filesystem::path& worldPath)
{
    ImpEngineModule::CubeMap* cube = _resourceManager->GetCubeMap();

    std::filesystem::path cubePath = worldPath;
    cubePath += "\\CubeMap.txt";

    std::wofstream cubeText(cubePath);

    assert(cubeText.is_open() && "파일열기 실패");

    std::wstring savePath = cube->GetPath();
    ImpEngineModule::PathManager::EraseResourcePath(savePath);

    cubeText << savePath;

    cubeText.close();
}

void ImpToolModule::WorldViewer::SaveTagMap(const std::filesystem::path& worldPath)
{
    std::filesystem::path tagMapPath = worldPath;
    tagMapPath += "\\TagMap.txt";

    std::ofstream tagText(tagMapPath);
    assert(tagText.is_open() && "파일열기 실패");

    size_t size = static_cast<size_t>(ImpEngineModule::Physics::Tag::END);

    for (size_t i = 0; i < size; ++i)
    {
        tagText << _tagMap.tagCheck[i].to_string() << '\n';
    }

    tagText.close();
}

void ImpToolModule::WorldViewer::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::LoadedWorld& event)
{
    _tagMap = _resourceManager->GetTagMapResource()->GetTagMap();
}

void ImpToolModule::WorldViewer::SaveResources(const std::filesystem::path& path)
{
    using namespace ImpReflection;
    using namespace ImpEngineModule;
    // 1. Entity들이 들고있는 경로 정보를 모두가져온다

    TypeManager* typeMgr = TypeManager::GetInstance();

    std::vector<std::wstring> pathResources;
    std::queue<std::wstring> entResources;

    for (const auto& ent : _world->GetEntities())
    {
        for (auto& component : ent->GetComponents())
        {
            Type* type = typeMgr->GetType(component.first);

            // 재귀적으로 경로정보를 가져온다.
            FindPathResource(component.second, type, pathResources, entResources);
        }
    }

    // 만약 리소스 내부에 리소스가 있다면 재귀적으로 모두 가져온다
    while (!entResources.empty())
    {
        std::wstring path = entResources.front();
        entResources.pop();

        ImpEngineModule::EntityResource res(path, 1);
        res.Load();
        auto ent = res.GetEntity();

        for (auto& component : ent->GetComponents())
        {
            Type* type = typeMgr->GetType(component.first);

            // 재귀적으로 경로정보를 가져온다.
            FindPathResource(component.second, type, pathResources, entResources);
        }
    }


    // 2. 경로가 비어있다면 저장하지 않는다
    pathResources.erase(
        std::remove_if(pathResources.begin(),
            pathResources.end(),
            [](std::wstring path)
            {
                if (path.empty()) return true;
                return false;
            }),
        pathResources.end());


    // 3. 경로들을 모두 상대경로로 바꿔줘야한다.
    for (auto& path : pathResources)
    {
        PathManager::EraseResourcePath(path);
    }

    // 4. 파일 형식에 따라서 분류해서 저장한다.
    SaveEntityResources(pathResources, path);

    // 5. 사운드 저장
    SaveSoundList(pathResources, path);
}

void ImpToolModule::WorldViewer::SaveSoundList(
    const std::vector<std::wstring>& pathResources,
    const std::filesystem::path& currentWorldPath)
{
    std::filesystem::path soundListPath = currentWorldPath;
    soundListPath += "/SoundList.txt";

    std::wofstream loadSoundText(soundListPath);

    assert(loadSoundText.is_open() && "파일열기 실패");
    for (auto& path : pathResources)
    {
        // .ent 확장자만 저장을 한다
        if (path.substr(path.find_last_of('.')) == L".mp3" ||
            path.substr(path.find_last_of('.')) == L".wav")
        {
            loadSoundText << path << '\n';
        }
    }

    loadSoundText.close();
}

void ImpToolModule::WorldViewer::SaveEntityResources(
    const std::vector<std::wstring>& pathResources,
    const std::filesystem::path& currentWorldPath)
{
    std::filesystem::path loadEntityPath = currentWorldPath;
    loadEntityPath += "/LoadEntityList.txt";

    std::wofstream loadEntityText(loadEntityPath);

    assert(loadEntityText.is_open() && "파일열기 실패");
    for (auto& path : pathResources)
    {
        // .ent 확장자만 저장을 한다
        if (path.substr(path.find_last_of('.')) != L".ent")
        {
            continue;
        }

        loadEntityText << path << '\n';
    }
    loadEntityText.close();
}


void ImpToolModule::WorldViewer::SetGizmo()
{
    if (ImGui::BeginMenu("Gizmo"))
    {
        if (ImGui::Checkbox("Axis", &_onAxis))
        {
            _graphicsEngine->SetAxis(_onAxis);
        }

        if (ImGui::Checkbox("Grid", &_onGrid))
        {
            _graphicsEngine->SetGrid(_onGrid);
        }

        if (ImGui::Checkbox("WireFrame", &_onWireFrame))
        {
            _graphicsEngine->SetWireFrame(_onWireFrame);
        }

        if (ImGui::Checkbox("Collider", &_onCollider))
        {
            _debugViewer->SetShowCollider(_onCollider);
        }

        if (ImGui::Checkbox("LightGizumo", &_onLightGizumo))
        {
            _debugViewer->SetShowLightGizumo(_onLightGizumo);
        }

        if (ImGui::Checkbox("CameraGizumo", &_onCameraGizumo))
        {
            _debugViewer->SetShowCameraGizumo(_onCameraGizumo);
        }

        if (ImGui::Checkbox("CopyBoundingVolume", &_onCopyBoundingVolume))
        {
            _shortcut->SetShowCopyBoundingVolume(_onCopyBoundingVolume);
        }

        ImGui::EndMenu();
    }
}


void ImpToolModule::WorldViewer::FindPathResource(void* object,
    ImpReflection::Type* type,
    std::vector<std::wstring>& resources,
    std::queue<std::wstring>& entResources)
{
    using namespace ImpReflection;
    switch (type->GetTypeCategory())
    {
        case TypeCategory::Class:
        {
            ClassType* classType = reinterpret_cast<ClassType*>(type);
            for (auto& member : classType->GetMembers())
            {
                FindPathResource((char*)object + member._offset,
                    member._type,
                    resources,
                    entResources);
            }
        }
        break;
        case TypeCategory::Primitive: // wstring 은 일단 다 담아온다.
        {
            if (type->GetFullName() == "std::wstring")
            {
                std::wstring* data = reinterpret_cast<std::wstring*>(object);
                resources.push_back(data->c_str());

                std::filesystem::path path(data->c_str());
                if (path.extension() == L".ent")
                {
                    entResources.push(data->c_str());
                }
            }
            break;
        }
    }
}
