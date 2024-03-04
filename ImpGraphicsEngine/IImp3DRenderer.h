#pragma once

#define WIN32_LEAN_AND_MEAN

#include "IImpGraphicsEngine.h"

/// <summary>
/// Graphics Engine �� Game Engine���� ������ �� �� ����� h�� ���� �̾ƾ� �ڴ�. �ƴѰ� ������ ������ ����� ū �� ���⵵?
/// ���, ���� �ʿ������� �� �� ����� �غ��߰ڴ�.
/// ������ Scene ���� ������ �������� �ʿ��� ��, ���۵� �������� �ʿ��� �� �ٸ� �� �ְڴ�. 
/// �׷� �Ѵ� �����ϴ� �� �´� �����̰���?
/// </summary>
namespace ImpGraphics
{
	/// <summary>
	/// ������ �������̽�
	/// 
	/// �ϴ��� �� �𸣰ھ �������� I3DRenderer.h�� �����ߴ�!
	/// 
	/// 2023.12.12 kimyujin
	/// </summary>
	class IImp3DRenderer abstract
	{
	public:
		IImp3DRenderer() {};
		virtual ~IImp3DRenderer() {};

		/// ���� �⺻������ �������� ���ư��� ���� �ʿ��� �ʼ� �Լ�
		// ���� ���۵�� ������� �ʾҴ�. 
		virtual bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) abstract;
		virtual void Update(float deltaTime) abstract;
		virtual void BeginRender() abstract; // ���� �������� BeginRender, Render, EndRender �� �����ؼ� ȣ���ؾ� �� ������ ������?
		virtual void Render() abstract;
		virtual void EndRender() abstract;
		virtual void Finalize() abstract;

		// â ��ȯ ����
		virtual void SetClientSize(int width, int height) abstract;

		/// �׸��� ���� �ʿ��� ������Ʈ��
        // ī�޶� ����
        virtual void UpdateCamera(Matrix transformMatrix) abstract;
        virtual void SetCamera(CameraInfo cameraInfo) abstract;
        // ȭ�� ���� ��
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

		/// �浹�� ���� BoundingVolume ������Ʈ �߰�
		virtual void AddCubeBoundingVolume(size_t _boundingVolumeID, /*Vector3 center,*/ Vector3 length) abstract;
		virtual void AddSphereBoundingVolume(size_t _boundingVolumeID, /*Vector3 center,*/ float radius) abstract;
		virtual void UpdateBoundingVolume(size_t boundingVolumeID, Matrix transformMatrix, Vector4 color) abstract;
		virtual void DeleteBoundingVolume(size_t boundingVolumeID) abstract;

		/// Picking!
		virtual size_t GetPickingObject(long mouseX, long mouseY) abstract;

		/// �׷��Ƚ� ���� ����
		virtual void SetGrid(bool isOn) abstract;
		virtual void SetAxis(bool isOn) abstract;
		virtual void SetWireFrame(bool isOn) abstract;
		// ��� ����
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

		/// ������Ʈ�� �� �´� bounding box�� ��ȯ ���ش�.
        virtual Vector3 GetBoundingBoxLength(std::wstring meshPath, Vector3& offset) abstract; 

		/// IMGUI
		virtual void* GetDevice() abstract;
		virtual void* GetDeviceContext() abstract;
	};
}