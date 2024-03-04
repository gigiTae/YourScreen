#include "pch.h"
#include "PBRMeshObject.h"

#include "ImpDevice.h"
#include "ImpCamera.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"

#include "Material.h"

using namespace ImpGraphics;

PBRMeshObject::PBRMeshObject(std::shared_ptr<GeometryBuffer> vbib,
                             std::shared_ptr<Shader> shader,
                             Material* material)
    : _emissionCoefficient(1)
{
    _geometryBuffer = vbib;
    _shader         = shader;
    _material       = material;
}

PBRMeshObject::~PBRMeshObject()
{
    delete _material;
}

void PBRMeshObject::Update(ImpCamera* camera)
{
    _viewTM = camera->GetViewTM();
    _projTM = camera->GetProjTM();
}

void PBRMeshObject::Render(ImpDevice* device,
                           ImpRenderState* renderState,
                           ImpRenderTarget* rendertarget)
{
    // 1st Pass
    ID3D11DeviceContext* deviceContext = device->GetDeviceContext();

    // temp »ùÇÃ·¯ ¼³Á¤
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
    HRESULT hr = device->GetDevice()->CreateSamplerState(&samplerDesc,
                                                         &myLinearWrapSampler);
    deviceContext->PSSetSamplers(0, 1, &myLinearWrapSampler);

    //
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

        Shader::EmissionCoefficientConstantBuffer emissionBuffer{};

        emissionBuffer.objectEmissionCoefficient = _emissionCoefficient;

        _shader->SetConstantBuffer(emissionBuffer);

        deviceContext->DrawIndexed(_geometryBuffer->GetIndexSize(i), 0, 0);

        //if (_texture)
        {
            ID3D11ShaderResourceView* pSRV = NULL;
            deviceContext->PSSetShaderResources(0, 1, &pSRV);

            pSRV = NULL;
            deviceContext->PSSetShaderResources(1, 1, &pSRV);
            pSRV = NULL;
            deviceContext->PSSetShaderResources(2, 1, &pSRV);
            pSRV = NULL;
            deviceContext->PSSetShaderResources(3, 1, &pSRV);
        }
    }

    myLinearWrapSampler->Release();
}

void PBRMeshObject::SetTransformMatrix(DirectX::XMMATRIX transformMatrix)
{
    XMStoreFloat4x4(&_worldTM, transformMatrix);
}