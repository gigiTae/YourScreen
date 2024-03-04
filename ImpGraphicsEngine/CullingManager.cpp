#include "pch.h"
#include "CullingManager.h"

#include "CollisionPrimitive.h"

#include "IObject.h"
#include "Resource.h"

#include "../ImpLib/Inc/SimpleMath.h"

// temp
#include "../ImpLib/Inc/PrimitiveBatch.h"

using namespace ImpGraphics;
using namespace DirectX;

XMGLOBALCONST XMVECTORF32 g_UnitQuaternionEpsilon = {
    { { 1.0e-4f, 1.0e-4f, 1.0e-4f, 1.0e-4f } }
};

bool XMQuaternionIsUnit(_In_ FXMVECTOR Q) noexcept
{
    XMVECTOR Difference = XMVectorSubtract(XMVector4Length(Q), XMVectorSplatOne());
    return XMVector4Less(XMVectorAbs(Difference), g_UnitQuaternionEpsilon);
}

XMVECTOR PointOnLineSegmentNearestPoint(_In_ FXMVECTOR S1,
                                               _In_ FXMVECTOR S2,
                                               _In_ FXMVECTOR P) noexcept
{
    XMVECTOR Dir        = XMVectorSubtract(S2, S1);
    XMVECTOR Projection = XMVectorSubtract(XMVector3Dot(P, Dir),
                                           XMVector3Dot(S1, Dir));
    XMVECTOR LengthSq   = XMVector3Dot(Dir, Dir);

    XMVECTOR t     = XMVectorMultiply(Projection, XMVectorReciprocal(LengthSq));
    XMVECTOR Point = XMVectorMultiplyAdd(t, Dir, S1);

    // t < 0
    XMVECTOR SelectS1 = XMVectorLess(Projection, XMVectorZero());
    Point             = XMVectorSelect(Point, S1, SelectS1);

    // t > 1
    XMVECTOR SelectS2 = XMVectorGreater(Projection, LengthSq);
    Point             = XMVectorSelect(Point, S2, SelectS2);

    return Point;
}


void CullingManager::AddObject(size_t objectNum, IObject* object)
{
    Sphere* tempSphere = new Sphere(object->GetGeometryBuffer()->GetVertexPositions());

    _defaultSpheres[objectNum] = tempSphere;
}

void CullingManager::DeleteObject(size_t objectNum)
{
    Sphere* temp = _defaultSpheres[objectNum];
    if (temp == nullptr)
    {
        MessageBox(NULL,
                   L"삭제하려는 Culling Sphere가 존재하지 않습니다.",
                   L"에러",
                   MB_ICONERROR);
        return;
    }

    delete temp;
    _defaultSpheres.erase(objectNum);
}

bool CullingManager::IsInsideFrustum(size_t objectNum, IObject* object)
{
    if (objectNum > -100)
    {
        return true;
    }
    else
    {
        Sphere tempTransformSphere(*_defaultSpheres[objectNum]);

        // 센터에 트랜스폼 곱하기s
        XMVECTOR positionVec = XMLoadFloat3(&_defaultSpheres[objectNum]->_center);
        positionVec = XMVector3Transform(positionVec, object->GetTransformMatrix());
        // 매트릭스 곱한 센터로 변경
        XMStoreFloat3(&tempTransformSphere._center, positionVec);

        float maxScale = 0;
        DirectX::XMFLOAT4X4 objectTransform;
        DirectX::XMStoreFloat4x4(&objectTransform, object->GetTransformMatrix());

        DirectX::SimpleMath::Matrix tempTransform = DirectX::SimpleMath::Matrix(&objectTransform._11);

        DirectX::SimpleMath::Vector3 scale;
        DirectX::SimpleMath::Quaternion rotation;
        DirectX::SimpleMath::Vector3 translation;

        tempTransform.Decompose(scale, rotation, translation);

        if (std::abs(scale.x) > maxScale)
        {
            maxScale = std::abs(scale.x);
        }
        if (std::abs(scale.y) > maxScale)
        {
            maxScale = std::abs(scale.y);
        }
        if (std::abs(scale.z) > maxScale)
        {
            maxScale = std::abs(scale.z);
        }

        tempTransformSphere._radius = tempTransformSphere._radius * maxScale;

        return IsIntersectSphereAndFrustum(tempTransformSphere, *_viewFrustum);
    }
}


