#pragma once
#define WIN32_LEAN_AND_MEAN 

struct ID3D11Device;
struct ID3D11DeviceContext;

//#pragma comment(lib, "../x64/Debug/ImpGraphicsEngine.lib")
#pragma comment(lib, "../ImpLib/ImpGraphicsEngine.lib")

namespace ImpGraphics
{
	class IImpGraphicsTool
	{
		virtual void Init(size_t hWnd, ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext) abstract;
		virtual void Update() abstract;
		virtual void Render() abstract;
		virtual void Finalize() abstract;
	};

	class ToolExporter
	{
	public:
		static IImpGraphicsTool* GetEngine();
		static void DeleteEngine();

	private:
		static IImpGraphicsTool* _tool;
	};
}