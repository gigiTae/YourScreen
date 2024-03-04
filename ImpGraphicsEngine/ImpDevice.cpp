#include "pch.h"
#include "ImpDevice.h"

#include "Define.h"

using namespace ImpGraphics;

ImpDevice::ImpDevice()
	:_device(nullptr),
	_deviceContext(nullptr),
	_swapChain(nullptr)
{

}

ImpDevice::~ImpDevice()
{
	// 삭제는 생성의 역순
	ReleaseCOM(_swapChain);

	// Restore all default settings.
	if (_deviceContext)
	{
		_deviceContext->ClearState();
	}

	ReleaseCOM(_deviceContext);
	ReleaseCOM(_device);
}

bool ImpDevice::Initialize(int hWnd, int screenWidth, int screenHeight)
{
	// 디바이스 생성 모드. 디버그 빌드인 경우 디버그 모드로
	UINT _createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 드라이버 타입
	D3D_DRIVER_TYPE driveType = D3D_DRIVER_TYPE_HARDWARE;

	// 스왑체인 정보
	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = (HWND)hWnd;		// DXGI-Windef
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	sd.SampleDesc.Count = 1;			// No MSAA
	sd.SampleDesc.Quality = 0;

	D3D_FEATURE_LEVEL featureLevel;

	/// 1. 디바이스와 디바이스컨텍스트와 스왑체인을 한 방에 생성한다.
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		0, driveType, 0, _createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &sd,
		&_swapChain, &_device, &featureLevel, &_deviceContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	return true;
}