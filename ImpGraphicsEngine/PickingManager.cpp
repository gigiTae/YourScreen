#include "pch.h"
#include "PickingManager.h"

#include "CollisionPrimitive.h"

// 렌더러에서 포지션을 받아와서, ID
// 여기에서 바운딩 볼륨을 만들어서 저장, 그냥도 저장 
// 매 프레임 검사 
// 없어지면 여기서도 딜리트...

#include "IObject.h"
#include "Resource.h"

#include "ImpCamera.h"

using namespace DirectX;

ImpGraphics::PickingManager::PickingManager(ImpCamera* camera, UINT screenWidth, UINT screenHeight)
	:_camera(camera), _screenWidth(screenWidth), _screenHeight(screenHeight)
{
}

ImpGraphics::PickingManager::~PickingManager()
{
	for (const auto& bV : _boundingVolumes)
	{
		delete bV.second;
	}
	_boundingVolumes.clear();
}

size_t ImpGraphics::PickingManager::GetPickingObject(std::unordered_map<size_t, IObject*> objects, UINT mouseX, UINT mouseY)
{
	// 받은 오브젝트들의 버텍스를 가져와서
	// 스피어를 만들어서 저장하고
	// 확인 한다음에
	// isCollision이 true인 스피어가 있다면
	// 직접 폴리곤마다 확인해서
	// 정확히 피킹

	for (const auto& bV : _boundingVolumes)
	{
		delete bV.second;
	}
	_boundingVolumes.clear();
	_polygonVertexs.clear();
	_polygonIndices.clear();
	_collisionSpheres.clear();

	_collisionSphere = nullptr;
	_isCollision = false;

	_dist = 10000000;

	for (const auto& object : objects)
	{
		if (object.second != nullptr)
		{
            if (object.first < size_t(-100))
            {
                _polygonVertexs[object.first] = objects[object.first]
                                                    ->GetGeometryBuffer()
                                                    ->GetVertexPositions();
                for (UINT i = 0; i < _polygonVertexs[object.first].size(); i++)
                {
                    // DirectX::XMFLOAT3 tempVertex{ (*_polygonVertexs[object.first])[i].x,
                    // (*_polygonVertexs[object.first])[i].y, (*_polygonVertexs[object.first])[i].z };
                    DirectX::XMFLOAT4X4 m;
                    DirectX::XMStoreFloat4x4(
                        &m, objects[object.first]->GetTransformMatrix());

                    ImpMath::Vector3 vertex = _polygonVertexs[object.first][i];
                    DirectX::XMFLOAT3 xmVertex{ vertex.x, vertex.y, vertex.z };
                    DirectX::XMVECTOR xmVec = DirectX::XMLoadFloat3(&xmVertex);

                    xmVec = DirectX::XMVector3Transform(
                        xmVec, objects[object.first]->GetTransformMatrix());
                    DirectX::XMStoreFloat3(&xmVertex, xmVec);

                    vertex = { xmVertex.x, xmVertex.y, xmVertex.z };

                    _polygonVertexs[object.first][i] = vertex;

                    int a = 3;
                }
                _polygonIndices[object.first] = objects[object.first]
                                                    ->GetGeometryBuffer()
                                                    ->GetIndices();
                _boundingVolumes[object.first] = new Sphere{
                    _polygonVertexs[object.first]
                };
            }
		}
	}

	/// 픽킹을 위한 레이 생성
	Ray* ray = new Ray;

	DirectX::XMFLOAT3 viewPos = _camera->GetPosition();
	ray->_origin = DirectX::XMFLOAT4(viewPos.x, viewPos.y, viewPos.z, 1.0f);

	DirectX::XMFLOAT4 rayDir(mouseX, mouseY, 1.0f, 0.0f);

	rayDir.x = (2 * rayDir.x) / _screenWidth - 1;
	rayDir.y = -(2 * rayDir.y) / _screenHeight + 1;

	rayDir.x = rayDir.x / _camera->GetProjTM().m[0][0];
	rayDir.y = rayDir.y / _camera->GetProjTM().m[1][1];

	DirectX::XMFLOAT4X4 viewTM = _camera->GetViewTM();
	DirectX::XMMATRIX viewProjTM = DirectX::XMLoadFloat4x4(&viewTM);
	DirectX::XMMATRIX viewProjInvTM = XMMatrixInverse(nullptr, viewProjTM);

	DirectX::XMMATRIX m = viewProjInvTM;
	DirectX::XMVECTOR endV = DirectX::XMLoadFloat4(&rayDir);

	DirectX::XMVECTOR mul = XMVector4Transform(endV, m);
	mul = DirectX::XMVector4Normalize(mul);
	DirectX::XMStoreFloat4(&rayDir, mul);

	ray->_direction = rayDir;

	/// 레이와 스피어 충돌 검사
	for (const auto& boundingVolume : _boundingVolumes)
	{
		float dist;

		if (IsIntersectRayAndSphere(*boundingVolume.second, *ray, dist))
		{
			if (dist < _dist)
			{
				_sphereNum = boundingVolume.first;
				_collisionSphere = boundingVolume.second;
				_dist = dist;
			}
			_collisionSpheres[boundingVolume.first] = boundingVolume.second;
		}
	}

	_dist = 1000000;
	for (const auto& collisionSphere : _collisionSpheres)
	{
		/// 레이, 폴리곤

		std::vector<UINT>* polygonIndices = _polygonIndices[collisionSphere.first];
		std::vector<ImpMath::Vector3> polygonVertices = _polygonVertexs[collisionSphere.first];

		float dist;

		for (UINT i = 0; i < polygonIndices->size(); i += 3)
		{
			ImpMath::Vector3 tempVec3 = polygonVertices[(*polygonIndices)[i]];
			XMFLOAT3 v0{ tempVec3.x, tempVec3.y, tempVec3.z };
			tempVec3 = polygonVertices[(*polygonIndices)[i + 1]];
			XMFLOAT3 v1{ tempVec3.x, tempVec3.y, tempVec3.z };
			tempVec3 = polygonVertices[(*polygonIndices)[i + 2]];
			XMFLOAT3 v2{ tempVec3.x, tempVec3.y, tempVec3.z };

			if (IsIntersectRayAndTriangle(v0, v1, v2, *ray, dist))
			{
				// temp Todo 거리 제일 짧은 폴리곤 저장
				if (dist < _dist)
				{
					_dist = dist;
					_sphereNum = collisionSphere.first;
				}
				int a = 3;
				_isCollision = true;
			}
		}
	}

	delete ray;

	if (_isCollision)
	{
		return _sphereNum;
	}
	else
	{
		return -1;
	}
}

