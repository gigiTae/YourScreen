#include "pch.h"
#include "BloomQuadObject.h"

#include "ImpDevice.h"
#include "ImpCamera.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"

using namespace ImpGraphics;


 BloomQuadObject::BloomQuadObject(std::shared_ptr<GeometryBuffer> vbib,
                                 std::shared_ptr<Shader> shader,
                                 std::shared_ptr<Texture> deferredTexture)
{
     _geometryBuffer = vbib;
     _shader         = shader;
     _deferredTexture = deferredTexture;

     _samplingShader = std::make_shared<Shader>(shader->GetResourceManager());
   
     _samplingShader->Load(L"../ImpGraphicsEngine/DownSamplingVS.hlsl", L"../ImpGraphicsEngine/DownSamplingPS.hlsl", VertexType::QuadVertex);
 }

BloomQuadObject::~BloomQuadObject()
{
}

void BloomQuadObject::Update(ImpCamera* camera)
{
     _viewTM = camera->GetViewTM();
     _projTM = camera->GetProjTM();
}

void BloomQuadObject::Render(ImpDevice* device,
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
     HRESULT hr = device->GetDevice()->CreateSamplerState(&samplerDesc,
                                                          &myLinearWrapSampler);
     deviceContext->PSSetSamplers(0, 1, &myLinearWrapSampler);

     // downSampling
     for (int i = 0; i < 3; i++)
     {
         rendertarget->DownSamplingPassBind(i);
         rendertarget->DownSamplingPassClear(i);

         deviceContext->IASetInputLayout(_samplingShader->GetInputLayout());
         deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
         deviceContext->RSSetState(renderState->GetSolidRS());

         UINT stride = _geometryBuffer->GetVertexSize();
         UINT offset = 0;
         deviceContext->IASetVertexBuffers(0, 1, _geometryBuffer->GetVB(), &stride, &offset);
         deviceContext->IASetIndexBuffer(_geometryBuffer->GetIB(),
                                         DXGI_FORMAT_R32_UINT,
                                         0);

         deviceContext->VSSetShader(_samplingShader->GetVertexShader(), nullptr, 0);
         deviceContext->PSSetShader(_samplingShader->GetPixelShader(), nullptr, 0);

         Shader::DownSamplingVSConstantBuffer test{};

         test.screenSize = DirectX::XMFLOAT4(1920, 1080, std::pow(rendertarget->_bloomRatio, i + 1), 0);

         _shader->SetConstantBuffer(test);

         std::vector<ID3D11ShaderResourceView*>
             temp = rendertarget->GetBloomShaderResourceViews();
         deviceContext->PSSetShaderResources(0, 1, &temp[1]);

         deviceContext->DrawIndexed(6, 0, 0);

         rendertarget->DownSamplingPassUnbind(i);
         
         // 렌더 타겟 바인딩을 다시 한다.
         // 상수 버퍼 바인딩을 다시 한다.
         // 다시 그린다.

         rendertarget->DownSamplingPassBind(i + 3);
         rendertarget->DownSamplingPassClear(i + 3);

         deviceContext->IASetInputLayout(_samplingShader->GetInputLayout());
         deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
         deviceContext->RSSetState(renderState->GetSolidRS());

         deviceContext->IASetVertexBuffers(0, 1, _geometryBuffer->GetVB(), &stride, &offset);
         deviceContext->IASetIndexBuffer(_geometryBuffer->GetIB(),
                                         DXGI_FORMAT_R32_UINT,
                                         0);

         deviceContext->VSSetShader(_samplingShader->GetVertexShader(), nullptr, 0);
         deviceContext->PSSetShader(_samplingShader->GetPixelShader(), nullptr, 0);

         test.screenSize = DirectX::XMFLOAT4(1920 / std::pow(rendertarget->_bloomRatio,
                                                             i + 1),
                      1080 / std::pow(rendertarget->_bloomRatio, i + 1),
                                             std::pow(rendertarget->_bloomRatio,
                                                      i + 1),
                                             1);

         _shader->SetConstantBuffer(test);

             temp = rendertarget->GetBloomShaderResourceViews();
         deviceContext->PSSetShaderResources(0, 1, &temp[i + 2]);

         deviceContext->DrawIndexed(6, 0, 0);

         rendertarget->DownSamplingPassUnbind(i + 3);
     }

     rendertarget->ThreePassBind();
     rendertarget->ThreePassClear();

     // 여기부터가 렌더
     deviceContext->IASetInputLayout(_shader->GetInputLayout());
     deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
     deviceContext->RSSetState(renderState->GetSolidRS());

     UINT stride = _geometryBuffer->GetVertexSize();
     UINT offset = 0;
     deviceContext->IASetVertexBuffers(0, 1, _geometryBuffer->GetVB(), &stride, &offset);
     deviceContext->IASetIndexBuffer(_geometryBuffer->GetIB(), DXGI_FORMAT_R32_UINT, 0);

     deviceContext->VSSetShader(_shader->GetVertexShader(), nullptr, 0);
     deviceContext->PSSetShader(_shader->GetPixelShader(), nullptr, 0);

     std::vector<ID3D11ShaderResourceView*> temp = rendertarget->GetBloomShaderResourceViews();
     
     deviceContext->PSSetShaderResources(0, 1, &temp[0]);
     deviceContext->PSSetShaderResources(1, 1, &temp[1]);
     deviceContext->PSSetShaderResources(2, 1, &temp[5]);
     deviceContext->PSSetShaderResources(3, 1, &temp[6]);
     deviceContext->PSSetShaderResources(4, 1, &temp[7]);

     deviceContext->DrawIndexed(6, 0, 0);

     ID3D11ShaderResourceView* pSRV = NULL;
     deviceContext->PSSetShaderResources(0, 1, &pSRV);
     pSRV = NULL;
     deviceContext->PSSetShaderResources(1, 1, &pSRV);
     pSRV = NULL;
     deviceContext->PSSetShaderResources(5, 1, &pSRV);
     pSRV = NULL;
     deviceContext->PSSetShaderResources(6, 1, &pSRV);
     pSRV = NULL;
     deviceContext->PSSetShaderResources(7, 1, &pSRV);

     myLinearWrapSampler->Release();
}

void BloomQuadObject::SetTransformMatrix(DirectX::XMMATRIX transformMatrix)
{
}