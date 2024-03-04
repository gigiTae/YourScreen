#pragma once

namespace ImpEngineModule
{
	class Entity;
	class World;

	/// <summary>
	/// 카메라를 관리하는 매니져
	/// </summary>
		class CameraManager
	{
	public:
		CameraManager();
		~CameraManager();

		// Main카메라를 가져온다
		void Initialize(World* world);
		
		// 종료함수
		void Finalize();

		// MainCamera를 찾는다
		Entity* FindMainCamera();

		// 2번째 카메라를 반환한다
		Entity* FindSecondCamera();

		// 메인 카메라를 얻어온다.
		Entity* GetMainCamera();

		// 두번째 카메라 반환
        Entity* GetSecondCamera();

	private:
		Entity* _mainCamera;
        Entity* _secondCamera;

		World* _world;
	};


}