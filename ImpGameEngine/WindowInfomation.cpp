#include "ImpGameEnginePCH.h"
#include "WindowInfomation.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LONG ImpGameEngine::WindowInfomation::resizeWidth = 0;
LONG ImpGameEngine::WindowInfomation::resizeHegiht = 0;

LRESULT CALLBACK ImpGameEngine::WindowInfomation::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		{
			resizeWidth = LOWORD(lParam);
			resizeHegiht = HIWORD(lParam);
		}
		break;
	default:
        return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ImpGameEngine::WindowInfomation::ToolWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOVE:
	{
		int left = (int)(short)LOWORD(lParam);
		int top = (int)(short)HIWORD(lParam);

		// 위치를 저장한다.
		std::ofstream os("screenPosition");

		if (os.is_open())
		{
			os << left << std::endl;
			os << top;
		}
		os.close();
	}
	break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		{
			resizeWidth = LOWORD(lParam);
			resizeHegiht = HIWORD(lParam);

			std::ofstream os("screenResolution");

			if (os.is_open())
			{
				os << resizeHegiht << std::endl;
				os << resizeWidth << std::endl;
			}
			os.close();
		}
		break;

	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}
