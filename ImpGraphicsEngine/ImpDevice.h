#pragma once

// Feature 레벨, 드라이버 타입, 멀티 샘플링은 아직 잘 모른다. 특히 feature 레벨 
// 나중에 알게 되어서 조절해야 한다면 추가
// 지금은 보류

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

namespace ImpGraphics
{
	class ImpDevice
	{
	public:
		ImpDevice();
		~ImpDevice();

		bool Initialize(int hWnd, int screenWidth, int screenHeight);

		ID3D11Device* GetDevice() const { return _device; }
		ID3D11DeviceContext* GetDeviceContext() const { return _deviceContext; }
		IDXGISwapChain* GetSwapChain() const { return _swapChain; }

	private:
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;
		IDXGISwapChain* _swapChain;
	};
}