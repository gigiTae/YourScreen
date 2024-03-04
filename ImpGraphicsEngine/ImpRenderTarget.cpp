#include "pch.h"
#include "ImpRenderTarget.h"
#include "ImpDevice.h"

#include "Define.h"

using namespace ImpGraphics;

ImpRenderTarget::ImpRenderTarget(ImpDevice* device)
	:_device(device),
	_renderTargetView(nullptr),
	_depthStencilView(nullptr), _deferredRenderTargetView()
    , _deferredViewSize(5)
    , _bloomViewSize(2), _bloomRatio(2)
{
	
}

ImpRenderTarget::~ImpRenderTarget()
{
	ReleaseCOM(_depthStencilView);
	ReleaseCOM(_renderTargetView);

	for (size_t i = 0; i < _deferredRenderTargetView.size(); i++)
	{
		ReleaseCOM(_deferredRenderTargetView[i]);
	}

	for (size_t i = 0; i < _deferredShaderResourceView.size(); i++)
	{
		ReleaseCOM(_deferredShaderResourceView[i]);
	}

	for (size_t i = 0; i < _bloomRenderTargetView.size(); i++)
    {
        ReleaseCOM(_bloomRenderTargetView[i]);
    }

	for (size_t i = 0; i < _bloomShaderResourceView.size(); i++)
    {
        ReleaseCOM(_bloomShaderResourceView[i]);
    }

    for (size_t i = 0; i < _depthStencilViews.size(); i++)
    {
        ReleaseCOM(_depthStencilViews[i]);
    }
}

