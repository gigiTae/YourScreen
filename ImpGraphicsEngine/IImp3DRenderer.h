#pragma once

#define WIN32_LEAN_AND_MEAN

#include "IImpGraphicsEngine.h"

/// <summary>
/// Graphics Engine 을 Game Engine에서 가져다 쓸 때 사용할 h를 따로 뽑아야 겠다. 아닌가 렌더러 정도면 충분히 큰 거 같기도?
/// 어떻게, 뭐가 필요한지는 좀 더 고민을 해봐야겠다.
/// 게임의 Scene 마다 포워드 렌더링이 필요할 지, 디퍼드 렌더링이 필요할 지 다를 수 있겠다. 
/// 그럼 둘다 지원하는 게 맞는 방향이겠지?
/// </summary>
namespace ImpGraphics
{
	/// <summary>
	/// 렌더러 인터페이스
	/// 
	/// 일단은 잘 모르겠어서 교수님의 I3DRenderer.h를 참고했다!
	/// 
	/// 2023.12.12 kimyujin
	/// </summary>
	class IImp3DRenderer abstract
	{
	public:
		IImp3DRenderer() {};
		virtual ~IImp3DRenderer() {};

		/// 가장 기본적으로 렌더러가 돌아가기 위해 필요한 필수 함수
		// 아직 디퍼드는 고려하지 않았다. 
		virtual bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) abstract;
		virtual void Update(float deltaTime) abstract;
		virtual void BeginRender() abstract; // 게임 엔진에서 BeginRender, Render, EndRender 를 구분해서 호출해야 할 이유가 있을까?
		virtual void Render() abstract;
		virtual void EndRender() abstract;
		virtual void Finalize() abstract;

		// 창 변환 관련
		virtual void SetClientSize(int width, int height) abstract;

		/// 그리기 위해 필요한 오브젝트들
        // 카메라 설정
        virtual void UpdateCamera(Matrix transformMatrix) abstract;
        virtual void SetCamera(CameraInfo cameraInfo) abstract;
        // 화면 분할 용
        virtual void Set2ndCamera(CameraInfo cameraInfo) abstract;
        virtual void Update2ndCamera(Matrix transformMatrix) abstract;
        virtual void Has2ndCamera(bool has2ndCamera) abstract;

		virtual void UpdateRight(float deltaTime) abstract;
        virtual void BeginRenderRight() abstract;
        virtual void RenderRight() abstract;

		virtual void AddLight(LightInfo lightInfo) abstract;
		virtual void UpdateLight(size_t lightID, Matrix transformMatrix) abstract;
		virtual void SetLight(size_t lightID, LightInfo lightInfo) abstract;
		virtual void DeleteLight(size_t lightID) abstract;

		virtual void AddMeshObejct(MeshObjectInfo meshObjectInfo) abstract;
		virtual void UpdateMeshObject(size_t objectID, Matrix _transformMatrix) abstract;
		virtual void DeleteMeshObject(size_t _objectID) abstract;

		virtual void AddSkeletalAnimationObject(MeshObjectInfo meshObjectInfo) abstract;
        virtual void AddSkeletalAnimation(size_t objectID, std::wstring fbx) abstract;
        virtual void PlaySkeletalAnimation(size_t objectID, std::wstring fbx) abstract;
		virtual void UpdateSkeletalAnimationSpeed(size_t objectID, std::wstring fbx, float speed) abstract;

		virtual void SetDefaultSkeltalAnimation(size_t objectID, std::wstring fbx) abstract;
        virtual void PlaySkeletalAnimationNonLoop(size_t objectID, std::wstring fbx) abstract;
        virtual std::wstring GetCurAnimation(size_t objectID) abstract;

		virtual void SetInterpolation(size_t objectID, bool onOff) abstract;

		virtual void SetTransparentObjectAlpha(size_t objectID, float customAlpha) abstract;
        virtual void SetWorldEmissionCoefficient(float Emssion) abstract;
        virtual void SetObjectEmissionCoefficient(size_t objectID, float Emssion) abstract;

		/// 충돌을 위한 BoundingVolume 오브젝트 추가
		virtual void AddCubeBoundingVolume(size_t _boundingVolumeID, /*Vector3 center,*/ Vector3 length) abstract;
		virtual void AddSphereBoundingVolume(size_t _boundingVolumeID, /*Vector3 center,*/ float radius) abstract;
		virtual void UpdateBoundingVolume(size_t boundingVolumeID, Matrix transformMatrix, Vector4 color) abstract;
		virtual void DeleteBoundingVolume(size_t boundingVolumeID) abstract;

		/// Picking!
		virtual size_t GetPickingObject(long mouseX, long mouseY) abstract;

		/// 그래픽스 엔진 제어
		virtual void SetGrid(bool isOn) abstract;
		virtual void SetAxis(bool isOn) abstract;
		virtual void SetWireFrame(bool isOn) abstract;
		// 배경 설정
		virtual void SetSkyBox(std::wstring cubeMapPath) abstract;
        /// UI
        virtual void AddUI(size_t uiID, UIInfo uiInfo) abstract;
        virtual void DeleteUI(size_t uiID) abstract;
        virtual void UpdateUI(size_t uiID, float x, float y, float alpha) abstract;
        virtual void SetUI(size_t uiID, UIInfo uiInfo) abstract;
        virtual void SetUILayer(size_t uiID, size_t layer) abstract;

        virtual void RenderUI() abstract;

		virtual void SetUINoise(size_t uiID, bool isOn) abstract;

		virtual void SetUIXRatio(size_t uiID, float ratio) abstract;

		/// 오브젝트에 딱 맞는 bounding box를 반환 해준다.
        virtual Vector3 GetBoundingBoxLength(std::wstring meshPath, Vector3& offset) abstract; 

		/// IMGUI
		virtual void* GetDevice() abstract;
		virtual void* GetDeviceContext() abstract;
	};
}