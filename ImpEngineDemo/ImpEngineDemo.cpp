// ImpEngineDemo.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "resource.h"
#include "Application.h"

#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Application* app = new Application;

    app->Initialize(hInstance);

    app->Process();

    app->Finalize();

    delete app;

    return 0;
}

