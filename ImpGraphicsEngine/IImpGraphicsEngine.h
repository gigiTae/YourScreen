#pragma once
#include "../ImpStructure/ImpMath.h"
#include <string>
// directx
#pragma comment(lib, "d3d11.lib")

#ifdef _DEBUG
#pragma comment(lib, "../ImpLib/DirectXTK.lib")
#pragma comment(lib, "../ImpLib/ImpFBXLoader.lib")
#pragma comment(lib, "../ImpLib/ImpASELoader.lib")
#else
#pragma comment(lib, "../ImpLib/DirectXTKR.lib")
#pragma comment(lib, "../ImpLib/ImpFBXLoaderR.lib")
#pragma comment(lib, "../ImpLib/ImpASELoaderR.lib")
#endif // DEBUG

// loader

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
		float _intensity = 1;

		// Point Light
		Vector3 _attenuation = {0, 0, 1}; // 물리적으로 정확한 값은 거리의 역인 0, 0, 1 이지만 아티스트가 원하는 느낌을 줄 수 있도록 // 입력안하면 0,0,1
		float _range = 10; // 빛의 범위
        float _spot  = 1; // 아마도 spot 지름?
	};

	struct CameraInfo
	{
		bool  _isPerspective = false;
		float _fieldOfView = 0;
		float _nearPlain = 0;
		float _farPlain = 0;
	};

	struct MaterialPath
	{
        std::wstring baseColor         = L"";
        std::wstring metallicRoughness = L"";
        std::wstring emission          = L"";
        std::wstring normalMap         = L"";
        //BaseColor, Roughness, Metallic, Emission
	};

	struct MeshObjectInfo
	{
		size_t _objectID;

		std::wstring _meshPath;

		std::wstring _vertexShaderPath;
		std::wstring _pisxelShaderPath;
		
		MaterialPath _material;
        bool _isOpaque; // false 면 transparent true면 불투명
        bool _isCullNone = false; // 양면 그리기
	};

	struct UIInfo
	{
        size_t _UIID;

		std::wstring _texturePath;
		
		Vector4 _UIPos; //x: startX, y: startY, z: width, w: height // x, y 는 일단 안씀
        size_t _layer;
	};

	/// <summary>
	/// 일단 게임엔진에서 사용할 함수가 뭐가 있는지 생각해보자.
	/// 1. 라이트, 카메라, 메쉬가 필요하다.
	/// 2. 렌더러를 생각하지 않아도 렌더링 방식만 선택하게 하면 좋을 것 같다.
	/// 
	/// 렌더러를 감싼 그래픽스 엔진이 필요할 것 같은데
	/// 1. 얘도 interface를 만들고 구현은 따로 두는 게 좋겠지?
	/// 2. 렌더러에 있는 함수들을 쓰려면 그 함수들의 시그니처를 또 가져와야 하는 건가?
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

		/// 기본 인터페이스
		virtual IMPGRAPHICS bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) abstract;
		virtual IMPGRAPHICS void Update(float deltaTime) abstract;
		virtual IMPGRAPHICS void BeginRender() abstract;
		virtual IMPGRAPHICS void Render(RendererType rendererType) abstract;
		virtual IMPGRAPHICS void EndRender() abstract;
		virtual IMPGRAPHICS void Finalize() abstract;

		/// 창 크기 변환
		virtual IMPGRAPHICS void SetClientSize(int width, int height) abstract;

		/// 그리기 위해 필요한 오브젝트들
		// 카메라 설정
		virtual IMPGRAPHICS void UpdateCamera(Matrix transformMatrix) abstract;
		virtual IMPGRAPHICS void SetCamera(CameraInfo cameraInfo) abstract;
		// 화면 분할 용
        virtual IMPGRAPHICS void Set2ndCamera(CameraInfo cameraInfo) abstract;
        virtual IMPGRAPHICS void Update2ndCamera(Matrix transformMatrix) abstract;
        virtual IMPGRAPHICS void Has2ndCamera(bool has2ndCamera) abstract;

		virtual IMPGRAPHICS void UpdateRight(float deltaTime) abstract;
        virtual IMPGRAPHICS void BeginRenderRight() abstract;
        virtual IMPGRAPHICS void RenderRight(RendererType rendererType) abstract;

		virtual IMPGRAPHICS void AddLight(LightInfo lightInfo) abstract;
		virtual IMPGRAPHICS void UpdateLight(size_t lightID, Matrix transformMatrix) abstract;
		virtual IMPGRAPHICS void SetLight(size_t lightID, LightInfo lightInfo) abstract;
		virtual IMPGRAPHICS void DeleteLight(size_t lightID) abstract;

		virtual IMPGRAPHICS void AddMeshObejct(MeshObjectInfo meshObjectInfo) abstract;
		virtual IMPGRAPHICS void UpdateMeshObject(size_t objectID, Matrix transformMatrix) abstract;
		virtual IMPGRAPHICS void DeleteMeshObject(size_t objectID) abstract;

		virtual IMPGRAPHICS void AddSkeletalAnimationObject(MeshObjectInfo meshObjectInfo) abstract;
        virtual IMPGRAPHICS void AddSkeletalAnimation(size_t objectID, std::wstring fbx) abstract;

        virtual IMPGRAPHICS void PlaySkeletalAnimation(size_t objectID, std::wstring fbx) abstract;
        virtual IMPGRAPHICS void PlaySkeletalAnimationNonLoop(size_t objectID, std::wstring fbx) abstract; // NonLoop를 하려면 무조건 SetDefault를 해줘야됨

        virtual IMPGRAPHICS void SetDefaultSkeltalAnimation(size_t objectID, std::wstring fbx) abstract;
        virtual IMPGRAPHICS void UpdateSkeletalAnimationSpeed(size_t objectID, std::wstring fbx, float speed) abstract;
        virtual IMPGRAPHICS std::wstring GetCurAnimation(size_t objectID) abstract;

		virtual IMPGRAPHICS void SetInterpolation(size_t objectID, bool onOff) abstract;

		virtual IMPGRAPHICS void SetTransparentObjectAlpha(size_t objectID, float customAlpha) abstract;
        virtual IMPGRAPHICS void SetWorldEmissionCoefficient(float Emssion) abstract;
        virtual IMPGRAPHICS void SetObjectEmissionCoefficient(size_t objectID, float Emssion) abstract;

		/// 충돌을 위한 BoundingVolume 오브젝트 추가
		// Cube의 length는 Half임!
		virtual IMPGRAPHICS void AddCubeBoundingVolume(size_t boundingVolumeID, /*Vector3 center,*/ Vector3 length) abstract; // 일단 센터는 0으로 고정
		virtual IMPGRAPHICS void AddSphereBoundingVolume(size_t boundingVolumeID, /*Vector3 center,*/ float radius) abstract; // TM를 쓸 거라서
		virtual IMPGRAPHICS void UpdateBoundingVolume(size_t boundingVolumeID, Matrix transformMatrix, Vector4 color) abstract;
		virtual IMPGRAPHICS void DeleteBoundingVolume(size_t boundingVolumeID) abstract;

		/// Picking!
		virtual IMPGRAPHICS size_t GetPickingObject(long mouseX, long mouseY) abstract;

		/// 그래픽스 엔진 제어
		virtual IMPGRAPHICS void SetGrid(bool isOn) abstract;
		virtual IMPGRAPHICS void SetAxis(bool isOn) abstract;
		virtual IMPGRAPHICS void SetWireFrame(bool isOn) abstract;
		// 배경 설정
		virtual IMPGRAPHICS void SetSkyBox(std::wstring cubeMapPath) abstract;
		/// UI
        virtual IMPGRAPHICS void AddUI(size_t uiID, UIInfo uiInfo) abstract;
        virtual IMPGRAPHICS void UpdateUI(size_t uiID, float x, float y, float alpha) abstract;
        virtual IMPGRAPHICS void SetUI(size_t uiID, UIInfo uiInfo) abstract;
        virtual IMPGRAPHICS void SetUILayer(size_t uiID, size_t layer) abstract;
        virtual IMPGRAPHICS void DeleteUI(size_t uiID) abstract;
        virtual IMPGRAPHICS void RenderUI() abstract;

		virtual IMPGRAPHICS void SetUINoise(size_t uiID, bool isOn) abstract;

		virtual IMPGRAPHICS void SetUIXRatio(size_t uiID, float ratio) abstract;

		/// IMGUI 전용
		// 를 위해서 필요한 Device, DeviceContext 반환
		virtual IMPGRAPHICS void* GetDevice() abstract;
		virtual IMPGRAPHICS void* GetDeviceContext() abstract;

		/// 오브젝트에 딱 맞는 bounding box를 반환 해준다.
        virtual Vector3 GetBoundingBoxLength(std::wstring meshPath, Vector3& offset) abstract;
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