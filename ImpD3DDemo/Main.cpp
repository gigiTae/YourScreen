#include "pch.h"
#include "Process.h"

// 버전 바꿀 때 신경써서 확인할 것
#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/ImpStructure.lib")
#pragma comment(lib, "../x64/Debug/ImpGraphicsEngine.lib")
#else
#pragma comment(lib, "../x64/Release/ImpStructure.lib")
#pragma comment(lib, "../x64/Release/ImpGraphicsEngine.lib")
#endif // DEBUG

//#pragma comment(lib, "../ImpLib/ImpGraphicsEngine.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	//_crtBreakAlloc = 898280; //_CrtSetBreakAlloc(18); 
	//_crtBreakAlloc = 13332; //_CrtSetBreakAlloc(18); 

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::unique_ptr<Process> mainWindow = std::make_unique<Process>();

	mainWindow->Init(hInstance);
	mainWindow->Loop();
	mainWindow->Finalize();

	return 0;
}