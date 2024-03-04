#pragma once

#include <windows.h>

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_dx11.h"

namespace ImpGraphics
{
	class IImpGraphicsEngine;

	class GUIManager 
	{
	public:
		GUIManager();
		~GUIManager();

	public:
		void Init(size_t hWnd, void* d3dDevice, void* d3dDeviceContext);
		void Update(IImpGraphicsEngine* graphicsEngine);
		void Render();
		void Finalize();
	};
}