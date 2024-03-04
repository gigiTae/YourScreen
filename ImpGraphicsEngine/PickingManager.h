#pragma once
#include <unordered_map>

#include "../ImpStructure/ImpMath.h"

namespace ImpGraphics
{
	class ImpCamera;
	class IObject;

	struct Ray;
    struct Sphere;

	class PickingManager
	{
	public:
		PickingManager(ImpCamera* camera, UINT screenWidth, UINT screenHeight);
		~PickingManager();

		size_t GetPickingObject(std::unordered_map<size_t, IObject*> objects, UINT mouseX, UINT mouseY);
		void SetClientSize(UINT screenWidth, UINT screenHeight);
		//void Initialize();
		//void Update();
		//void Render(); // 아마도 디버깅 용이 되지 않을까?
		//void Finalize();

		bool IsIntersectRayAndSphere(const Sphere& sphere, const Ray& ray, float& dist); // 레이와 충돌 했는가
		bool IsIntersectRayAndTriangle(DirectX::XMFLOAT3 v0, DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2, const Ray& ray, float& dist);

	private:
		UINT _screenWidth;
		UINT _screenHeight;

		ImpCamera* _camera;

		std::unordered_map<size_t, Sphere*> _boundingVolumes; // 한 개는 바운딩 볼륨
		std::unordered_map<size_t, std::vector<ImpMath::Vector3>> _polygonVertexs; // 한 개는 폴리곤
		std::unordered_map<size_t, std::vector<UINT>*> _polygonIndices; // 한 개는 폴리곤

		Sphere* _collisionSphere;
		UINT _sphereNum;
		float _dist = 1000000;
		bool _isCollision = false;

		std::unordered_map<size_t, Sphere*> _collisionSpheres;

		DirectX::XMFLOAT3 _collisionV0;
		DirectX::XMFLOAT3 _collisionV1;
		DirectX::XMFLOAT3 _collisionV2;
	};
}