void ImpRenderTarget::Initialize(int width, int height)
{
	ID3D11Device* device = _device->GetDevice();
	ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();
	IDXGISwapChain* swapChain = _device->GetSwapChain();

	assert(deviceContext);
	assert(device);
	assert(swapChain);

	ID3D11Texture2D* backBuffer = nullptr;
	HR(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	if (backBuffer == nullptr)
	{
		MessageBox(NULL, L"backBuffer가 nullptr입니다.", L"에러", MB_ICONERROR);
	}
	else
	{
		HR(device->CreateRenderTargetView(backBuffer, 0, &_renderTargetView));
	}
	ReleaseCOM(backBuffer);

	/// Deferred Rendering을 위한 준비
	{
        D3D11_TEXTURE2D_DESC textureDesc{};
        textureDesc.Width            = width;
        textureDesc.Height           = height;
        textureDesc.MipLevels        = 1;
        textureDesc.ArraySize        = 1;
        textureDesc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage            = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        ID3D11Texture2D* temp;
        std::vector<ID3D11Texture2D*> textures;
        for (size_t i = 0; i < _deferredViewSize; i++)
        {
            HR(device->CreateTexture2D(&textureDesc, 0, &temp));
            textures.push_back(temp);
        }

        ID3D11RenderTargetView* tempRenderTarget;
        D3D11_RENDER_TARGET_VIEW_DESC rendertargetViewDesc{};

        rendertargetViewDesc.Format        = textureDesc.Format;
        rendertargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        for (size_t i = 0; i < _deferredViewSize; i++)
        {
            HR(device->CreateRenderTargetView(textures[i],
                                              &rendertargetViewDesc,
                                              &tempRenderTarget));
            _deferredRenderTargetView.push_back(tempRenderTarget);
        }

        ID3D11ShaderResourceView* tempShaderResourceView;
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};

        shaderResourceViewDesc.Format        = textureDesc.Format;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;

        for (size_t i = 0; i < _deferredViewSize; i++)
        {
            HR(device->CreateShaderResourceView(textures[i],
                                                &shaderResourceViewDesc,
                                                &tempShaderResourceView));
            _deferredShaderResourceView.push_back(tempShaderResourceView);
        }

        for (size_t i = 0; i < textures.size(); i++)
        {
            ReleaseCOM(textures[i]);
        }

		D3D11_TEXTURE2D_DESC depthStencilDesc{};

        depthStencilDesc.Width     = width;
        depthStencilDesc.Height    = height;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

        depthStencilDesc.SampleDesc.Count   = 1;
        depthStencilDesc.SampleDesc.Quality = 0;

        depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.CPUAccessFlags = 0;
        depthStencilDesc.MiscFlags      = 0;

        ID3D11Texture2D* depthStencilBuffer;
        HR(device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer));
        if (depthStencilBuffer == nullptr)
        {
            MessageBox(NULL, L"depthStencilBuffer가 nullptr입니다.", L"에러", MB_ICONERROR);
        }
        else
        {
            HR(device->CreateDepthStencilView(depthStencilBuffer, 0, &_depthStencilView));
        }
        ReleaseCOM(depthStencilBuffer);
	}

    /// Bloom을 위한 준비
    {
        D3D11_TEXTURE2D_DESC textureDesc{};
        textureDesc.Width            = width;
        textureDesc.Height           = height;
        textureDesc.MipLevels        = 1;
        textureDesc.ArraySize        = 1;
        textureDesc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage            = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags        = D3D11_BIND_RENDER_TARGET |
                                D3D11_BIND_SHADER_RESOURCE;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

        ID3D11Texture2D* temp;
        std::vector<ID3D11Texture2D*> textures;
        for (size_t i = 0; i < _bloomViewSize; i++)
        {
            HR(device->CreateTexture2D(&textureDesc, 0, &temp));
            textures.push_back(temp);
        }

        ID3D11RenderTargetView* tempRenderTarget;
        D3D11_RENDER_TARGET_VIEW_DESC rendertargetViewDesc{};

        rendertargetViewDesc.Format        = textureDesc.Format;
        rendertargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        for (size_t i = 0; i < _bloomViewSize; i++)
        {
            HR(device->CreateRenderTargetView(textures[i],
                                                &rendertargetViewDesc,
                                                &tempRenderTarget));
            _bloomRenderTargetView.push_back(tempRenderTarget);
        }

        ID3D11ShaderResourceView* tempShaderResourceView;
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};

        shaderResourceViewDesc.Format = textureDesc.Format;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;

        for (size_t i = 0; i < _bloomViewSize; i++)
        {
            HR(device->CreateShaderResourceView(textures[i],
                                                &shaderResourceViewDesc,
                                                &tempShaderResourceView));
            _bloomShaderResourceView.push_back(tempShaderResourceView);
        }

        for (size_t i = 0; i < textures.size(); i++)
        {
            ReleaseCOM(textures[i]);
        }

        for (int blur = 0; blur < 2; blur++)
        {
            for (int dView = 0; dView < 3; dView++)
            {
                D3D11_TEXTURE2D_DESC textureDesc{};
                textureDesc.Width = width / std::pow(_bloomRatio, dView + 1);
                textureDesc.Height = height / std::pow(_bloomRatio, dView + 1);
                textureDesc.MipLevels = 1;
                textureDesc.ArraySize = 1;
                textureDesc.Format    = DXGI_FORMAT_R32G32B32A32_FLOAT;
                textureDesc.SampleDesc.Count = 1;
                textureDesc.Usage            = D3D11_USAGE_DEFAULT;
                textureDesc.BindFlags        = D3D11_BIND_RENDER_TARGET |
                                        D3D11_BIND_SHADER_RESOURCE;
                textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

                ID3D11Texture2D* temp;
                std::vector<ID3D11Texture2D*> textures;
                // for (size_t i = 0; i < _bloomViewSize; i++)
                {
                    HR(device->CreateTexture2D(&textureDesc, 0, &temp));
                    textures.push_back(temp);
                }

                ID3D11RenderTargetView* tempRenderTarget;
                D3D11_RENDER_TARGET_VIEW_DESC rendertargetViewDesc{};

                rendertargetViewDesc.Format = textureDesc.Format;
                rendertargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

                // for (size_t i = 0; i < _bloomViewSize; i++)
                {
                    HR(device->CreateRenderTargetView(textures[0],
                                                        &rendertargetViewDesc,
                                                        &tempRenderTarget));
                    _bloomRenderTargetView.push_back(tempRenderTarget);
                }

                ID3D11ShaderResourceView* tempShaderResourceView;
                D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};

                shaderResourceViewDesc.Format = textureDesc.Format;
                shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                shaderResourceViewDesc.Texture2D.MipLevels = 1;

                // for (size_t i = 0; i < _bloomViewSize; i++)
                {
                    HR(device->CreateShaderResourceView(textures[0],
                                                        &shaderResourceViewDesc,
                                                        &tempShaderResourceView));
                    _bloomShaderResourceView.push_back(tempShaderResourceView);
                }

                // for (size_t i = 0; i < textures.size(); i++)
                {
                    ReleaseCOM(textures[0]);
                }

                D3D11_TEXTURE2D_DESC depthStencilDesc{};

                depthStencilDesc.Width = width /
                                            std::pow(_bloomRatio, dView + 1);
                depthStencilDesc.Height = height /
                                            std::pow(_bloomRatio, dView + 1);
                depthStencilDesc.MipLevels = 1;
                depthStencilDesc.ArraySize = 1;
                depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

                depthStencilDesc.SampleDesc.Count   = 1;
                depthStencilDesc.SampleDesc.Quality = 0;

                depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
                depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
                depthStencilDesc.CPUAccessFlags = 0;
                depthStencilDesc.MiscFlags      = 0;

                ID3D11Texture2D* depthStencilBuffer;
                ID3D11DepthStencilView* depthStencilView;
                HR(device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer));
                if (depthStencilBuffer == nullptr)
                {
                    MessageBox(NULL,
                                L"depthStencilBuffer가 nullptr입니다.",
                                L"에러",
                                MB_ICONERROR);
                }
                else
                {
                    HR(device->CreateDepthStencilView(depthStencilBuffer,
                                                        0,
                                                        &depthStencilView));
                    _depthStencilViews.push_back(depthStencilView);
                }
                ReleaseCOM(depthStencilBuffer);
            }
        }
    }

	D3D11_VIEWPORT screenViewport{};

	/// 뷰포트 변환을 셋팅한다.
	screenViewport.TopLeftX = 0;
	screenViewport.TopLeftY = 0;
	screenViewport.Width = static_cast<float>(width);
	screenViewport.Height = static_cast<float>(height);
	screenViewport.MinDepth = 0.0f;
	screenViewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &screenViewport);
}

