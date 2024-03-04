#include "pch.h"
#include "UIObject.h"

#include "ImpDevice.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"
#include "ResourceManager.h"

ImpGraphics::UIObject::UIObject(std::shared_ptr<GeometryBuffer> vbib,
                                std::shared_ptr<Shader> shader,
                                std::shared_ptr<Texture> texture,
                                size_t layer)
    : _xRatio(1.0f)
    , _isNoise(false)
{
    _geometryBuffer = vbib;
    _shader         = shader;
    _texture        = texture;
    _layer          = layer;

    _noiseTexture = std::make_shared<Texture>(shader->GetResourceManager());

    _noiseTexture->Load(L"Resources/Texture/Test/noise.jpeg");
}

ImpGraphics::UIObject::~UIObject()
{
}

void ImpGraphics::UIObject::Update(float deltaTime)
{
    _noiseTime += deltaTime;
}

void ImpGraphics::UIObject::Render(ImpDevice* device,
                                   ImpRenderState* renderState,
                                   ImpRenderTarget* rendertarget)
{
    ID3D11DeviceContext* deviceContext = device->GetDeviceContext();

    // temp 샘플러 설정
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias     = 0.0f;
    samplerDesc.MaxAnisotropy  = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD         = -FLT_MAX;
    samplerDesc.MaxLOD         = FLT_MAX;

    ID3D11SamplerState* myLinearWrapSampler;
    HRESULT hr = device->GetDevice()->CreateSamplerState(&samplerDesc,
                                                         &myLinearWrapSampler);
    deviceContext->PSSetSamplers(0, 1, &myLinearWrapSampler);

    // blend 상태 설정
    D3D11_BLEND_DESC blendStateDesc{};
    blendStateDesc.AlphaToCoverageEnable          = FALSE;
    blendStateDesc.IndependentBlendEnable         = FALSE;
    blendStateDesc.RenderTarget[0].BlendEnable    = TRUE;
    blendStateDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
    blendStateDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState* blendState;
    device->GetDevice()->CreateBlendState(&blendStateDesc, &blendState);

    deviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);

    // draw
    deviceContext->IASetInputLayout(_shader->GetInputLayout());
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    deviceContext->RSSetState(renderState->GetCurrentRS());

    for (UINT i = 0; i < _geometryBuffer->GetSize(); i++)
    {
        UINT stride = _geometryBuffer->GetVertexSize();
        UINT offset = 0;
        deviceContext->IASetVertexBuffers(0, 1, _geometryBuffer->GetVB(i), &stride, &offset);
        deviceContext->IASetIndexBuffer(_geometryBuffer->GetIB(i),
                                        DXGI_FORMAT_R32_UINT,
                                        0);

        DirectX::XMMATRIX view          = DirectX::XMMatrixIdentity();
        DirectX::XMMATRIX proj          = DirectX::XMMatrixIdentity();
        DirectX::XMMATRIX world         = DirectX::XMMatrixIdentity();
        DirectX::XMMATRIX worldViewProj = world * view * proj;

        deviceContext->VSSetShader(_shader->GetVertexShader(), nullptr, 0);
        deviceContext->PSSetShader(_shader->GetPixelShader(), nullptr, 0);

        // 상수버퍼 setting
        // vs
        Shader::UIConstantBuffer test{};

        test.x = _x;
        test.y = _y;
        test.ratio = _ratio;

        _shader->SetConstantBuffer(test);

        // ps
        Shader::UIAlphaConstantBuffer psCons{};

        psCons.alpha = _alpha;
        psCons.fStregth = 0.05f;

        static int count   = 0;
        const float noiseOffset = 3;
        if (count % 8 == 0)
        {
            psCons.uvDir = DirectX::XMFLOAT2{ -noiseOffset, 0.f };
            psCons.isPlus = true;
        }
        else if (count % 8 == 1)
        {
            psCons.uvDir = DirectX::XMFLOAT2{ noiseOffset * 2, 0.f };
            psCons.isPlus = false;
        }
        else if (count % 8 == 2)
        {
            psCons.uvDir = DirectX::XMFLOAT2{ -noiseOffset * 2, 0.f };
            psCons.isPlus = true;
        }
        else if (count % 8 == 3)
        {
            psCons.uvDir = DirectX::XMFLOAT2{ noiseOffset, 0.f };
            psCons.isPlus = false;
        }

        if (_noiseTime > 0.3f)
        {
            count++;
            if (count == 4)
            {
                count = 0;
            }
            _noiseTime = 0;
        }

        psCons.isNoise = _isNoise;
        psCons.xRatio  = _xRatio;

        _shader->SetConstantBuffer(psCons);

        // 텍스처 바인딩
        deviceContext->PSSetShaderResources(0, 1, &_texture->GetTextureSRV());
        deviceContext->PSSetShaderResources(1, 1, &_noiseTexture->GetTextureSRV());

        deviceContext->DrawIndexed(_geometryBuffer->GetIndexSize(i), 0, 0);

        // 텍스처 바인딩 해제
        ID3D11ShaderResourceView* pSRV = NULL;
        deviceContext->PSSetShaderResources(0, 1, &pSRV);
        pSRV = NULL;
        deviceContext->PSSetShaderResources(1, 1, &pSRV);
    }

    myLinearWrapSampler->Release();

    deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    blendState->Release();
}

void ImpGraphics::UIObject::SetXY(float x, float y, float ratio)
{
    _x = x;
    _y = y;
    _ratio = ratio;
}

void ImpGraphics::UIObject::SetGeometryBuffer(std::shared_ptr<GeometryBuffer> vbib)
{
    _geometryBuffer = vbib;
}

void ImpGraphics::UIObject::SetLayer(size_t layer)
{
    _layer = layer;
}

void ImpGraphics::UIObject::SetTexture(std::shared_ptr<Texture> texture)
{
    _texture = texture;
}

void ImpGraphics::UIObject::SetAlpha(float alpha)
{
    _alpha = alpha;
}

void ImpGraphics::UIObject::SetOffset(float x, float y)
{
    _offsetX = x;
    _offsetY = y;
}

void ImpGraphics::UIObject::GetOffset(float& x, float& y)
{
    x = _offsetX;
    y = _offsetY;
}

void ImpGraphics::UIObject::SetWidth(float width)
{
    _width = width;
}

void ImpGraphics::UIObject::SetHeight(float height)
{
    _height = height;
}

void ImpGraphics::UIObject::SetScreenWidth(float width)
{
    _screenWidth = width;
}

void ImpGraphics::UIObject::SetScreenHeight(float height)
{
    _screenHeight = height;
}

void ImpGraphics::UIObject::SetXSize(float ratio)
{
    Vector4 uiPos             = Vector4{ 0, 0, _width * ratio, _height };
    DirectX::XMFLOAT2 screenSize = { _screenWidth, _screenHeight };
    uiPos.x                   = 0;
    uiPos.y                   = 0;
    std::wstring lengthString = std::to_wstring(screenSize.x) +
                                std::to_wstring(screenSize.y) +
                                std::to_wstring(uiPos.x) + std::to_wstring(uiPos.y) +
                                std::to_wstring(uiPos.z) + std::to_wstring(uiPos.w);

    _geometryBuffer = _shader->GetResourceManager()->GetResource<GeometryBuffer>(
            lengthString + L".quad", uiPos, screenSize);

    SetOffset((uiPos.z / _screenWidth), (uiPos.w / _screenHeight));

    _xRatio = ratio;
}

void ImpGraphics::UIObject::SetNoise(bool isOn)
{
    _isNoise = isOn;
}
