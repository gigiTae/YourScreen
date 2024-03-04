#pragma once

#include "../ImpStructure/ImpMath.h"

namespace ImpGraphics
{
    struct Ray
    {
    public:
        DirectX::XMFLOAT4 _origin;
        DirectX::XMFLOAT4 _direction;
    };

    struct Sphere
    {
    public:
        // Sphere(ImpMath::Vector3 center = ImpMath::Vector3::Zero, float radius = 0.f);
        Sphere(const std::vector<ImpMath::Vector3>& vertices);
        Sphere(DirectX::XMFLOAT3 center, float radius);
        Sphere(const Sphere& sphere);
        ~Sphere();
        // bool IsInside(const ImpMath::Vector3& point) const; // 특정 점이 안에 있는가

    public:
        DirectX::XMFLOAT3 _center;
        float _radius;
    };

    struct Box
    {
    public:
        Box(const std::vector<ImpMath::Vector3>& vertices);

        void UpdateMinMax(const ImpMath::Vector3& other);

        static bool IsIntersectRayAndBox(const Box& box, const Ray& ray);

    public:
        ImpMath::Vector3 _center;
        ImpMath::Vector3 _extents;

        ImpMath::Vector3 _min;
        ImpMath::Vector3 _max;
    };

    // 일단 view Frustum 만 지원할랭
    struct Frustum
    {
    public:
        void SetFrustum(DirectX::XMMATRIX* pProjection);
        Frustum();

    public:
        static constexpr size_t CORNER_COUNT = 8;

        DirectX::XMFLOAT3 _origin;      // Origin of the frustum (and projection).
        DirectX::XMFLOAT4 _orientation; // Quaternion representing rotation.

        float _rightSlope;  // Positive X (X/Z)
        float _leftSlope;   // Negative X
        float _topSlope;    // Positive Y (Y/Z)
        float _bottomSlope; // Negative Y
        float _near;
        float _far;   // Z of the near plane and far plane.
    };
}