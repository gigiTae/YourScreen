#pragma once

#include <windows.h>

#include "IImpGraphicsTool.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_dx11.h"

namespace ImpGraphics
{
	class GUIManager : public IImpGraphicsTool
	{
	public:
		GUIManager();
		~GUIManager();

	public:
		virtual void Init(size_t hWnd, ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext) override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void Finalize() override;
	};
}