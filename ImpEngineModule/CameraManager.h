#pragma once

namespace ImpEngineModule
{
	class Entity;
	class World;

	/// <summary>
	/// ī�޶� �����ϴ� �Ŵ���
	/// </summary>
		class CameraManager
	{
	public:
		CameraManager();
		~CameraManager();

		// Mainī�޶� �����´�
		void Initialize(World* world);
		
		// �����Լ�
		void Finalize();

		// MainCamera�� ã�´�
		Entity* FindMainCamera();

		// 2��° ī�޶� ��ȯ�Ѵ�
		Entity* FindSecondCamera();

		// ���� ī�޶� ���´�.
		Entity* GetMainCamera();

		// �ι�° ī�޶� ��ȯ
        Entity* GetSecondCamera();

	private:
		Entity* _mainCamera;
        Entity* _secondCamera;

		World* _world;
	};


}