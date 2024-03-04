#pragma once

namespace ImpEngineModule
{
	// namespace로 접근해서 경로에 관한 함수들을 호출
	namespace PathManager
	{
		// ResourcesPath 경로를 반환
		std::filesystem::path GetResourcesPath();

		// EntityData 경로를 반환
		std::filesystem::path GetEntityDataPath();

		// WorldData 경로를 반환
		std::filesystem::path GetWorldDataPath();

		// WorlData 내부의 월드 파일들의 경로를 vector담아서 반환
		std::vector<std::filesystem::path> GetWorldDataList();

		// 월드의 이름으로 월드의 경로얻는다.
		std::filesystem::path GetWorldPath(const std::wstring& worldName);

		// EntityData 내부의 Entity 들의 경로를 vector담아서 반환
		std::vector<std::filesystem::path> GetEntityDataList();
		
		// 현재 경로에 있는 내부 파일 리스트를 가져온다          
		std::vector<std::filesystem::path> GetDirectoryList(const std::filesystem::path& path);
		 
		// <summary>
		// 메모리에 로드할 Entity들의 경로들을 불러온다.
		// </summary>
		// <param name="worldPath">로드할 월드의 경로</param>
		std::vector<std::filesystem::path> GetLoadEntityList(const std::filesystem::path& worldPath);

		// World 시작전에 로드할 Entity들의 List를 가져온다.
		std::vector<std::filesystem::path> GetStartEntityList(const std::filesystem::path& worldPath);

		// 경로중에서 .../Resources/ 를 지운다.
		void EraseResourcePath(std::wstring& path);

		// 경로와 확장자는 동일하고 파일의 이름을 바꾼다 
		std::wstring ChangeFileName(const std::wstring& path, const std::wstring& name);

		// 경로에서 확장자를 가져온다 .txt, .ent 등등 없으면 빈 스트링반환
		std::wstring GetExtension(const std::wstring& path);
	}

}