#pragma once
#include "IImpGraphicsEngine.h"

namespace ImpGraphics 
{
	class IImp3DRenderer;

	class ImpGraphicsEngine : public IImpGraphicsEngine
	{
	public:
		ImpGraphicsEngine();
		virtual ~ImpGraphicsEngine();

		/// 기본 인터페이스
		virtual bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) override;
		virtual void Update(float deltaTime) override;
		virtual void BeginRender() override;
		virtual void Render(RendererType rendererType) override;
		virtual void EndRender() override;
		virtual void Finalize() override;

		/// 창 크기 변환
		virtual void SetClientSize(int width, int height) override;

		/// 그리기 위해 필요한 오브젝트들
		// 카메라 설정
		virtual void UpdateCamera(Matrix transformMatrix) override;
		virtual void SetCamera(CameraInfo cameraInfo) override;
		// 화면 분할 용
        virtual void Set2ndCamera(CameraInfo cameraInfo) override;
        virtual void Update2ndCamera(Matrix transformMatrix) override;
        virtual void Has2ndCamera(bool has2ndCamera) override;

		virtual void UpdateRight(float deltaTime) override;
        virtual void BeginRenderRight() override;
        virtual void RenderRight(RendererType rendererType) override;

		virtual void AddLight(LightInfo lightInfo) override;
		virtual void UpdateLight(size_t lightID, Matrix transformMatrix) override;
		virtual void SetLight(size_t lightID, LightInfo lightInfo) override;
		virtual void DeleteLight(size_t lightID) override;

		virtual void AddMeshObejct(MeshObjectInfo meshObjectInfo) override;
		virtual void UpdateMeshObject(size_t objectID, Matrix transformMatrix) override;
		virtual void DeleteMeshObject(size_t objectID) override;

		virtual void AddSkeletalAnimationObject(MeshObjectInfo meshObjectInfo) override;
		virtual void AddSkeletalAnimation(size_t objectID, std::wstring fbx) override;
        virtual void PlaySkeletalAnimation(size_t objectID, std::wstring fbx) override;
		virtual void UpdateSkeletalAnimationSpeed(size_t objectID, std::wstring fbx, float speed) override;

		virtual void SetDefaultSkeltalAnimation(size_t objectID, std::wstring fbx) override;
        virtual void PlaySkeletalAnimationNonLoop(size_t objectID, std::wstring fbx) override;
        virtual std::wstring GetCurAnimation(size_t objectID) override;

		virtual void SetInterpolation(size_t objectID, bool onOff) override;

		virtual void SetTransparentObjectAlpha(size_t objectID, float customAlpha) override;
		virtual void SetWorldEmissionCoefficient(float emission) override;
        virtual void SetObjectEmissionCoefficient(size_t objectID, float emission) override;

		/// 충돌을 위한 BoundingVolume 오브젝트 추가
		virtual void AddCubeBoundingVolume(size_t _boundingVolumeID, /*Vector3 center,*/ Vector3 length) override;
		virtual void AddSphereBoundingVolume(size_t _boundingVolumeID, /*Vector3 center,*/ float radius) override;
		virtual void UpdateBoundingVolume(size_t boundingVolumeID, Matrix transformMatrix, Vector4 color) override;
		virtual void DeleteBoundingVolume(size_t boundingVolumeID) override;

		/// Picking!
		virtual size_t GetPickingObject(long mouseX, long mouseY) override;

		/// 그래픽스 엔진 제어
		virtual void SetGrid(bool isOn) override;
		virtual void SetAxis(bool isOn) override;
		virtual void SetWireFrame(bool isOn) override;
		// 배경 설정
		virtual void SetSkyBox(std::wstring cubeMapPath) override;
        /// UI
        virtual void AddUI(size_t uiID, UIInfo uiInfo) override;
        virtual void DeleteUI(size_t uiID) override;
        virtual void UpdateUI(size_t uiID, float x, float y, float alpha) override;
        virtual void SetUI(size_t uiID, UIInfo uiInfo) override;
        virtual void SetUILayer(size_t uiID, size_t layer) override;
        virtual void RenderUI() override;

		virtual void SetUINoise(size_t uiID, bool isOn) override;

		virtual void SetUIXRatio(size_t uiID, float ratio) override;

		/// IMGUI 전용
		// 를 위해서 필요한 Device, DeviceContext 반환
		virtual void* GetDevice() override;
		virtual void* GetDeviceContext() override;

		/// 오브젝트에 딱 맞는 bounding box를 반환 해준다.
        virtual Vector3 GetBoundingBoxLength(std::wstring meshPath, Vector3& offset) override;

	private:
		IImp3DRenderer* _renderer;
	};
}

