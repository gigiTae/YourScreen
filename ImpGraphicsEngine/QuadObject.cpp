#include "pch.h"
#include "QuadObject.h"

#include "ImpDevice.h"
#include "ImpCamera.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"

// temp
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

ImpGraphics::QuadObject::QuadObject(std::shared_ptr<GeometryBuffer> vbib, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> irr)
    : _emissionCoefficient(1)
{
	_geometryBuffer = vbib;
	_shader = shader;
	_irr = irr;
}

ImpGraphics::QuadObject::~QuadObject()
{

}

void ImpGraphics::QuadObject::Update(ImpCamera* camera)
{
	_viewTM = camera->GetViewTM();
	_projTM = camera->GetProjTM();
}

void ImpGraphics::QuadObject::Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget)
{
	ID3D11DeviceContext* deviceContext = device->GetDeviceContext();

	// 샘플러를 어디서 만들어서 넣어야 할까! 여긴 아닌게 확실하다!
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	ID3D11SamplerState* myLinearWrapSampler;
	HRESULT hr = device->GetDevice()->CreateSamplerState(&samplerDesc, &myLinearWrapSampler);
	deviceContext->PSSetSamplers(0, 1, &myLinearWrapSampler);

	deviceContext->IASetInputLayout(_shader->GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->RSSetState(renderState->GetSolidRS());

	UINT stride = _geometryBuffer->GetVertexSize();
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, _geometryBuffer->GetVB(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_geometryBuffer->GetIB(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->VSSetShader(_shader->GetVertexShader(), nullptr, 0);
	deviceContext->PSSetShader(_shader->GetPixelShader(), nullptr, 0);

	std::vector<ID3D11ShaderResourceView*> temp = rendertarget->GetDeferredShaderResourceViews();
	deviceContext->PSSetShaderResources(0, 1, &temp[0]);
	deviceContext->PSSetShaderResources(1, 1, &temp[1]);
	deviceContext->PSSetShaderResources(2, 1, &temp[2]);
	deviceContext->PSSetShaderResources(3, 1, &temp[3]);
	deviceContext->PSSetShaderResources(4, 1, &temp[4]);

	//_shader->SetConstantBuffer2(test);

	if (_irr)
	{
		deviceContext->PSSetShaderResources(5, 1, &_irr->GetTextureSRV());
	}

	deviceContext->DrawIndexed(6, 0, 0);

	if (_irr)
	{
		ID3D11ShaderResourceView* pSRV = NULL;
		deviceContext->PSSetShaderResources(5, 1, &pSRV);
	}

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

	myLinearWrapSampler->Release();
	rendertarget->FirstPassUnbind();
}

void ImpGraphics::QuadObject::SetTransformMatrix(DirectX::XMMATRIX transformMatrix)
{

}

std::shared_ptr<ImpGraphics::GeometryBuffer> ImpGraphics::QuadObject::GetGeometryBuffer()
{
	return _geometryBuffer;
}

/// <summary>
///  업데이트 다음 렌더 전 호출
/// </summary>
/// <param name="_lights"></param>
void ImpGraphics::QuadObject::SetLight(std::unordered_map<size_t, LightBase*> _lights, float globalEmissionCoefficient)
{
	Shader::QuadConstantBuffer test{};

	test.numOfDirectionalLight = 0;
	test.numOfSpotLight = 0;
	test.numOfPointLight = 0;

	for (const auto& light : _lights)
	{
		// 0 Directional, 1 Spot, 2 Point
		size_t lightType = light.second->GetLightType();

		static bool isOverLight = false;

		switch (lightType)
		{
			case 0:
			{
				DirectionalLight* temp = dynamic_cast<DirectionalLight*>(light.second);

				test.directionalData[test.numOfDirectionalLight++] = *temp->GetLightData();
                if (!isOverLight && test.numOfDirectionalLight > 3)
				{
                    MessageBox(NULL, L"Directional Light가 3개가 넘었습니다. 플밍에게 문의 부탁드립니다...", L"에러", MB_ICONERROR);
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
                    MessageBox(NULL, L"Spot Light가 10개가 넘었습니다. 플밍에게 문의 부탁드립니다...", L"에러", MB_ICONERROR);
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
                    MessageBox(NULL, L"Point Light가 30개가 넘었습니다. 플밍에게 문의 부탁드립니다...", L"에러", MB_ICONERROR);
                    isOverLight = true;
				}
			}
				break;
			default:
				break;
		}
	}

	DirectX::XMMATRIX view = XMLoadFloat4x4(&_viewTM);

	XMStoreFloat4x4(&_viewTM, DirectX::XMMatrixInverse(nullptr, view));

	test.eyePos.x = _viewTM._41;
	test.eyePos.y = _viewTM._42;
	test.eyePos.z = _viewTM._43;

	test.globalEmissionCoefficient = globalEmissionCoefficient;
    test.objectEmissionCoefficient = _emissionCoefficient;

	_shader->SetConstantBuffer(test);
}
