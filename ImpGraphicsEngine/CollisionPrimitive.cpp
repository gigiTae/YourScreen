#include "pch.h"
#include "CollisionPrimitive.h"

using namespace ImpGraphics;
using namespace DirectX;


void Frustum::SetFrustum(DirectX::XMMATRIX* pProjection)
{ 
    // Corners of the projection frustum in homogenous space.
    static XMVECTOR HomogenousPoints[6] = {
        { 1.0f, 0.0f, 1.0f, 1.0f },  // right (at far plane)
        { -1.0f, 0.0f, 1.0f, 1.0f }, // left
        { 0.0f, 1.0f, 1.0f, 1.0f },  // top
        { 0.0f, -1.0f, 1.0f, 1.0f }, // bottom

        { 0.0f, 0.0f, 0.0f, 1.0f }, // near
        { 0.0f, 0.0f, 1.0f, 1.0f }  // far
    };

    XMVECTOR Determinant;
    XMMATRIX matInverse = XMMatrixInverse(&Determinant, *pProjection);

    // Compute the frustum corners in world space.
    XMVECTOR Points[6];

    for (INT i = 0; i < 6; i++)
    {
        // Transform point.
        Points[i] = XMVector4Transform(HomogenousPoints[i], matInverse);
    }

    _origin      = XMFLOAT3(0.0f, 0.0f, 0.0f);
    _orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

    // Compute the slopes.
    Points[0] = Points[0] * XMVectorReciprocal(XMVectorSplatZ(Points[0]));
    Points[1] = Points[1] * XMVectorReciprocal(XMVectorSplatZ(Points[1]));
    Points[2] = Points[2] * XMVectorReciprocal(XMVectorSplatZ(Points[2]));
    Points[3] = Points[3] * XMVectorReciprocal(XMVectorSplatZ(Points[3]));

    _rightSlope  = XMVectorGetX(Points[0]);
    _leftSlope   = XMVectorGetX(Points[1]);
    _topSlope    = XMVectorGetY(Points[2]);
    _bottomSlope = XMVectorGetY(Points[3]);

    // Compute near and far.
    Points[4] = Points[4] * XMVectorReciprocal(XMVectorSplatW(Points[4]));
    Points[5] = Points[5] * XMVectorReciprocal(XMVectorSplatW(Points[5]));

    _near = XMVectorGetZ(Points[4]);
    _far  = XMVectorGetZ(Points[5]);

    return;
}

Frustum::Frustum()
{
}

Sphere::Sphere(const Sphere& sphere)
{
    _center = sphere._center;
    _radius = sphere._radius;
}

Sphere::Sphere(DirectX::XMFLOAT3 center, float radius)
{
    _center = center;
    _radius = radius;
}

Box::Box(const std::vector<ImpMath::Vector3>& vertices)
    : _min(ImpMath::Vector3{ 1000.0f, 1000.0f, 1000.0f })
    , _max(ImpMath::Vector3{ -1000.0f, -1000.0f, -1000.0f })
{
    for (const auto& vertex : vertices)
    {
        UpdateMinMax(vertex);
    }

    _center  = (_max + _min) / 2;
    _extents = (_max - _min) / 2;
}

void Box::UpdateMinMax(const ImpMath::Vector3& other)
{
    _min.x = min(_min.x, other.x);
    _min.y = min(_min.y, other.y);
    _min.z = min(_min.z, other.z);

    _max.x = max(_max.x, other.x);
    _max.y = max(_max.y, other.y);
    _max.z = max(_max.z, other.z);
}

static inline BOOL XMVector3AnyTrue(FXMVECTOR V)
{
    XMVECTOR C;

    // Duplicate the fourth element from the first element.
    C = XMVectorSwizzle(V, 0, 1, 2, 0);

    return XMComparisonAnyTrue(XMVector4EqualIntR(C, XMVectorTrueInt()));
}

bool Box::IsIntersectRayAndBox(const Box& box, const Ray& ray)
{
    static const XMVECTOR Epsilon = { 1e-20f, 1e-20f, 1e-20f, 1e-20f };
    static const XMVECTOR FltMin  = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };
    static const XMVECTOR FltMax  = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };

    // Load the box.
    XMFLOAT3 center;
    center.x         = box._center.x;
    center.y         = box._center.y;
    center.z         = box._center.z;
    XMFLOAT3 extents;
    extents.x = box._extents.x;
    extents.y = box._extents.y;
    extents.z = box._extents.z;

    XMVECTOR Center  = XMLoadFloat3(&center);
    XMVECTOR Extents = XMLoadFloat3(&extents);

    // Adjust ray origin to be relative to center of the box.
    XMVECTOR origin  = XMLoadFloat4(&ray._origin);
    XMVECTOR TOrigin = Center - origin;

    // Compute the dot product againt each axis of the box.
    // Since the axii are (1,0,0), (0,1,0), (0,0,1) no computation is necessary.
    XMVECTOR AxisDotOrigin    = TOrigin;
    XMVECTOR AxisDotDirection = XMLoadFloat4(&ray._direction);

    // if (fabs(AxisDotDirection) <= Epsilon) the ray is nearly parallel to the slab.
    XMVECTOR IsParallel = XMVectorLessOrEqual(XMVectorAbs(AxisDotDirection), Epsilon);

    // Test against all three axii simultaneously.
    XMVECTOR InverseAxisDotDirection = XMVectorReciprocal(AxisDotDirection);
    XMVECTOR t1 = (AxisDotOrigin - Extents) * InverseAxisDotDirection;
    XMVECTOR t2 = (AxisDotOrigin + Extents) * InverseAxisDotDirection;

    // Compute the max of min(t1,t2) and the min of max(t1,t2) ensuring we don't
    // use the results from any directions parallel to the slab.
    XMVECTOR t_min = XMVectorSelect(XMVectorMin(t1, t2), FltMin, IsParallel);
    XMVECTOR t_max = XMVectorSelect(XMVectorMax(t1, t2), FltMax, IsParallel);

    // t_min.x = maximum( t_min.x, t_min.y, t_min.z );
    // t_max.x = minimum( t_max.x, t_max.y, t_max.z );
    t_min = XMVectorMax(t_min, XMVectorSplatY(t_min)); // x = max(x,y)
    t_min = XMVectorMax(t_min, XMVectorSplatZ(t_min)); // x = max(max(x,y),z)
    t_max = XMVectorMin(t_max, XMVectorSplatY(t_max)); // x = min(x,y)
    t_max = XMVectorMin(t_max, XMVectorSplatZ(t_max)); // x = min(min(x,y),z)

    // if ( t_min > t_max ) return FALSE;
    XMVECTOR NoIntersection = XMVectorGreater(XMVectorSplatX(t_min),
                                              XMVectorSplatX(t_max));

    // if ( t_max < 0.0f ) return FALSE;
    NoIntersection = XMVectorOrInt(NoIntersection,
                                   XMVectorLess(XMVectorSplatX(t_max),
                                                XMVectorZero()));

    // if (IsParallel && (-Extents > AxisDotOrigin || Extents < AxisDotOrigin)) return FALSE;
    XMVECTOR ParallelOverlap = XMVectorInBounds(AxisDotOrigin, Extents);
    NoIntersection           = XMVectorOrInt(NoIntersection,
                                   XMVectorAndCInt(IsParallel, ParallelOverlap));

    if (!XMVector3AnyTrue(NoIntersection))
    {
        // Store the x-component to *pDist
        return TRUE;
    }

    return FALSE;
}