void ImpGraphics::PickingManager::SetClientSize(UINT screenWidth, UINT screenHeight)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
}

bool ImpGraphics::PickingManager::IsIntersectRayAndSphere(const Sphere& sphere, const Ray& ray, float& dist)
{
	// Direct X 함수
	XMVECTOR Center = XMLoadFloat3(&sphere._center);
	XMVECTOR Radius = XMVectorReplicatePtr(&sphere._radius);

	// l is the vector from the ray origin to the center of the sphere.
	XMVECTOR origin = XMLoadFloat4(&ray._origin);
	XMVECTOR l = Center - origin;

	// s is the projection of the l onto the ray direction.
	XMVECTOR direction = XMLoadFloat4(&ray._direction);
	XMVECTOR s = XMVector3Dot(l, direction);

	XMVECTOR l2 = XMVector3Dot(l, l);

	XMVECTOR r2 = Radius * Radius;

	// m2 is squared distance from the center of the sphere to the projection.
	XMVECTOR m2 = l2 - s * s;

	XMVECTOR NoIntersection;

	// If the ray origin is outside the sphere and the center of the sphere is 
	// behind the ray origin there is no intersection.
	NoIntersection = XMVectorAndInt(XMVectorLess(s, XMVectorZero()), XMVectorGreater(l2, r2));

	// If the squared distance from the center of the sphere to the projection
	// is greater than the radius squared the ray will miss the sphere.
	NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(m2, r2));

	// The ray hits the sphere, compute the nearest intersection point.
	XMVECTOR q = XMVectorSqrt(r2 - m2);
	XMVECTOR t1 = s - q;
	XMVECTOR t2 = s + q;

	XMVECTOR OriginInside = XMVectorLessOrEqual(l2, r2);
	XMVECTOR t = XMVectorSelect(t1, t2, OriginInside);

	if (XMVector4NotEqualInt(NoIntersection, XMVectorTrueInt()))
	{
		// Store the x-component to *pDist.
		XMStoreFloat(&dist, t);
		return TRUE;
	}

	return FALSE;
}

