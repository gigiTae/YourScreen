#pragma once

namespace ImpGraphics
{
struct Sphere;
struct Frustum;

class IObject;

class CullingManager
{
public:
    CullingManager();
    ~CullingManager();

    void AddObject(size_t objectNum, IObject* object); // Addobject 시
    bool IsInsideFrustum(size_t objectNum, IObject* object);
    bool IsInsideFrustumRight(size_t objectNum, IObject* object);
    void DeleteObject(size_t objectNum); // DeleteObject 시

    void AddLight(size_t lightNum, float radius);
    bool IsInsideFrustum(size_t lightNum);
    bool IsInsideFrustumRight(size_t lightNum);
    void UpdateLight(size_t lightNum, DirectX::XMFLOAT3 center);
    void DeleteLight(size_t lightNum);

    // Tool 용
    void SetViewFrustum(DirectX::XMMATRIX* proj);
    // 평상시
    void UpdateCameraPosition(DirectX::XMFLOAT3 position,
                              DirectX::XMFLOAT4 rotationQut); // update 카메라시

    void SetViewFrustumRight(DirectX::XMMATRIX* proj);
    void UpdateCameraPositionRight(DirectX::XMFLOAT3 position,
                              DirectX::XMFLOAT4 rotationQut); // update 카메라시

private:
    bool IsIntersectSphereAndFrustum(const Sphere& sphere, const Frustum& frustum) const;

private:
    Frustum* _viewFrustum;
    Frustum* _viewFrustumRight;

    std::unordered_map<size_t, Sphere*> _defaultSpheres; // 일단 트랜스폼 매트릭스를 적용하지 않은 스피어
    std::unordered_map<size_t, Sphere*> _lightSpheres; // 일단 Point만 Spot은 추가하면 넣자
}; 

}