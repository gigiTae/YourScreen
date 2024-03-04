#include "pch.h"
#include "MeshObject.h"

#include "ImpDevice.h"
#include "ImpCamera.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"

using namespace ImpGraphics;

MeshObject::MeshObject(std::shared_ptr<GeometryBuffer> vbib, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
{
	_geometryBuffer = vbib;
	_shader = shader;
	_texture = texture;
}

MeshObject::~MeshObject()
{

}

void MeshObject::Update(ImpCamera* camera)
{
	_viewTM = camera->GetViewTM();
	_projTM = camera->GetProjTM();
}

void ImpGraphics::MeshObject::Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget)
{
	// 1st Pass
	ID3D11DeviceContext* deviceContext = device->GetDeviceContext();

	// temp »ùÇÃ·¯ ¼³Á¤
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	ID3D11SamplerState* myLinearWrapSampler;
	HRESULT hr = device->GetDevice()->CreateSamplerState(&samplerDesc, &myLinearWrapSampler);
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
		deviceContext->IASetIndexBuffer(_geometryBuffer->GetIB(i), DXGI_FORMAT_R32_UINT, 0);

		DirectX::XMMATRIX view = XMLoadFloat4x4(&_viewTM);
		DirectX::XMMATRIX proj = XMLoadFloat4x4(&_projTM);
		DirectX::XMMATRIX world = XMLoadFloat4x4(&_worldTM);
		DirectX::XMMATRIX worldViewProj = world * view * proj;

		deviceContext->VSSetShader(_shader->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_shader->GetPixelShader(), nullptr, 0);

		Shader::MeshConstantBuffer test{};

		XMStoreFloat4x4(&test.world, world);
		XMStoreFloat4x4(&test.transform, worldViewProj);
		XMStoreFloat4x4(&test.worldInvTranspose, XMMatrixInverse(nullptr, world));

		_shader->SetConstantBuffer(test);

		if (_texture)
		{
			deviceContext->PSSetShaderResources(0, 1, &_texture->GetTextureSRV());
		}

		deviceContext->DrawIndexed(_geometryBuffer->GetIndexSize(i), 0, 0);

		if (_texture)
		{
			ID3D11ShaderResourceView* pSRV = NULL;
			deviceContext->PSSetShaderResources(0, 1, &pSRV);
		}
	}

	myLinearWrapSampler->Release();
}


void MeshObject::SetTransformMatrix(DirectX::XMMATRIX transformMatrix)
{
	XMStoreFloat4x4(&_worldTM, transformMatrix);
}