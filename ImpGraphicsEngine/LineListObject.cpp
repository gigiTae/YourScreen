#include "pch.h"
#include "LineListObject.h"

#include "ImpDevice.h"
#include "ImpCamera.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"

ImpGraphics::LineListObject::LineListObject(std::shared_ptr<GeometryBuffer> vbib, std::shared_ptr<Shader> shader)
{
	_geometryBuffer = vbib;
	_shader = shader;
}

ImpGraphics::LineListObject::~LineListObject()
{

}

void ImpGraphics::LineListObject::Update(ImpCamera* camera)
{
	_viewTM = camera->GetViewTM();
	_projTM = camera->GetProjTM();
}

void ImpGraphics::LineListObject::Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget)
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
	DirectX::XMMATRIX worldViewProj = world * view * proj;

	deviceContext->VSSetShader(_shader->GetVertexShader(), nullptr, 0);
	deviceContext->PSSetShader(_shader->GetPixelShader(), nullptr, 0);

	Shader::WorldViewProjConstantBuffer test{};

	XMStoreFloat4x4(&test.worldViewProj, worldViewProj);

	_shader->SetConstantBuffer(test);

	deviceContext->DrawIndexed(_geometryBuffer->GetIndexSize(), 0, 0);
}

void ImpGraphics::LineListObject::SetTransformMatrix(DirectX::XMMATRIX transformMatrix)
{
	XMStoreFloat4x4(&_worldTM, transformMatrix);
}

std::shared_ptr<ImpGraphics::GeometryBuffer> ImpGraphics::LineListObject::GetGeometryBuffer()
{
	return _geometryBuffer;
}