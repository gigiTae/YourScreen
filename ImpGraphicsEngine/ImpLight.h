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
		~ImpCamera(); // ���� �������̽��� ���� ����ų� �Ϸ��� ����Ҹ��ڷ�

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