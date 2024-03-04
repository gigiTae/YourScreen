#pragma once
#include "../ImpStructure/ImpMath.h"
#include <string>
// directx
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "../ImpLib/DirectXTK.lib")
// loader
#pragma comment(lib, "../ImpLib/ImpFBXLoader.lib")
#pragma comment(lib, "../ImpLib/ImpASELoader.lib")

#ifdef IMPGRAPHICS
#define IMPGRAPHICS __declspec(dllexport)
#else
#define IMPGRAPHICS __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif 

using namespace ImpMath;

namespace ImpGraphics
{
	struct LightInfo
	{
		size_t _lightID;

		size_t _lightType; // 0 Directional, 1 Spot, 2 Point
		Vector4 _color = {1.0f, 1.0f, 1.0f, 1.0f};
		size_t _intensity = 1;

		// Point Light
		Vector3 _attenuation = {0, 0, 1}; // ���������� ��Ȯ�� ���� �Ÿ��� ���� 0, 0, 1 ������ ��Ƽ��Ʈ�� ���ϴ� ������ �� �� �ֵ��� // �Է¾��ϸ� 0,0,1
		float _range = 10; // ���� ����
	};

	struct CameraInfo
	{
		bool  _isPerspective = false;
		float _fieldOfView = 0;
		float _nearPlain = 0;
		float _farPlain = 0;
	};

	struct MeshObjectInfo
	{
		size_t _objectID;

		std::wstring _meshPath;

		std::wstring _vertexShaderPath;
		std::wstring _pisxelShaderPath;
	};

	/// <summary>
	/// �ϴ� ���ӿ������� ����� �Լ��� ���� �ִ��� �����غ���.
	/// 1. ����Ʈ, ī�޶�, �޽��� �ʿ��ϴ�.
	/// 2. �������� �������� �ʾƵ� ������ ��ĸ� �����ϰ� �ϸ� ���� �� ����.
	/// 
	/// �������� ���� �׷��Ƚ� ������ �ʿ��� �� ������
	/// 1. �굵 interface�� ����� ������ ���� �δ� �� ������?
	/// 2. �������� �ִ� �Լ����� ������ �� �Լ����� �ñ״�ó�� �� �����;� �ϴ� �ǰ�?
	/// </summary>
	class IImpGraphicsEngine
	{
	public:
		enum class RendererType
		{
			Forward,
			Deferred
		};

	public:
		IImpGraphicsEngine() {};
		virtual ~IImpGraphicsEngine() {};

		/// �⺻ �������̽�
		virtual IMPGRAPHICS bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) abstract;
		virtual IMPGRAPHICS void Update(float deltaTime) abstract;
		virtual IMPGRAPHICS void BeginRender() abstract;
		virtual IMPGRAPHICS void Render(RendererType rendererType) abstract;
		virtual IMPGRAPHICS void EndRender() abstract;
		virtual IMPGRAPHICS void Finalize() abstract;

		/// â ũ�� ��ȯ
		virtual IMPGRAPHICS void SetClientSize(int width, int height) abstract;

		/// �׸��� ���� �ʿ��� ������Ʈ��
		virtual IMPGRAPHICS void UpdateCamera(Matrix transformMatrix) abstract;
		virtual IMPGRAPHICS void SetCamera(CameraInfo cameraInfo) abstract;
		
		virtual IMPGRAPHICS void AddLight(LightInfo lightInfo) abstract;
		virtual IMPGRAPHICS void UpdateLight(size_t lightID, Matrix transformMatrix) abstract;
		virtual IMPGRAPHICS void SetLight(size_t lightID, LightInfo lightInfo) abstract;
		virtual IMPGRAPHICS void DeleteLight(size_t lightID) abstract;

		virtual IMPGRAPHICS void AddMeshObejct(MeshObjectInfo meshObjectInfo) abstract;
		virtual IMPGRAPHICS void UpdateMeshObject(size_t objectID, Matrix transformMatrix) abstract;
		virtual IMPGRAPHICS void DeleteMeshObject(size_t objectID) abstract;

		/// �׷��Ƚ� ���� ����
		virtual IMPGRAPHICS void SetGrid(bool isOn) abstract;
		virtual IMPGRAPHICS void SetAxis(bool isOn) abstract;
		virtual IMPGRAPHICS void SetWireFrame(bool isOn) abstract;
		// ��� ����
		virtual IMPGRAPHICS void SetSkyBox(std::wstring cubeMapPath) abstract;

		/// IMGUI ����
		// �� ���ؼ� �ʿ��� Device, DeviceContext ��ȯ
		virtual IMPGRAPHICS void* GetDevice() abstract;
		virtual IMPGRAPHICS void* GetDeviceContext() abstract;
	};

	class EngineExporter
	{
	public:
		static IMPGRAPHICS IImpGraphicsEngine* GetEngine();
		static IMPGRAPHICS void DeleteEngine();

	private:
		static IImpGraphicsEngine* _engine;
	};
}

#ifdef __cplusplus
}
#endif 