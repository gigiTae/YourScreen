#pragma once

namespace ImpMath { struct Matrix; }

namespace ImpGraphics
{
	struct CameraInfo;

	class ImpCamera
	{
	public:
		ImpCamera(int width, int height);
		~ImpCamera(); // 만약 인터페이스를 따로 만들거나 하려면 가상소멸자로

		void SetCamera(const CameraInfo& cameraInfo);
		void Update(ImpMath::Matrix transformMatrix);
		void MakeTM();

		DirectX::XMFLOAT4X4 GetViewTM() { return _viewTM; }

		void MakePerspectiveProjTM();
		void MakeOthographicProjTM();
		DirectX::XMFLOAT4X4 GetProjTM() { return _projTM; }

		float GetFieldOfView() const {return _fieldOfView;};
        float GetNearPlain() const {return _nearPlain;};
        float GetFarPlain() const {return _farPlain;};

		DirectX::XMFLOAT3 GetPosition() { return _position; }
		DirectX::XMFLOAT4 GetRotationQut() {return _rotationQut;}

		DirectX::XMMATRIX gettempSkyBox();

		void SetScreenSize(int width, int height);

	private:
		int _screenWidth;
		int _screenHeight;

		DirectX::XMFLOAT4X4 _viewTM;
		DirectX::XMFLOAT4X4 _projTM;

		bool  _isPerspective;
		float _fieldOfView;
		float _nearPlain;
		float _farPlain;

		DirectX::XMFLOAT3 _position;
        DirectX::XMFLOAT4 _rotationQut;
	};
}