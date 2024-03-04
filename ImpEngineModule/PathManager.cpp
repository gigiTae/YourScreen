#include "EngineModulePCH.h"
#include "PathManager.h"

std::filesystem::path ImpEngineModule::PathManager::GetResourcesPath()
{
    std::filesystem::path path = std::filesystem::current_path();
    path += "\\Resources";

    return path;
}

std::filesystem::path ImpEngineModule::PathManager::GetEntityDataPath()
{
    std::filesystem::path path = std::filesystem::current_path();
    path += "/Resources/EntityData";

    return path;
}

std::filesystem::path ImpEngineModule::PathManager::GetWorldDataPath()
{
    std::filesystem::path path = std::filesystem::current_path();
    path += "/Resources/WorldData";

    return path;
}

std::vector<std::filesystem::path> ImpEngineModule::PathManager::GetWorldDataList()
{
    std::filesystem::path worldPath = GetWorldDataPath();
    std::vector<std::filesystem::path> worldPaths;

    for (const auto& entry : std::filesystem::directory_iterator(worldPath))
    {
        worldPaths.push_back(entry.path());
    }

    return worldPaths;
}

std::filesystem::path ImpEngineModule::PathManager::GetWorldPath(
    const std::wstring& worldName)
{
    std::filesystem::path path = GetWorldDataPath();
    path += "/";
    path += worldName;

    return path;
}

std::vector<std::filesystem::path> ImpEngineModule::PathManager::GetEntityDataList()
{
    std::filesystem::path worldPath = GetEntityDataPath();
    std::vector<std::filesystem::path> worldPaths;

    for (const auto& entry : std::filesystem::directory_iterator(worldPath))
    {
        worldPaths.push_back(entry.path());
    }

    return worldPaths;
}

std::vector<std::filesystem::path> ImpEngineModule::PathManager::GetDirectoryList(
    const std::filesystem::path& path)
{
    std::vector<std::filesystem::path> worldPaths;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        worldPaths.push_back(entry.path());
    }

    return worldPaths;
}

std::vector<std::filesystem::path> ImpEngineModule::PathManager::GetLoadEntityList(
    const std::filesystem::path& worldPath)
{
    std::filesystem::path loadEntityListPath = worldPath;
    loadEntityListPath += "/LoadEntityList.txt";

    std::vector<std::filesystem::path> loadEntityList;

    std::wifstream inputFile(loadEntityListPath);

    if (inputFile.is_open())
    {
        std::wstring line;
        while (std::getline(inputFile, line))
        {
            loadEntityList.push_back(line);
        }
    }
    else
    {
        assert(!"파일열기 실패");
    }

    return loadEntityList;
}

std::vector<std::filesystem::path> ImpEngineModule::PathManager::GetStartEntityList(
    const std::filesystem::path& worldPath)
{
    std::filesystem::path startEntityListPath = worldPath;
    startEntityListPath += "/StartEntity/";

    std::vector<std::filesystem::path> startEntityList;


    for (const auto& entry :
        std::filesystem::directory_iterator(startEntityListPath))
    {
        startEntityList.push_back(entry.path());
    }


    return startEntityList;
}

void ImpEngineModule::PathManager::EraseResourcePath(std::wstring& path)
{
    // 혹시모를 / \\ 등같은 경로가있을 수 있으니 정규화한다.
    std::filesystem::path resouresPath = GetResourcesPath();
    std::filesystem::path currentPath(path);

    if (currentPath.is_relative())
    {
        return;
    }

    std::wstring relativePath = currentPath.wstring().substr(
        resouresPath.wstring().size());

    path = relativePath;
}

std::wstring ImpEngineModule::PathManager::ChangeFileName(const std::wstring& path,
    const std::wstring& name)
{
    std::filesystem::path currentPath(path);

    std::wstring changedName = currentPath.parent_path();

    changedName += L"\\" + name + currentPath.extension().wstring();

    return changedName;
}

std::wstring ImpEngineModule::PathManager::GetExtension(const std::wstring& path)
{
    size_t start = path.find_last_of('.');

    if (start == std::wstring::npos)
    {
        return std::wstring();
    }

    return path.substr(start);
}
