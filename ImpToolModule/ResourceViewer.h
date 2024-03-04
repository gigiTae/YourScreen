#pragma once


namespace ImpToolModule
{
	/// <summary>
	/// 리소스 데이터를 관리하는 GUI
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

		// 파일 삭제 추가처리 
		void ViewEditFile();

		// 현재 경로
		void ViewShortPath();
		
		// 폴더
		void ViewDirectory(const std::filesystem::path& path);
		
		// 리소스 
		void ViewResource(const std::filesystem::path& path);
		
		// ... 파일 
		void ViewParentDirectory();

		// 파일드랍을 처리한다.
		void DropFile(const std::filesystem::path& destination);

		// 버튼의 색깔을 확장자에 따라서 바꾼다.
		void PushExtensionColor(const std::wstring extension);
		
		// 파일 삭제
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