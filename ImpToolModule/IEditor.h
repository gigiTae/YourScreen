#pragma once

#include <d3d11.h>

namespace IImpGraphicsEngine { class IImpGraphicsEngine; }

namespace ImpToolModule
{
	enum class ToolMode
	{
		EditorMode, // 편집 모드
		GamePlayMode, // 게임 플레이 모드
		PauseMode, // 정지모드
	};

	/// <summary>
	/// Editor Interface
	/// ImGui , dx11 사용
	/// </summary>
	class IEditor
	{
	public:
		virtual ~IEditor(){}

		/// <summary>
		/// Editor 초기화, dx11,winAPI,imgui를 사용한다.
		/// </summary>
		virtual void Initialize(HWND hWnd, void* device, void* deviceContext,
			ImpEngineModule::EngineModule* engineModule, ImpGraphics::IImpGraphicsEngine* grahicsEngnie) abstract;

		/// <summary>
		/// 종료 함수
		/// </summary>
		virtual void Finalize() abstract;

		/// <summary>
		/// GUI정보를 Editor에 전달한다.
		/// </summary>
		virtual void Update()abstract;

		/// <summary>
		/// 새로운 프레임의 갱신을 imgui에게 전달한다.
		/// </summary>
		virtual void NewFrame() abstract;

		/// <summary>
		/// GUI를 그린다.
		/// </summary>
		virtual void EndRender() abstract;

		/// Tool용 카메라를 반환한다.
		virtual ImpEngineModule::Entity* GetToolCamera()const abstract;

		/// 이번프레임에 모드가 변경되었는지
		virtual bool IsChangeMode()abstract;

		/// 현재 모드를 변경한다.
		virtual void ChangeMode(ToolMode mode)abstract;

		/// 현재 툴의 모드를 가져온다.
		virtual ToolMode GetMode()const abstract;
	};


}