CullingManager::~CullingManager()
{
    delete _viewFrustum;
    delete _viewFrustumRight;

    for (const auto& sphere : _defaultSpheres)
    {
        delete sphere.second;
    }
    for (const auto& light : _lightSpheres)
    {
        delete light.second;
    }
}


CullingManager::CullingManager()
{
    _viewFrustum = new Frustum();
    _viewFrustumRight = new Frustum();
}

void CullingManager::SetViewFrustum(DirectX::XMMATRIX* proj)
{
    _viewFrustum->SetFrustum(proj);
}


bool CullingManager::IsInsideFrustumRight(size_t objectNum, IObject* object)
{
    if (objectNum > -100)
    {
        return true;
    }
    else
    {
        Sphere tempTransformSphere(*_defaultSpheres[objectNum]);

        // 센터에 트랜스폼 곱하기
        XMVECTOR positionVec = XMLoadFloat3(&_defaultSpheres[objectNum]->_center);
        positionVec = XMVector3Transform(positionVec, object->GetTransformMatrix());
        // 매트릭스 곱한 센터로 변경
        XMStoreFloat3(&tempTransformSphere._center, positionVec);

        float maxScale = 0;
        DirectX::XMFLOAT4X4 objectTransform;
        DirectX::XMStoreFloat4x4(&objectTransform, object->GetTransformMatrix());

        DirectX::SimpleMath::Matrix tempTransform = DirectX::SimpleMath::Matrix(&objectTransform._11);

        DirectX::SimpleMath::Vector3 scale;
        DirectX::SimpleMath::Quaternion rotation;
        DirectX::SimpleMath::Vector3 translation;

        tempTransform.Decompose(scale, rotation, translation);

        if (std::abs(scale.x) > maxScale)
        {
            maxScale = std::abs(scale.x);
        }
        if (std::abs(scale.y) > maxScale)
        {
            maxScale = std::abs(scale.y);
        }
        if (std::abs(scale.z) > maxScale)
        {
            maxScale = std::abs(scale.z);
        }

        tempTransformSphere._radius = tempTransformSphere._radius * maxScale;

        return IsIntersectSphereAndFrustum(tempTransformSphere, *_viewFrustumRight);
    }
}

void ImpGraphics::CullingManager::AddLight(size_t objectNum, float radius)
{
    Sphere* sphere           = new Sphere({0.0f, 0.0f, 0.0f}, radius);
    _lightSpheres[objectNum] = sphere;
}

bool CullingManager::IsInsideFrustum(size_t lightNum)
{
    Sphere tempSphere  = *_lightSpheres[lightNum];
    
    return IsIntersectSphereAndFrustum(tempSphere, *_viewFrustum);
}

bool CullingManager::IsInsideFrustumRight(size_t lightNum)
{
    Sphere tempSphere  = *_lightSpheres[lightNum];

    return IsIntersectSphereAndFrustum(tempSphere, *_viewFrustumRight);
}

void CullingManager::UpdateLight(size_t lightNum, DirectX::XMFLOAT3 center)
{
    _lightSpheres[lightNum]->_center = center;
}

void ImpGraphics::CullingManager::DeleteLight(size_t objectNum)
{
    Sphere* temp = _lightSpheres[objectNum];
    if (temp == nullptr)
    {
        MessageBox(NULL,
                   L"삭제하려는 Culling Light가 존재하지 않습니다.",
                   L"에러",
                   MB_ICONERROR);
        return;
    }

    delete temp;
    _lightSpheres.erase(objectNum);
}

void ImpGraphics::CullingManager::UpdateCameraPosition(DirectX::XMFLOAT3 position,
                                                        DirectX::XMFLOAT4 rotationQut)
 {
    _viewFrustum->_origin = position;
    _viewFrustum->_orientation = rotationQut;
 }