namespace test
{
	XMGLOBALCONST XMVECTORF32 g_RayEpsilon = { { { 1e-20f, 1e-20f, 1e-20f, 1e-20f } } };
	XMGLOBALCONST XMVECTORF32 g_RayNegEpsilon = { { { -1e-20f, -1e-20f, -1e-20f, -1e-20f } } };
}

bool ImpGraphics::PickingManager::IsIntersectRayAndTriangle(DirectX::XMFLOAT3 v0, DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2, const Ray& ray, float& dist)
{
	//assert(DirectX::Internal::XMVector3IsUnit(ray.m_direction));

	XMVECTOR V0 = XMLoadFloat3(&v0);
	XMVECTOR V1 = XMLoadFloat3(&v1);
	XMVECTOR V2 = XMLoadFloat3(&v2);

	XMVECTOR Zero = XMVectorZero();

	XMVECTOR e1 = XMVectorSubtract(V1, V0);
	XMVECTOR e2 = XMVectorSubtract(V2, V0);

	// p = Direction ^ e2;
	XMVECTOR Direction = XMLoadFloat4(&ray._direction);
	XMVECTOR Origin = XMLoadFloat4(&ray._origin);

	XMVECTOR p = XMVector3Cross(Direction, e2);

	// det = e1 * p;
	XMVECTOR det = XMVector3Dot(e1, p);

	XMVECTOR u, v, t;

	if (XMVector3GreaterOrEqual(det, test::g_RayEpsilon))
	{
		// Determinate is positive (front side of the triangle).
		XMVECTOR s = XMVectorSubtract(Origin, V0);

		// u = s * p;
		u = XMVector3Dot(s, p);

		XMVECTOR NoIntersection = XMVectorLess(u, Zero);
		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(u, det));

		// q = s ^ e1;
		XMVECTOR q = XMVector3Cross(s, e1);

		// v = Direction * q;
		v = XMVector3Dot(Direction, q);

		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(v, Zero));
		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(XMVectorAdd(u, v), det));

		// t = e2 * q;
		t = XMVector3Dot(e2, q);

		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(t, Zero));

		if (XMVector4EqualInt(NoIntersection, XMVectorTrueInt()))
		{
			dist = 0.f;
			return false;
		}
	}
	else if (XMVector3LessOrEqual(det, test::g_RayNegEpsilon))
	{
		// Determinate is negative (back side of the triangle).
		XMVECTOR s = XMVectorSubtract(Origin, V0);

		// u = s * p;
		u = XMVector3Dot(s, p);

		XMVECTOR NoIntersection = XMVectorGreater(u, Zero);
		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(u, det));

		// q = s ^ e1;
		XMVECTOR q = XMVector3Cross(s, e1);

		// v = Direction * q;
		v = XMVector3Dot(Direction, q);

		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(v, Zero));
		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(XMVectorAdd(u, v), det));

		// t = e2 * q;
		t = XMVector3Dot(e2, q);

		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(t, Zero));

		if (XMVector4EqualInt(NoIntersection, XMVectorTrueInt()))
		{
			dist = 0.f;
			return false;
		}
	}
	else
	{
		// Parallel ray.
		dist = 0.f;
		return false;
	}

	t = XMVectorDivide(t, det);

	// (u / det) and (v / dev) are the barycentric cooridinates of the intersection.

	// Store the x-component to *pDist
	XMStoreFloat(&dist, t);

	return true;
}

#pragma region Sphere
ImpGraphics::Sphere::Sphere(const std::vector<ImpMath::Vector3>& vertices)
{
	size_t size = vertices.size();

	size_t cnt = vertices.size();
	if (cnt == 0)
	{
		return;
	}

	ImpMath::Vector3 sum = {};
	for (const auto& v : vertices)
	{
		sum += v;
	}

	sum /= (float)cnt;

	_radius = 0;
	// center에서 모든 정점의 거리를 구하고 제일 큰 값을 반지름으로 설정
	for (const auto& v : vertices)
	{
		float temp = (sum - v).LengthSquared();

		if (temp > _radius) _radius = temp;
	}

	_center.x = sum.x;
	_center.y = sum.y;
	_center.z = sum.z;

	_radius = sqrt(_radius);
}

ImpGraphics::Sphere::~Sphere()
{

}
#pragma endregion Sphere