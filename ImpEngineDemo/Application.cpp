#include "pch.h"
#include "Application.h"
#include "../ImpGameEngine/IImpEngine.h"
#include <fstream>
#include <string>
#include <cassert>

Application::Application()
	:_gameEngine{}, _info()
{


}

Application::~Application()
{
}


void Application::Initialize(HINSTANCE hInstance, Mode mode)
{
	_info._hInstance = hInstance;

	if (mode == Mode::Game)
	{
		// GameEngine 시작
		_gameEngine = ImpGameEngine::EngineExporter::GetGameEngine();
	}
	else if (mode == Mode::Tool)
	{
		// ToolEngine 시작
		_gameEngine = ImpGameEngine::EngineExporter::GetToolEngine();

		LoadScreenInfomation();
	}

	// Window 초기화
	InitializeWindow(hInstance, mode);

	_gameEngine->Initialize(_info);
}

void Application::Process()
{
	_gameEngine->Process();
}

void Application::Finalize()
{
	_gameEngine->Finalize();

	ImpGameEngine::EngineExporter::DeleteEngine();
}


void Application::InitializeWindow(HINSTANCE hInstance, Mode mode)
{
	WNDCLASS wndClass{};

	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	if (mode == Mode::Game)
	{
		wndClass.lpfnWndProc = ImpGameEngine::WindowInfomation::WndProc;
	}
	else if (mode == Mode::Tool)
	{
		wndClass.lpfnWndProc = ImpGameEngine::WindowInfomation::ToolWndProc;
	}
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = _info._title;

	RegisterClass(&wndClass);

	RECT rect{ _info._screenLeft, _info._screenTop,
		_info._screenLeft + _info._screenWidth, _info._screenTop + _info._screenHeight };

	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	_info._screenTop = rect.top;
	_info._screenLeft = rect.left;
	_info._screenWidth = rect.right - rect.left;
	_info._screenHeight = rect.bottom - rect.top;

	_info._hWnd = CreateWindow(_info._title, _info._title
		, WS_OVERLAPPEDWINDOW // 창이 겹침
		, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top
		, NULL, NULL, hInstance, NULL);
	
	assert(_info._hWnd);

	ShowWindow(_info._hWnd, SW_SHOWNORMAL);
	UpdateWindow(_info._hWnd);
}

void Application::LoadScreenInfomation()
{
	std::ifstream screenInfo("screenPosition");

	if (screenInfo.is_open())
	{
		std::string line{};
		try {
			std::getline(screenInfo, line);
			_info._screenLeft = std::stol(line.c_str());

			std::getline(screenInfo, line);
			_info._screenTop = std::stol(line.c_str());
		}
		catch (const std::exception& e)
		{
			std::string what = e.what();
			_info._screenLeft = 100; _info._screenTop = 100;
		}

		if(_info._screenLeft <= 0)
		{
			_info._screenLeft= 100;
		}
		if (_info._screenTop <= 0)
		{
			_info._screenTop = 100;
		}

	}
	screenInfo.close();

	{
		std::ifstream screenInfo("screenResolution");

		if (screenInfo.is_open())
		{
			std::string line{};
			try {
				std::getline(screenInfo, line);
				_info._screenHeight = std::stol(line.c_str());

				std::getline(screenInfo, line);
				_info._screenWidth = std::stol(line.c_str());
			}
			catch (const std::exception& e)
			{
				std::string what =e.what();
				_info._screenWidth = 1920;
				_info._screenHeight = 1080;
			}

			if (_info._screenWidth <= 0.f)
			{
				_info._screenWidth = 1920;

			}
			if (_info._screenHeight <= 0.f)
			{
				_info._screenHeight = 1080;
			}
		}
		screenInfo.close();
	}
}
