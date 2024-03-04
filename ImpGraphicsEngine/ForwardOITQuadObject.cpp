#include "pch.h"
#include "ForwardOITQuadObject.h"

#include "ImpDevice.h"
#include "ImpCamera.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"

ImpGraphics::ForwardOITQuadObject::ForwardOITQuadObject(
    std::shared_ptr<GeometryBuffer> vbib,
    std::shared_ptr<Shader> shader)
{
    _geometryBuffer = vbib;
    _shader         = shader;
}

ImpGraphics::ForwardOITQuadObject::~ForwardOITQuadObject()
{
}

void ImpGraphics::ForwardOITQuadObject::Update(ImpCamera* camera)
{
    _viewTM = camera->GetViewTM();
    _projTM = camera->GetProjTM();
}

void ImpGraphics::ForwardOITQuadObject::Render(ImpDevice* device,
    ImpRenderState* renderState,
    ImpRenderTarget* rendertarget)
{
    ID3D11DeviceContext* deviceContext = device->GetDeviceContext();

    // 샘플러를 어디서 만들어서 넣어야 할까! 여긴 아닌게 확실하다!
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
    HRESULT hr = device->GetDevice()->CreateSamplerState(
        &samplerDesc, &myLinearWrapSampler);
    deviceContext->PSSetSamplers(0, 1, &myLinearWrapSampler);

    // blend 상태 설정
    D3D11_BLEND_DESC blendStateDesc{};
    blendStateDesc.AlphaToCoverageEnable          = FALSE;
    blendStateDesc.IndependentBlendEnable         = FALSE;
    blendStateDesc.RenderTarget[0].BlendEnable    = TRUE;
    blendStateDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    
    blendStateDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState* blendState;
    device->GetDevice()->CreateBlendState(&blendStateDesc, &blendState);

    deviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);

    //
    deviceContext->IASetInputLayout(_shader->GetInputLayout());
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->RSSetState(renderState->GetSolidRS());

    UINT stride = _geometryBuffer->GetVertexSize();
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(
        0, 1, _geometryBuffer->GetVB(), &stride, &offset);
    deviceContext->IASetIndexBuffer(
        _geometryBuffer->GetIB(), DXGI_FORMAT_R32_UINT, 0);

    deviceContext->VSSetShader(_shader->GetVertexShader(), nullptr, 0);
    deviceContext->PSSetShader(_shader->GetPixelShader(), nullptr, 0);

    std::vector<ID3D11ShaderResourceView*>
        temp = rendertarget->GetDeferredShaderResourceViews();
    deviceContext->PSSetShaderResources(0, 1, &temp[0]);
    deviceContext->PSSetShaderResources(1, 1, &temp[1]);

    //_shader->SetConstantBuffer2(test);

    deviceContext->DrawIndexed(6, 0, 0);

    ID3D11ShaderResourceView* pSRV = NULL;
    deviceContext->PSSetShaderResources(0, 1, &pSRV);
    pSRV = NULL;
    deviceContext->PSSetShaderResources(1, 1, &pSRV);
    pSRV = NULL;
    deviceContext->PSSetShaderResources(2, 1, &pSRV);

    myLinearWrapSampler->Release();
    rendertarget->FirstPassUnbind();

    deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    blendState->Release();
}

void ImpGraphics::ForwardOITQuadObject::SetTransformMatrix(
    DirectX::XMMATRIX transformMatrix)
{
}

std::shared_ptr<ImpGraphics::GeometryBuffer> ImpGraphics::ForwardOITQuadObject::GetGeometryBuffer()
{
    return _geometryBuffer;
}