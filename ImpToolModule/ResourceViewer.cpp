#include "ImpToolModulePCH.h"
#include "ResourceViewer.h"


ImpToolModule::ResourceViewer::~ResourceViewer()
{
}

void ImpToolModule::ResourceViewer::Initalize()
{
    _resourcesPath = ImpEngineModule::PathManager::GetResourcesPath();
    _currentPath   = ImpEngineModule::PathManager::GetResourcesPath();
}

void ImpToolModule::ResourceViewer::Update()
{
    using namespace ImpEngineModule;

    if (ImGui::Begin("ResourceViewer"))
    {
        _viewerWidth    = ImGui::GetWindowWidth();
        _buttonCount    = 0;
        _maxButtonCount = static_cast<unsigned int>(
            _viewerWidth / _fileButtonSize.x);


        ViewProjectTabBar();


        ImGui::End();
    }
}

void ImpToolModule::ResourceViewer::ViewProjectTabBar()
{
    using namespace ImpEngineModule;

    auto dataList = PathManager::GetDirectoryList(_currentPath);

    ViewEditFile();
    ViewShortPath();

    ViewParentDirectory();
    // ����
    for (auto& path : dataList)
    {
        if (std::filesystem::is_directory(path))
        {
            ViewDirectory(path);
            DropFile(path);
            SameLine();
        }
    }

    // ���ҽ�
    for (auto& path : dataList)
    {
        if (!std::filesystem::is_directory(path))
        {
            ViewResource(path);
            SameLine();
        }
    }
}

void ImpToolModule::ResourceViewer::ViewEditFile()
{
    // ���� �߰� ��ư
    if (ImGui::Button("Add"))
    {
        _isOpenDeletePopup = true;
    }

    if (_isOpenDeletePopup)
    {
        ImGui::OpenPopup("Enter Directory Name");

        if (ImGui::BeginPopupModal(
                "Enter Directory Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {

            ImGui::InputText("Directory Name", &_newDirectoryName);

            if (ImGui::Button("Create"))
            {
                std::filesystem::path directoryPath = _currentPath.string() + '/';
                directoryPath += _newDirectoryName;

                std::filesystem::create_directory(directoryPath);

                ImGui::CloseCurrentPopup();
                _isOpenDeletePopup = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
                _isOpenDeletePopup = false;
            }

            ImGui::EndPopup();
        }
    }
    ImGui::SameLine();

    DeleteFileButton();

    ImGui::SameLine();
}

void ImpToolModule::ResourceViewer::ViewShortPath()
{
    if (ImGui::Button("Resource"))
    {
        _currentPath = _resourcesPath;
    }
    ImGui::SameLine();

    if (ImGui::Button("Prefab##1234"))
    {
        _currentPath = _resourcesPath;
        _currentPath += L"\\EntityData";
    }

    ImGui::SameLine();

    if (ImGui::Button("Mesh##1234"))
    {
        _currentPath = _resourcesPath;
        _currentPath += L"\\Mesh";
    }

    ImGui::SameLine();

    if (ImGui::Button("Shader##1234"))
    {
        _currentPath = _resourcesPath;
        _currentPath += L"\\Shader";
    }

    ImGui::Separator();
}

void ImpToolModule::ResourceViewer::ViewDirectory(const std::filesystem::path& path)
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImGuiColor::FILE_COLOR);

    std::string directory = path.filename().string();
    if (ImGui::Button(directory.c_str(), _fileButtonSize))
    {
        _currentPath = path;
    }
    ImGui::PopStyleColor();

    std::string entName  = path.string();
    std::wstring entPath = path.wstring();

    // �巡�� �ؼ� ��θ� �����Ѵ�.
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
    {
        ImGui::Text(entName.c_str());
        ImGui::SetDragDropPayload("ResourcePath",
            entPath.c_str(),
            (entPath.size() + 1) * sizeof(wchar_t));
        ImGui::EndDragDropSource();
    }
}

void ImpToolModule::ResourceViewer::ViewResource(const std::filesystem::path& path)
{
    // Ȯ���ڿ� ���� ������ �з��Ѵ�.
    std::wstring extension = path.extension().c_str();

    std::string entName  = path.filename().string();
    std::wstring entPath = path.wstring();

    PushExtensionColor(extension);
    ImGui::Button(entName.c_str(), _fileButtonSize);
    ImGui::PopStyleColor();

    // �巡�� �ؼ� ��θ� �����Ѵ�.
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
    {
        ImGui::Text(entName.c_str());
        ImGui::SetDragDropPayload("ResourcePath",
            entPath.c_str(),
            (entPath.size() + 1) * sizeof(wchar_t));
        ImGui::EndDragDropSource();
    }
}

void ImpToolModule::ResourceViewer::ViewParentDirectory()
{
    if (_currentPath == _resourcesPath)
    {
        return;
    }

    ImGui::PushStyleColor(ImGuiCol_Button, ImGuiColor::FILE_DARK_COLOR);

    // �θ��� ��ư
    if (ImGui::Button("...", _fileButtonSize))
    {
        _currentPath = _currentPath.parent_path();
    }
    DropFile(_currentPath.parent_path());
    SameLine();
    ImGui::PopStyleColor();
}

void ImpToolModule::ResourceViewer::DropFile(const std::filesystem::path& destination)
{
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload(
            "ResourcePath");

        if (payLoad)
        {
            const wchar_t* receiveData = static_cast<const wchar_t*>(payLoad->Data);

            std::filesystem::path path(receiveData);

            std::filesystem::path newPath = destination;
            newPath += '/';
            newPath += path.filename();

            try
            {
                std::filesystem::rename(path, newPath);
            } catch (std::exception& e)
            {
                std::string s = e.what();
            }
        }
    }
}

void ImpToolModule::ResourceViewer::PushExtensionColor(const std::wstring extension)
{
    // Entity
    if (extension == L".ent")
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGuiColor::OLIVE);
    }
    // Shader
    else if (extension == L".cso")
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGuiColor::COFFEE);
    }
    // FBX
    else if (extension == L".fbx")
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGuiColor::STEEL_GRAY);
    }
    // ��Ÿ ���
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGuiColor::DARK_GRAY);
    }
}

void ImpToolModule::ResourceViewer::DeleteFileButton()
{
    if (ImGui::Button("Delete"))
    {
    }
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload(
            "ResourcePath");

        if (payLoad)
        {
            const wchar_t* receiveData = static_cast<const wchar_t*>(payLoad->Data);

            std::filesystem::path path(receiveData);

            // ���� ������ �������� Ȯ��
            std::filesystem::file_status status = std::filesystem::status(path);

            if (std::filesystem::exists(status) &&
                std::filesystem::is_regular_file(status))
            {
                std::filesystem::remove(path);
            }
            else if (std::filesystem::is_directory(path)) // ���������ΰ��
            {
                std::filesystem::directory_iterator iter(path);
                if (iter ==
                    std::filesystem::end(iter)) // ���ϳ��ο� �ƹ��͵� ���°��
                {
                    std::filesystem::remove(path);
                }
            }
        }
    }
}

void ImpToolModule::ResourceViewer::SameLine()
{
    ++_buttonCount;
    if (_buttonCount >= _maxButtonCount)
    {
        _buttonCount = 0;
    }
    else
    {
        ImGui::SameLine();
    }
}
