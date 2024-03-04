#pragma once

// Feature ����, ����̹� Ÿ��, ��Ƽ ���ø��� ���� �� �𸥴�. Ư�� feature ���� 
// ���߿� �˰� �Ǿ �����ؾ� �Ѵٸ� �߰�
// ������ ����

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