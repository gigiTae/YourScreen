#pragma once

namespace ImpEngineModule
{
	// namespace�� �����ؼ� ��ο� ���� �Լ����� ȣ��
	namespace PathManager
	{
		// ResourcesPath ��θ� ��ȯ
		std::filesystem::path GetResourcesPath();

		// EntityData ��θ� ��ȯ
		std::filesystem::path GetEntityDataPath();

		// WorldData ��θ� ��ȯ
		std::filesystem::path GetWorldDataPath();

		// WorlData ������ ���� ���ϵ��� ��θ� vector��Ƽ� ��ȯ
		std::vector<std::filesystem::path> GetWorldDataList();

		// ������ �̸����� ������ ��ξ�´�.
		std::filesystem::path GetWorldPath(const std::wstring& worldName);

		// EntityData ������ Entity ���� ��θ� vector��Ƽ� ��ȯ
		std::vector<std::filesystem::path> GetEntityDataList();
		
		// ���� ��ο� �ִ� ���� ���� ����Ʈ�� �����´�          
		std::vector<std::filesystem::path> GetDirectoryList(const std::filesystem::path& path);
		 
		// <summary>
		// �޸𸮿� �ε��� Entity���� ��ε��� �ҷ��´�.
		// </summary>
		// <param name="worldPath">�ε��� ������ ���</param>
		std::vector<std::filesystem::path> GetLoadEntityList(const std::filesystem::path& worldPath);

		// World �������� �ε��� Entity���� List�� �����´�.
		std::vector<std::filesystem::path> GetStartEntityList(const std::filesystem::path& worldPath);

		// ����߿��� .../Resources/ �� �����.
		void EraseResourcePath(std::wstring& path);

		// ��ο� Ȯ���ڴ� �����ϰ� ������ �̸��� �ٲ۴� 
		std::wstring ChangeFileName(const std::wstring& path, const std::wstring& name);

		// ��ο��� Ȯ���ڸ� �����´� .txt, .ent ��� ������ �� ��Ʈ����ȯ
		std::wstring GetExtension(const std::wstring& path);
	}

}