void CullingManager::SetViewFrustumRight(DirectX::XMMATRIX* proj)
{
    _viewFrustumRight->SetFrustum(proj);
}

 void CullingManager::UpdateCameraPositionRight(DirectX::XMFLOAT3 position,
                                                DirectX::XMFLOAT4 rotationQut)
 {
    _viewFrustumRight->_origin = position;
    _viewFrustumRight->_orientation = rotationQut;
 }

 bool ImpGraphics::CullingManager::IsIntersectSphereAndFrustum(
    const Sphere& sphere,
    const Frustum& frustum) const
{
    XMVECTOR Zero = XMVectorZero();

    // Build the frustum planes.
    XMVECTOR Planes[6];
    Planes[0] = XMVectorSet(0.0f, 0.0f, -1.0f, frustum._near);
    Planes[1] = XMVectorSet(0.0f, 0.0f, 1.0f, -frustum._far);
    Planes[2] = XMVectorSet(1.0f, 0.0f, -frustum._rightSlope, 0.0f);
    Planes[3] = XMVectorSet(-1.0f, 0.0f, frustum._leftSlope, 0.0f);
    Planes[4] = XMVectorSet(0.0f, 1.0f, -frustum._topSlope, 0.0f);
    Planes[5] = XMVectorSet(0.0f, -1.0f, frustum._bottomSlope, 0.0f);

    // Normalize the planes so we can compare to the sphere radius.
    Planes[2] = XMVector3Normalize(Planes[2]);
    Planes[3] = XMVector3Normalize(Planes[3]);
    Planes[4] = XMVector3Normalize(Planes[4]);
    Planes[5] = XMVector3Normalize(Planes[5]);

    // Load origin and orientation of the frustum.
    XMVECTOR vOrigin      = XMLoadFloat3(&frustum._origin);
    XMVECTOR vOrientation = XMLoadFloat4(&frustum._orientation);

    assert(XMQuaternionIsUnit(vOrientation));

    // Load the sphere.
    XMVECTOR vCenter = XMLoadFloat3(&sphere._center);
    XMVECTOR vRadius = XMVectorReplicatePtr(&sphere._radius);

    // Transform the center of the sphere into the local space of frustum.
    vCenter = XMVector3InverseRotate(XMVectorSubtract(vCenter, vOrigin),
                                     vOrientation);

    // Set w of the center to one so we can dot4 with the plane.
    vCenter = XMVectorInsert<0, 0, 0, 0, 1>(vCenter, XMVectorSplatOne());

    // Check against each plane of the frustum.
    XMVECTOR Outside         = XMVectorFalseInt();
    XMVECTOR InsideAll       = XMVectorTrueInt();
    XMVECTOR CenterInsideAll = XMVectorTrueInt();

    XMVECTOR Dist[6];

    for (size_t i = 0; i < 6; ++i)
    {
        Dist[i] = XMVector4Dot(vCenter, Planes[i]);

        // Outside the plane?
        Outside = XMVectorOrInt(Outside, XMVectorGreater(Dist[i], vRadius));

        // Fully inside the plane?
        InsideAll = XMVectorAndInt(InsideAll,
                                   XMVectorLessOrEqual(Dist[i],
                                                       XMVectorNegate(vRadius)));

        // Check if the center is inside the plane.
        CenterInsideAll = XMVectorAndInt(CenterInsideAll,
                                         XMVectorLessOrEqual(Dist[i], Zero));
    }

    // If the sphere is outside any of the planes it is outside.
    if (XMVector4EqualInt(Outside, XMVectorTrueInt()))
        return false;

    // If the sphere is inside all planes it is fully inside.
    if (XMVector4EqualInt(InsideAll, XMVectorTrueInt()))
        return true;

    // If the center of the sphere is inside all planes and the sphere
    // intersects one or more planes then it must intersect.
    if (XMVector4EqualInt(CenterInsideAll, XMVectorTrueInt()))
        return true;

    // The sphere may be outside the frustum or intersecting the frustum.
    // Find the nearest feature (face, edge, or corner) on the frustum
    // to the sphere.

    // The faces adjacent to each face are:
    static const size_t adjacent_faces[6][4] = {
        { 2, 3, 4, 5 }, // 0
        { 2, 3, 4, 5 }, // 1
        { 0, 1, 4, 5 }, // 2
        { 0, 1, 4, 5 }, // 3
        { 0, 1, 2, 3 }, // 4
        { 0, 1, 2, 3 }
    }; // 5

    XMVECTOR Intersects = XMVectorFalseInt();

    // Check to see if the nearest feature is one of the planes.
    for (size_t i = 0; i < 6; ++i)
    {
        // Find the nearest point on the plane to the center of the sphere.
        XMVECTOR Point = XMVectorNegativeMultiplySubtract(Planes[i], Dist[i], vCenter);

        // Set w of the point to one.
        Point = XMVectorInsert<0, 0, 0, 0, 1>(Point, XMVectorSplatOne());

        // If the point is inside the face (inside the adjacent planes) then
        // this plane is the nearest feature.
        XMVECTOR InsideFace = XMVectorTrueInt();

        for (size_t j = 0; j < 4; j++)
        {
            size_t plane_index = adjacent_faces[i][j];

            InsideFace = XMVectorAndInt(InsideFace,
                                        XMVectorLessOrEqual(XMVector4Dot(Point,
                                                                         Planes[plane_index]),
                                                            Zero));
        }

        // Since we have already checked distance from the plane we know that
        // the sphere must intersect if this plane is the nearest feature.
        Intersects = XMVectorOrInt(Intersects,
                                   XMVectorAndInt(XMVectorGreater(Dist[i], Zero),
                                                  InsideFace));
    }

    if (XMVector4EqualInt(Intersects, XMVectorTrueInt()))
        return true;

    // Build the corners of the frustum.
    XMVECTOR vRightTop = XMVectorSet(frustum._rightSlope, frustum._topSlope, 1.0f, 0.0f);
    XMVECTOR vRightBottom = XMVectorSet(frustum._rightSlope, frustum._bottomSlope, 1.0f, 0.0f);
    XMVECTOR vLeftTop = XMVectorSet(frustum._leftSlope, frustum._topSlope, 1.0f, 0.0f);
    XMVECTOR vLeftBottom = XMVectorSet(frustum._leftSlope, frustum._bottomSlope, 1.0f, 0.0f);
    XMVECTOR vNear = XMVectorReplicatePtr(&frustum._near);
    XMVECTOR vFar  = XMVectorReplicatePtr(&frustum._far);

    XMVECTOR Corners[frustum.CORNER_COUNT];
    Corners[0] = XMVectorMultiply(vRightTop, vNear);
    Corners[1] = XMVectorMultiply(vRightBottom, vNear);
    Corners[2] = XMVectorMultiply(vLeftTop, vNear);
    Corners[3] = XMVectorMultiply(vLeftBottom, vNear);
    Corners[4] = XMVectorMultiply(vRightTop, vFar);
    Corners[5] = XMVectorMultiply(vRightBottom, vFar);
    Corners[6] = XMVectorMultiply(vLeftTop, vFar);
    Corners[7] = XMVectorMultiply(vLeftBottom, vFar);

    // The Edges are:
    static const size_t edges[12][2] = {
        { 0, 1 }, { 2, 3 }, { 0, 2 }, { 1, 3 }, // Near plane
        { 4, 5 }, { 6, 7 }, { 4, 6 }, { 5, 7 }, // Far plane
        { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 },
    }; // Near to far

    XMVECTOR RadiusSq = XMVectorMultiply(vRadius, vRadius);

    // Check to see if the nearest feature is one of the edges (or corners).
    for (size_t i = 0; i < 12; ++i)
    {
        size_t ei0 = edges[i][0];
        size_t ei1 = edges[i][1];

        // Find the nearest point on the edge to the center of the sphere.
        // The corners of the frustum are included as the endpoints of the edges.
        XMVECTOR
        Point = PointOnLineSegmentNearestPoint(Corners[ei0],
                                                                  Corners[ei1],
                                                                  vCenter);

        XMVECTOR Delta = XMVectorSubtract(vCenter, Point);

        XMVECTOR DistSq = XMVector3Dot(Delta, Delta);

        // If the distance to the center of the sphere to the point is less than
        // the radius of the sphere then it must intersect.
        Intersects = XMVectorOrInt(Intersects,
                                   XMVectorLessOrEqual(DistSq, RadiusSq));
    }

    if (XMVector4EqualInt(Intersects, XMVectorTrueInt()))
        return true;

    // The sphere must be outside the frustum.
    return false;
}