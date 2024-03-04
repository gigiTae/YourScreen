#include "pch.h"
#include "ForwardObject.h"

#include "ImpDevice.h"
#include "ImpCamera.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"

#include "Material.h"

// temp
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

ImpGraphics::ForwardObject::ForwardObject(std::shared_ptr<GeometryBuffer> vbib,
    std::shared_ptr<Shader> shader,
    Material* material)
    : _emissionCoefficient(1)
{
    _geometryBuffer = vbib;
    _shader         = shader;
    _material       = material;

    _alpha = 1.0f;
}

ImpGraphics::ForwardObject::~ForwardObject()
{
    delete _material;
}

void ImpGraphics::ForwardObject::Update(ImpCamera* camera)
{
    _viewTM = camera->GetViewTM();
    _projTM = camera->GetProjTM();
}

void ImpGraphics::ForwardObject::Render(ImpDevice* device,
    ImpRenderState* renderState,
    ImpRenderTarget* rendertarget)
{
    // 1st Pass
    ID3D11DeviceContext* deviceContext = device->GetDeviceContext();

    // temp 샘플러 설정
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
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
    blendStateDesc.IndependentBlendEnable         = TRUE;
    blendStateDesc.RenderTarget[0].BlendEnable    = TRUE;
    blendStateDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    blendStateDesc.RenderTarget[1].BlendEnable    = TRUE;
    blendStateDesc.RenderTarget[1].SrcBlend       = D3D11_BLEND_ZERO;
    blendStateDesc.RenderTarget[1].DestBlend      = D3D11_BLEND_INV_SRC_COLOR;
    blendStateDesc.RenderTarget[1].BlendOp        = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[1].SrcBlendAlpha  = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDesc.RenderTarget[1].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState* blendState;
    device->GetDevice()->CreateBlendState(&blendStateDesc, &blendState);

    deviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);

    // no Depth Test
    deviceContext->OMSetDepthStencilState(renderState->_noDepthTestDSS, 0);

    //
    deviceContext->IASetInputLayout(_shader->GetInputLayout());
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    deviceContext->RSSetState(renderState->GetCurrentRS());

    for (UINT i = 0; i < _geometryBuffer->GetSize(); i++)
    {
        UINT stride = _geometryBuffer->GetVertexSize();
        UINT offset = 0;
        deviceContext->IASetVertexBuffers(
            0, 1, _geometryBuffer->GetVB(i), &stride, &offset);
        deviceContext->IASetIndexBuffer(
            _geometryBuffer->GetIB(i), DXGI_FORMAT_R32_UINT, 0);

        DirectX::XMMATRIX view          = XMLoadFloat4x4(&_viewTM);
        DirectX::XMMATRIX proj          = XMLoadFloat4x4(&_projTM);
        DirectX::XMMATRIX world         = XMLoadFloat4x4(&_worldTM);
        DirectX::XMMATRIX worldViewProj = world * view * proj;

        deviceContext->VSSetShader(_shader->GetVertexShader(), nullptr, 0);
        deviceContext->PSSetShader(_shader->GetPixelShader(), nullptr, 0);

        Shader::MeshConstantBuffer test{};

        XMStoreFloat4x4(&test.world, world);
        XMStoreFloat4x4(&test.transform, worldViewProj);
        XMStoreFloat4x4(&test.worldInvTranspose, XMMatrixInverse(nullptr, world));

        _shader->SetConstantBuffer(test);

        if (_material->GetBaseColor())
        {
            deviceContext->PSSetShaderResources(
                0, 1, &_material->GetBaseColor()->GetTextureSRV());
        }
        if (_material->GetMetallicRoughness())
        {
            deviceContext->PSSetShaderResources(
                1, 1, &_material->GetMetallicRoughness()->GetTextureSRV());
        }
        if (_material->GetEmission())
        {
            deviceContext->PSSetShaderResources(
                2, 1, &_material->GetEmission()->GetTextureSRV());
        }
        if (_material->GetNormalMap())
        {
            deviceContext->PSSetShaderResources(
                3, 1, &_material->GetNormalMap()->GetTextureSRV());
        }

        // 디퍼드 그린 뎁스 버퍼 바인딩
        ID3D11ShaderResourceView* deferredDepth = rendertarget->GetDeferredDepthShaderResourceView();
        deviceContext->PSSetShaderResources(4, 1, &deferredDepth);

        Shader::AlphaConstantBuffer test2{};

        test2.alpha = _alpha;

        _shader->SetConstantBuffer(test2);

        // ???
        Shader::EmissionCoefficientConstantBuffer test3{};

        test3.objectEmissionCoefficient = _emissionCoefficient;

        _shader->SetConstantBuffer(test3);

        deviceContext->DrawIndexed(_geometryBuffer->GetIndexSize(i), 0, 0);

        // if (_texture)
        {
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
    }

    myLinearWrapSampler->Release();

    deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    blendState->Release();
}

