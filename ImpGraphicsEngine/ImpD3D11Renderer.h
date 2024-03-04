#pragma once
#include "pch.h"
#include "IImp3DRenderer.h"

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;

namespace ImpGraphics
{
	class ImpDevice;
	class ImpRenderTarget;
	class ImpRenderState;

	class ImpCamera;

	class ImpFont;

	//temp
	class IObject;
	class BoundingVolumeObject;
    class IUIObject;
	class QuadObject;
	class BloomQuadObject;
    class ForwardOITQuadObject;
	class Factory;
	class ResourceManager;

	class ImpAnimationPlayer;
	class PickingManager;
	class CullingManager;

	//temp
	class LightBase;

	class ImpD3D11Renderer : public IImp3DRenderer
	{
	public:
		ImpD3D11Renderer();
		virtual ~ImpD3D11Renderer();

		/// 가장 기본적으로 렌더러가 돌아가기 위해 필요한 필수 함수
		// 아직 디퍼드는 고려하지 않았다. 
		virtual bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) override;
		virtual void Update(float deltaTime) override;
		virtual void BeginRender() override;
		virtual void Render() override;
		virtual void EndRender() override;
		virtual void Finalize() override;

		// 창 변환 관련
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
        virtual void RenderRight() override;

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
        virtual void SetWorldEmissionCoefficient(float Emssion) override;
        virtual void SetObjectEmissionCoefficient(size_t objectID, float Emssion) override;

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
        virtual void SetUILayer(size_t uiID, size_t) override;

        virtual void RenderUI() override;

		virtual void SetUINoise(size_t uiID, bool isOn) override;

		virtual void SetUIXRatio(size_t uiID, float ratio) override;

		/// 오브젝트에 딱 맞는 bounding box를 반환 해준다.
        virtual Vector3 GetBoundingBoxLength(std::wstring meshPath, Vector3& offset) override;

		// temp
		virtual void* GetDevice() override;
		virtual void* GetDeviceContext() override;

		/// 상속 받지 않은 렌더러 만 사용하는 함수
		void DrawDebugData();
		void RenderTransparent();
        void RenderTransparentRight();

	private:
		/// window(screen) 관련
		// 윈도우가 여러 개일 때가 있을까
		HWND _hWnd;
		
		UINT _screenWidth;
		UINT _screenHeight;

		// ImpDirectX
		ImpDevice* _device;
		ImpRenderTarget* _renderTarget;
		ImpRenderState* _renderState;

		ImpCamera* _camera;
		ImpCamera* _cameraRight;

		// ImpObject
		QuadObject* _quadLeftObjectForDeferred;
		QuadObject* _quadRightObjectForDeferred;

		BloomQuadObject* _bloomPassQuadObject;

		ForwardOITQuadObject* _forwardOITQuadLeftObject;
		ForwardOITQuadObject* _forwardOITQuadRightObject;

		std::unordered_map<size_t, IObject*> _opaqueObjects;
		std::unordered_map<size_t, IObject*> _transparentObjects;
		std::unordered_map<size_t, BoundingVolumeObject*> _boundingVolumeObjects;
        std::unordered_map<size_t, IUIObject*> _UIObjects;

		IObject* _GridObject;
		IObject* _AxisObject;

		IObject* _testForward;

		/// Light
		std::unordered_map<size_t, LightBase*> _lights;

		/// 엔진 제어를 위한 bool 변수들
		bool _isOnGrid;
		bool _isOnAxis;
		bool _isOnWireFrame;

		//temp
		Factory* _factory;
		ResourceManager* _resourceManager;

		ImpAnimationPlayer* _animationPlayer;

		//temp
		PickingManager* _pickingManager;
        CullingManager* _cullingManager;

		//debug
		ImpFont* _font;
		float _deltaTime;
		size_t pickingObjectNum;

		//temp 
		bool _has2ndCamera;

		//
        float _globalEmissionCoefficient;
	};
}