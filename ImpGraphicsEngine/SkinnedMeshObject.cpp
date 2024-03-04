#include "pch.h"
#include "SkinnedMeshObject.h"

#include "ImpDevice.h"
#include "ImpCamera.h"
#include "ImpRenderTarget.h"
#include "ImpRenderState.h"

#include "Resource.h"

#include "../ImpStructure/ImpMesh.h"

#include "Material.h"

ImpGraphics::SkinnedMeshObject::SkinnedMeshObject(std::shared_ptr<GeometryBuffer> vbib, std::shared_ptr<Shader> shader, Material* material /*= nullptr*/)
    : _changingCount(false)
{
	_geometryBuffer = vbib;
	_shader = shader;
    _material       = material;
}

ImpGraphics::SkinnedMeshObject::~SkinnedMeshObject()
{
    delete _material;
}

void ImpGraphics::SkinnedMeshObject::Update(ImpCamera* camera)
{
	_viewTM = camera->GetViewTM();
	_projTM = camera->GetProjTM();
}

void ImpGraphics::SkinnedMeshObject::Render(ImpDevice* device, ImpRenderState* renderState, ImpRenderTarget* rendertarget)
{
	// 1st Pass
	ID3D11DeviceContext* deviceContext = device->GetDeviceContext();

	// temp 샘플러 설정
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

	for (UINT i = 0; i < 1; i++)
	{
		UINT stride = _geometryBuffer->GetVertexSize();
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, _geometryBuffer->GetVB(i), &stride, &offset);
		deviceContext->IASetIndexBuffer(_geometryBuffer->GetIB(i), DXGI_FORMAT_R32_UINT, 0);

		DirectX::XMMATRIX view = XMLoadFloat4x4(&_viewTM);
		DirectX::XMMATRIX proj = XMLoadFloat4x4(&_projTM);
		DirectX::XMMATRIX world = XMLoadFloat4x4(&_worldTM);
		DirectX::XMMATRIX worldViewProj = world * view * proj;

		/// ---------------------skinning-----------------------
		if (_changingCount)
		{
            _preFinalBoneTMList.clear();
		}
		_finalBoneTMList.clear();
		for (int i = 0; i < _BoneList.size(); i++)
		{
			ImpStructure::Bone* tempBone = _BoneList[i];

			DirectX::XMMATRIX _boneWorldTM = DirectX::XMMATRIX(&tempBone->_boneTM_WorldTM._11);
			DirectX::XMMATRIX _boneNodeTM = DirectX::XMMATRIX(&tempBone->_boneTM_NodeTM._11);

			DirectX::XMMATRIX _skinWorldTM = DirectX::XMMATRIX(&_geometryBuffer->GetWorldTransform(0)._11);
			DirectX::XMMATRIX _skinWorldTM_Inverse = XMMatrixInverse(nullptr, _skinWorldTM);

			DirectX::XMMATRIX _boneoffsetTM = _boneNodeTM * _skinWorldTM_Inverse;
			DirectX::XMMATRIX _boneoffsetTM_Inverse = XMMatrixInverse(nullptr, _boneoffsetTM);

			DirectX::XMMATRIX _finalboneTM = _boneoffsetTM_Inverse * _boneWorldTM;

			DirectX::XMFLOAT4X4 _finalTM /*= Matrix::Identity*/;
			XMStoreFloat4x4(&_finalTM, _finalboneTM);
			_finalBoneTMList.push_back(_finalTM);

			if (_changingCount)
            {
				ImpStructure::Bone* tempBone = _preBoneList[i];

				DirectX::XMMATRIX _boneWorldTM2 = DirectX::XMMATRIX(&tempBone->_boneTM_WorldTM._11);

                DirectX::XMMATRIX _finalboneTM2 = _boneoffsetTM_Inverse * _boneWorldTM2;

                DirectX::XMFLOAT4X4 _finalTM2 /*= Matrix::Identity*/;
                XMStoreFloat4x4(&_finalTM2, _finalboneTM2);
                _preFinalBoneTMList.push_back(_finalTM2);
            }
		}
		/// ---------------------End skinning-----------------------

		deviceContext->VSSetShader(_shader->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_shader->GetPixelShader(), nullptr, 0);

		// ----------------------상수버퍼----------------------------
		Shader::SkinnedMeshConstantBuffer test{};

		XMStoreFloat4x4(&test.world, world);
		XMStoreFloat4x4(&test.transform, worldViewProj);
		XMStoreFloat4x4(&test.worldInvTranspose, XMMatrixInverse(nullptr, world));

		//Effects::BasicTexFX->SetBoneTransforms(&(_finalBoneTMList[0]), _finalBoneTMList.size());
		for (UINT i = 0; i < _finalBoneTMList.size(); i++)
		{
			test.boneTransform[i] = _finalBoneTMList[i];
			if (_changingCount)
			{
                test.preBoneTransform[i] = _preFinalBoneTMList[i];
			}
            //DirectX::XMFLOAT4X4 temptemp;
            //XMStoreFloat4x4(&temptemp, DirectX::XMMatrixIdentity());
            //test.boneTransform[i] = temptemp;
		}

		test.changingCount = _changingCount;
		test.maxChanging = 6;
        test.isOnInterpolation = _isOnInterpolation;

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

		deviceContext->DrawIndexed(_geometryBuffer->GetIndexSize(i), 0, 0);

		ID3D11ShaderResourceView* pSRV = NULL;
		deviceContext->PSSetShaderResources(0, 1, &pSRV);

		pSRV = NULL;
        deviceContext->PSSetShaderResources(1, 1, &pSRV);

		pSRV = NULL;
        deviceContext->PSSetShaderResources(2, 1, &pSRV);

		pSRV = NULL;
        deviceContext->PSSetShaderResources(3, 1, &pSRV);
	}

	myLinearWrapSampler->Release();
}

void ImpGraphics::SkinnedMeshObject::SetTransformMatrix(DirectX::XMMATRIX transformMatrix)
{
	XMStoreFloat4x4(&_worldTM, transformMatrix);
}

void ImpGraphics::SkinnedMeshObject::SetAnimation(const std::wstring& animationName)
{
	if (_BoneList.size() == 0)
	{
        _preBoneList = _geometryBuffer->GetBoneList(animationName);
	}
	else if (_changingCount > 0 && _changingCount < 6)
	{
        int a = 3;
	}
	else
	{
        _preBoneList = _BoneList;
	}
    _BoneList = _geometryBuffer->GetBoneList(animationName);
}

void ImpGraphics::SkinnedMeshObject::SetPreAnimation(
    const std::wstring& animationName)
{
    _preBoneList = _geometryBuffer->GetBoneList(animationName);
}
