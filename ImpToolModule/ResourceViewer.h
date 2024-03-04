#pragma once


namespace ImpToolModule
{
	/// <summary>
	/// ���ҽ� �����͸� �����ϴ� GUI
	/// 
	/// </summary>
	class ResourceViewer
	{
	public:
		ResourceViewer() = default;
		~ResourceViewer();

		void Initalize();
		void Update();
		std::filesystem::path GetCurrentResourcePath()const { return _currentPath; }

	private:
		void ViewProjectTabBar();

		// ���� ���� �߰�ó�� 
		void ViewEditFile();

		// ���� ���
		void ViewShortPath();
		
		// ����
		void ViewDirectory(const std::filesystem::path& path);
		
		// ���ҽ� 
		void ViewResource(const std::filesystem::path& path);
		
		// ... ���� 
		void ViewParentDirectory();

		// ���ϵ���� ó���Ѵ�.
		void DropFile(const std::filesystem::path& destination);

		// ��ư�� ������ Ȯ���ڿ� ���� �ٲ۴�.
		void PushExtensionColor(const std::wstring extension);
		
		// ���� ����
		void DeleteFileButton();

		void SameLine();
	private:
		std::filesystem::path _resourcesPath;
		std::filesystem::path _currentPath;
				
		const ImVec2 _fileButtonSize = { 125.f,125.f }; 
		unsigned int _buttonCount =0;
		unsigned int _maxButtonCount =0;
		float _viewerWidth =0.f;
		
		std::string _newDirectoryName;
		bool _isOpenDeletePopup = false;
	};

}