void ImpGraphics::ForwardObject::SetTransformMatrix(DirectX::XMMATRIX transformMatrix)
{
    XMStoreFloat4x4(&_worldTM, transformMatrix);
}

/// <summary>
///  업데이트 다음 렌더 전 호출
/// </summary>
/// <param name="_lights"></param>
void ImpGraphics::ForwardObject::SetLight(std::unordered_map<size_t, LightBase*> _lights, float emissionCoefficient)
{
    Shader::QuadConstantBuffer test{};

    test.numOfDirectionalLight = 0;
    test.numOfSpotLight        = 0;
    test.numOfPointLight       = 0;

    for (const auto& light : _lights)
    {
        // 0 Directional, 1 Spot, 2 Point
        size_t lightType = light.second->GetLightType();

        static bool isOverLight = false;

        switch (lightType)
        {
            case 0:
            {
                DirectionalLight* temp = dynamic_cast<DirectionalLight*>(
                    light.second);

                test.directionalData[test.numOfDirectionalLight++] = *temp->GetLightData();
                if (!isOverLight && test.numOfDirectionalLight > 3)
                {
                    MessageBox(NULL,
                        L"Directional Light가 3개가 넘었습니다. 플밍에게 문의 "
                        L"부탁드립니다...",
                        L"에러",
                        MB_ICONERROR);
                    isOverLight = true;
                }
            }
            break;
            case 1:
            {
                SpotLight* temp = dynamic_cast<SpotLight*>(light.second);

                test.spotData[test.numOfSpotLight++] = *temp->GetLightData();
                if (!isOverLight && test.numOfSpotLight > 10)
                {
                    MessageBox(NULL,
                        L"Spot Light가 10개가 넘었습니다. 플밍에게 문의 "
                        L"부탁드립니다...",
                        L"에러",
                        MB_ICONERROR);
                    isOverLight = true;
                }
            }
            break;
            case 2:
            {
                PointLight* temp = dynamic_cast<PointLight*>(light.second);

                test.pointData[test.numOfPointLight++] = *temp->GetLightData();
                if (!isOverLight && test.numOfPointLight > 30)
                {
                    MessageBox(NULL,
                        L"Point Light가 30개가 넘었습니다. 플밍에게 문의 "
                        L"부탁드립니다...",
                        L"에러",
                        MB_ICONERROR);
                    isOverLight = true;
                }
            }
            break;
            default:
                break;
        }
    }

    //DirectX::XMMATRIX view = XMLoadFloat4x4(&_viewTM);

    //XMStoreFloat4x4(&_viewTM, DirectX::XMMatrixInverse(nullptr, view));

    test.eyePos.x = _viewTM._41;
    test.eyePos.y = _viewTM._42;
    test.eyePos.z = _viewTM._43;

    test.globalEmissionCoefficient = emissionCoefficient;
    test.objectEmissionCoefficient = _emissionCoefficient;

    _shader->SetConstantBuffer(test);
}

void ImpGraphics::ForwardObject::SetAlpha(float alpha)
{
    _alpha = alpha;
}