void ImpRenderTarget::OnResize(int width, int height)
{
	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	ReleaseCOM(_depthStencilView);
	ReleaseCOM(_renderTargetView);
	for (size_t i = 0; i < _deferredRenderTargetView.size(); i++)
	{
		ReleaseCOM(_deferredRenderTargetView[i]);
		ReleaseCOM(_deferredShaderResourceView[i]);
	}
	_deferredRenderTargetView.clear();
	_deferredShaderResourceView.clear();

    for (size_t i = 0; i < _bloomRenderTargetView.size(); i++)
    {
        ReleaseCOM(_bloomRenderTargetView[i]);
        ReleaseCOM(_bloomShaderResourceView[i]);
    }
    _bloomRenderTargetView.clear();
    _bloomShaderResourceView.clear();

    for (size_t i = 0; i < _depthStencilViews.size(); i++)
    {
        ReleaseCOM(_depthStencilViews[i]);
    }
    _depthStencilViews.clear();

	// Resize the swap chain and recreate the render target view.
	IDXGISwapChain* swapChain = _device->GetSwapChain();
	HR(swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	Initialize(width, height);
}

void ImpRenderTarget::SecondPassClear()
{
	ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();
	assert(deviceContext);

	deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	const float blackBackgroundColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (size_t i = 0; i < 1; i++)
	{
		deviceContext->ClearRenderTargetView(_bloomRenderTargetView[0], blackBackgroundColor);
		deviceContext->ClearRenderTargetView(_bloomRenderTargetView[1], blackBackgroundColor);
	}
}

void ImpRenderTarget::SecondPassBind()
{
	_device->GetDeviceContext()->OMSetRenderTargets(2, &_bloomRenderTargetView[0], _depthStencilView);
}

void ImpRenderTarget::SecondPassUnbind()
{
    ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();

    ID3D11ShaderResourceView* pSRV = NULL;
    deviceContext->PSSetShaderResources(0, 1, &pSRV);

    pSRV = NULL;
    deviceContext->PSSetShaderResources(1, 1, &pSRV);
}

void ImpRenderTarget::FirstPassBind()
{
	_device->GetDeviceContext()->OMSetRenderTargets(5, &_deferredRenderTargetView[0], _depthStencilView);
}

void ImpRenderTarget::FirstPassClear()
{
	ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();
	assert(deviceContext);

	deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	const float blackBackgroundColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const float blackBackgroundColor2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (size_t i = 0; i < 1; i++)
	{
		deviceContext->ClearRenderTargetView(_deferredRenderTargetView[0], blackBackgroundColor);
		deviceContext->ClearRenderTargetView(_deferredRenderTargetView[1], blackBackgroundColor2);
		deviceContext->ClearRenderTargetView(_deferredRenderTargetView[2], blackBackgroundColor2);
		deviceContext->ClearRenderTargetView(_deferredRenderTargetView[3], blackBackgroundColor2);
		deviceContext->ClearRenderTargetView(_deferredRenderTargetView[4], blackBackgroundColor2);
	}
}

void ImpRenderTarget::FirstPassUnbind()
{
	ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();

	ID3D11ShaderResourceView* pSRV = NULL;
	deviceContext->PSSetShaderResources(0, 1, &pSRV);

	pSRV = NULL;
	deviceContext->PSSetShaderResources(1, 1, &pSRV);

	pSRV = NULL;
	deviceContext->PSSetShaderResources(2, 1, &pSRV);

	pSRV = NULL;
	deviceContext->PSSetShaderResources(3, 1, &pSRV);

	pSRV = NULL;
    deviceContext->PSSetShaderResources(4, 1, &pSRV);
}

void ImpRenderTarget::ThreePassClear()
{
	ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();
	assert(deviceContext);

	// 랜더타겟 뷰를 클리어한다.
	const float backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->ClearRenderTargetView(_renderTargetView, backgroundColor);

	// 뎁스스탠실 뷰를 클리어한다.
	deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ImpRenderTarget::ThreePassBind()
{
	// 이전까지는 매 프레임 안 해도 됐지만
	// 추측으로는 IMGUI 에서 렌더 타겟이 여러 개가 되어서 바인딩을 해줘야 하는걸로 보인다.
	/// 렌더타겟뷰, 뎁스/스탠실뷰를 파이프라인에 바인딩한다.
	_device->GetDeviceContext()->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
}

void ImpRenderTarget::ThreePassUnbind()
{
    ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();

    ID3D11ShaderResourceView* pSRV = NULL;
    deviceContext->PSSetShaderResources(0, 1, &pSRV);
}

void ImpRenderTarget::DownSamplingPassClear(size_t index)
{
    ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();
	assert(deviceContext);

	// 랜더타겟 뷰를 클리어한다.
	const float backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    deviceContext->ClearRenderTargetView(_bloomRenderTargetView[index + 2], backgroundColor);

	// 뎁스스탠실 뷰를 클리어한다.
	deviceContext->ClearDepthStencilView(_depthStencilViews[0], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ImpRenderTarget::DownSamplingPassBind(size_t index)
{
    _device->GetDeviceContext()->OMSetRenderTargets(1, &_bloomRenderTargetView[index + 2], _depthStencilViews[index]);
}

void ImpRenderTarget::DownSamplingPassUnbind(size_t index)
{
    ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();

	ID3D11ShaderResourceView* pSRV = NULL;
	deviceContext->PSSetShaderResources(0, 1, &pSRV);
}

void ImpRenderTarget::ForwardPassClear()
{
    ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();
	assert(deviceContext);

	// 랜더타겟 뷰를 클리어한다.
    // accum
	const float backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    deviceContext->ClearRenderTargetView(_deferredRenderTargetView[0], backgroundColor);
    //reveal
    const float backgroundColor2[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    deviceContext->ClearRenderTargetView(_deferredRenderTargetView[1], backgroundColor2);

	// 뎁스스탠실 뷰를 클리어한다.
	deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ImpRenderTarget::ForwardPassBind()
{
    // 디퍼드 뎁스 가져오기 mr x
    // 디퍼드 오리지날 바인딩
    _device->GetDeviceContext()->OMSetRenderTargets(2, &_deferredRenderTargetView[0], _depthStencilView);
}

void ImpRenderTarget::ForwardPassUnbind()
{
    ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();

    ID3D11ShaderResourceView* pSRV = NULL;
    deviceContext->PSSetShaderResources(0, 1, &pSRV);
    pSRV = NULL;
    deviceContext->PSSetShaderResources(0, 1, &pSRV);
}

void ImpRenderTarget::ForwardOITPassClear()
{
    ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();
    assert(deviceContext);

    //const float backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    //deviceContext->ClearRenderTargetView(_deferredRenderTargetView[5], backgroundColor);

    // 뎁스스탠실 뷰를 클리어한다.
	deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ImpRenderTarget::ForwardOITPassBind()
{
    _device->GetDeviceContext()->OMSetRenderTargets(1, &_bloomRenderTargetView[0], _depthStencilView);
}

void ImpRenderTarget::ForwardOITPassUnbind()
{
    ID3D11DeviceContext* deviceContext = _device->GetDeviceContext();

    ID3D11ShaderResourceView* pSRV = NULL;
    deviceContext->PSSetShaderResources(0, 1, &pSRV);
}
