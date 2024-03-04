#include "pch.h"
#include "ImpRenderState.h"

#include "define.h"

ImpGraphics::ImpRenderState::ImpRenderState()
	:_wireframeRS(nullptr),
	_solidRS(nullptr),
	_normalDSS(nullptr),
	_cubeMapDSS(nullptr),
	_isWireFrame(false)
{
	
}

ImpGraphics::ImpRenderState::~ImpRenderState()
{
	ReleaseCOM(_wireframeRS);
	ReleaseCOM(_solidRS);
	ReleaseCOM(_normalDSS);
	ReleaseCOM(_cubeMapDSS);
	ReleaseCOM(_noDepthTestDSS);
}

void ImpGraphics::ImpRenderState::Initialize(ID3D11Device* device)
{
	/// Render State

	/// Rasterizer State
	// 솔리드
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_NONE;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&solidDesc, &_solidRS));

	D3D11_RASTERIZER_DESC solidDescCullBack;
    ZeroMemory(&solidDescCullBack, sizeof(D3D11_RASTERIZER_DESC));
    solidDescCullBack.FillMode              = D3D11_FILL_SOLID;
    solidDescCullBack.CullMode              = D3D11_CULL_BACK;
    solidDescCullBack.FrontCounterClockwise = false;
    solidDescCullBack.DepthClipEnable       = true;
    HR(device->CreateRasterizerState(&solidDescCullBack, &_solidCullBackRS));

	// 와이어
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&wireframeDesc, &_wireframeRS));


	/// Depth-Stencil State
	// 폰트용 DSS
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰기는 한다
    equalsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HR(device->CreateDepthStencilState(&equalsDesc, &_normalDSS));

	// Cube Map 용 LESS_EQUAL 
	D3D11_DEPTH_STENCIL_DESC cubeMapDesc;
	ZeroMemory(&cubeMapDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	cubeMapDesc.DepthEnable = true;
	cubeMapDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	cubeMapDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	cubeMapDesc.StencilEnable = false;
	cubeMapDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	cubeMapDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	cubeMapDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	cubeMapDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	cubeMapDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	cubeMapDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	HR(device->CreateDepthStencilState(&cubeMapDesc, &_cubeMapDSS));

	// depth Test 안하는 버전
	D3D11_DEPTH_STENCIL_DESC noDepthTestDesc;
    ZeroMemory(&cubeMapDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    noDepthTestDesc.DepthEnable                  = false;
    noDepthTestDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
    noDepthTestDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
    noDepthTestDesc.StencilEnable                = false;
    noDepthTestDesc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
    noDepthTestDesc.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    noDepthTestDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    noDepthTestDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    noDepthTestDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    noDepthTestDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;

    HR(device->CreateDepthStencilState(&noDepthTestDesc, &_noDepthTestDSS));
}