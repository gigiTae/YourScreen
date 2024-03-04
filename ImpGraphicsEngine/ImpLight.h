#pragma once
#include "IImpGraphicsEngine.h"

namespace ImpGraphics
{
	class ImpLight
	{
		ImpLight();
		~ImpLight();

		void Update();
	};

	/*class ImpCamera
	{
	public:
		ImpCamera(int width, int height);
		~ImpCamera(); // 만약 인터페이스를 따로 만들거나 하려면 가상소멸자로

		void Update(const CameraInfo& cameraInfo);

		void MakeViewTM(const CameraInfo& cameraInfo);
		DirectX::XMFLOAT4X4 GetViewTM();

		void MakePerspectiveProjTM(const CameraInfo& cameraInfo);
		void MakeOthographicProjTM(const CameraInfo& cameraInfo);
		DirectX::XMFLOAT4X4 GetProjTM();

		void SetScreenSize(int width, int height);

	private:
		int _screenWidth;
		int _screenHeight;

		DirectX::XMFLOAT4X4 _viewTM;
		DirectX::XMFLOAT4X4 _projTM;
	};*/
}