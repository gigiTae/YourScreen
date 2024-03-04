#include "pch.h"
#include "BoundingVolumeObject.h"

#include "ImpDevice.h"
#include "ImpCamera.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"

ImpGraphics::BoundingVolumeObject::BoundingVolumeObject(std::shared_ptr<GeometryBuffer> vbib, std::shared_ptr<Shader> shader)
{
	_geometryBuffer = vbib;
	_shader = shader;
}

ImpGraphics::BoundingVolumeObject::~BoundingVolumeObject()
{

}

void ImpGraphics::BoundingVolumeObject::Update(ImpCamera* camera)
{
	_viewTM = camera->GetViewTM();
	_projTM = camera->GetProjTM();
}

void ImpGraphics::BoundingVolumeObject::Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget)
{
	ID3D11DeviceContext* deviceContext = device->GetDeviceContext();

	deviceContext->IASetInputLayout(_shader->GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	deviceContext->RSSetState(renderState->GetSolidRS());

	UINT stride = _geometryBuffer->GetVertexSize();
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, _geometryBuffer->GetVB(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_geometryBuffer->GetIB(), DXGI_FORMAT_R32_UINT, 0);

	DirectX::XMMATRIX view = XMLoadFloat4x4(&_viewTM);
	DirectX::XMMATRIX proj = XMLoadFloat4x4(&_projTM);
	DirectX::XMMATRIX world = XMLoadFloat4x4(&_worldTM);
	//DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX worldViewProj = world * view * proj;

	deviceContext->VSSetShader(_shader->GetVertexShader(), nullptr, 0);
	deviceContext->PSSetShader(_shader->GetPixelShader(), nullptr, 0);

	Shader::WorldViewProjConstantBuffer test{};

	XMStoreFloat4x4(&test.worldViewProj, worldViewProj);
	test.color = _color;

	_shader->SetConstantBuffer(test);

	deviceContext->DrawIndexed(_geometryBuffer->GetIndexSize(), 0, 0);
}

void ImpGraphics::BoundingVolumeObject::SetTransformMatrix(DirectX::XMMATRIX transformMatrix)
{
	XMStoreFloat4x4(&_worldTM, transformMatrix);
}

void ImpGraphics::BoundingVolumeObject::SetColor(const Vector4& color)
{
	_color.x = color.x;
	_color.y = color.y;
	_color.z = color.z;
	_color.w = color.w;
}