#include "pch.h"
#include "ImpCamera.h"

#include "IImpGraphicsEngine.h"

using namespace DirectX;

ImpGraphics::ImpCamera::ImpCamera(int screenWidth, int screenHeight)
	:_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_viewTM(), _projTM(),
	_isPerspective(true), _fieldOfView(0), _nearPlain(0), _farPlain(0)
{
}

ImpGraphics::ImpCamera::~ImpCamera()
{

}

void ImpGraphics::ImpCamera::Update(Matrix transformMatrix)
{
	//MakeViewTM(cameraInfo);
	_position.x = transformMatrix._41;
	_position.y = transformMatrix._42;
	_position.z = transformMatrix._43;

    XMMATRIX transformMatrixXM(&transformMatrix._11);
    // 변환 행렬로부터 회전 부분을 추출합니다.
    XMVECTOR rotationVector = XMQuaternionRotationMatrix(transformMatrixXM); // 이거 회전행렬 성분만 있는 게 맞나
    // 회전 벡터를 XMFLOAT4 형태로 변환합니다.
    XMStoreFloat4(&_rotationQut, rotationVector);

	DirectX::XMMATRIX viewTM(transformMatrixXM);
	XMStoreFloat4x4(&_viewTM, XMMatrixInverse(nullptr, viewTM));

	MakeTM();
}

void ImpGraphics::ImpCamera::MakeTM()
{
    if (_isPerspective)
    {
        MakePerspectiveProjTM();
    }
    else
    {
        MakeOthographicProjTM();
    }
}

void ImpGraphics::ImpCamera::MakePerspectiveProjTM()
{
	DirectX::XMMATRIX PerspecProjTM = DirectX::XMMatrixPerspectiveFovLH(_fieldOfView, static_cast<float>(_screenWidth) / _screenHeight, _nearPlain, _farPlain);
	XMStoreFloat4x4(&_projTM, PerspecProjTM);
}

void ImpGraphics::ImpCamera::MakeOthographicProjTM()
{
	DirectX::XMMATRIX othoProjTM = DirectX::XMMatrixOrthographicLH(static_cast<float>(_screenWidth), static_cast<float>(_screenWidth), 
		_nearPlain, _farPlain);
	XMStoreFloat4x4(&_projTM, othoProjTM);
}

DirectX::XMMATRIX ImpGraphics::ImpCamera::gettempSkyBox()
{
	DirectX::XMMATRIX view = XMLoadFloat4x4(&_viewTM);
	DirectX::XMMATRIX proj = XMLoadFloat4x4(&_projTM);
	XMMATRIX my = XMMatrixInverse(nullptr, view);
	DirectX::XMFLOAT4X4 temp;
	DirectX::XMStoreFloat4x4(&temp, my);
	XMMATRIX T = XMMatrixTranslation(temp._41, temp._42, temp._43);

	XMMATRIX WVP = T * view * proj;
	return WVP;
}

void ImpGraphics::ImpCamera::SetScreenSize(int width, int height)
{
	_screenWidth = width;
	_screenHeight = height;

	if (std::abs(_farPlain) < 0.000001f)
	{
        return;
	}
	else
	{
        MakeTM();
	}
}

void ImpGraphics::ImpCamera::SetCamera(const CameraInfo& cameraInfo)
{
	_isPerspective = cameraInfo._isPerspective;
	_fieldOfView = cameraInfo._fieldOfView;
	_nearPlain = cameraInfo._nearPlain;
	_farPlain = cameraInfo._farPlain;

	MakeTM();
}
