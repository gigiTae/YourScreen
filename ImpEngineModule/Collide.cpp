#include "EngineModulePCH.h"
#include "Collide.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"

ImpEngineModule::Physics::Collision ImpEngineModule::Physics::Collide(
    BoxCollider* lfm,
    BoxCollider* rfm)
{
    using namespace ImpMath;

    Collision collision;
    collision.lfm = lfm;
    collision.rfm = rfm;

    // AABB
    if (!lfm->IsRotatable() && !rfm->IsRotatable())
    {
        Vector3 lfmMin;
        Vector3 lfmMax;
        lfm->GetMinMaxPoint(lfmMin, lfmMax);

        Vector3 rfmMin, rfmMax;
        rfm->GetMinMaxPoint(rfmMin, rfmMax);

        // AABB 충돌
        if (lfmMin.x <= rfmMax.x && lfmMax.x >= rfmMin.x && lfmMin.y <= rfmMax.y &&
            lfmMax.y >= rfmMin.y && lfmMin.z <= rfmMax.z && lfmMax.z >= rfmMin.z)
        {
            collision.isCollide = true;
        }
        else
        {
            collision.isCollide = false;
            return collision;
        }

        float overlapX = std::min(lfmMax.x, rfmMax.x) -
                         std::max(lfmMin.x, rfmMin.x);
        float overlapY = std::min(lfmMax.y, rfmMax.y) -
                         std::max(lfmMin.y, rfmMin.y);
        float overlapZ = std::min(lfmMax.z, rfmMax.z) -
                         std::max(lfmMin.z, rfmMin.z);

        if (overlapX < overlapY && overlapX < overlapZ)
        {
            collision.intersectionLength = overlapX;
            collision.separationNormal   = Vector3(
                (lfmMin.x < rfmMin.x) ? -1.0f : 1.0f, 0.0f, 0.0f);
        }
        else if (overlapY < overlapZ)
        {
            collision.intersectionLength = overlapY;
            collision.separationNormal   = Vector3(
                0.0f, (lfmMin.y < rfmMin.y) ? -1.0f : 1.0f, 0.0f);
        }
        else
        {
            collision.intersectionLength = overlapZ;
            collision.separationNormal   = Vector3(
                0.0f, 0.0f, (lfmMin.z < rfmMin.z) ? -1.0f : 1.0f);
        }

        return collision;
    }

    // OBB
    Vector3 lfmCenter = lfm->GetCenterPosition();
    Vector3 lfmSize   = lfm->GetSize() * 0.5f;
    Matrix lfmRotM    = Matrix::Identity();
    if (lfm->IsRotatable())
    {
        lfmRotM = Matrix::MakeFromQuaternion(lfm->GetRotation());
    }

    Vector3 lfmXAxis(lfmRotM._11, lfmRotM._12, lfmRotM._13);
    Vector3 lfmYAxis(lfmRotM._21, lfmRotM._22, lfmRotM._23);
    Vector3 lfmZAxis(lfmRotM._31, lfmRotM._32, lfmRotM._33);

    Vector3 rfmCenter = rfm->GetCenterPosition();
    Vector3 rfmSize   = rfm->GetSize() * 0.5f;
    Matrix rfmRotM    = Matrix::Identity();
    if (rfm->IsRotatable())
    {
        rfmRotM = Matrix::MakeFromQuaternion(rfm->GetRotation());
    }
    Vector3 rfmXAxis(rfmRotM._11, rfmRotM._12, rfmRotM._13);
    Vector3 rfmYAxis(rfmRotM._21, rfmRotM._22, rfmRotM._23);
    Vector3 rfmZAxis(rfmRotM._31, rfmRotM._32, rfmRotM._33);

    Vector3 D = rfmCenter - lfmCenter;

    Vector3 C[3];
    Vector3 absC[3];
    Vector3 AD;
    float R0, R1, R;
    float R01;

    float minDistance = 0.F;
    Vector3 normal;

    // A0 (lfm xAxis)
    C[0].x    = lfmXAxis.Dot(rfmXAxis);
    C[0].y    = lfmXAxis.Dot(rfmYAxis);
    C[0].z    = lfmXAxis.Dot(rfmZAxis);
    AD.x      = lfmXAxis.Dot(D);
    absC[0].x = std::abs(C[0].x);
    absC[0].y = std::abs(C[0].y);
    absC[0].z = std::abs(C[0].z);
    R         = std::abs(AD.x);
    R1  = rfmSize.x * absC[0].x + rfmSize.y * absC[0].y + rfmSize.z * absC[0].z;
    R01 = lfmSize.x + R1;

    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }
    // 분리축 계산
    minDistance = R01 - R;
    normal      = -lfmXAxis;

    if (AD.x < 0)
    {
        normal *= -1;
    }

    // A1 (lfm yAixs)
    C[1].x    = lfmYAxis.Dot(rfmXAxis);
    C[1].y    = lfmYAxis.Dot(rfmYAxis);
    C[1].z    = lfmYAxis.Dot(rfmZAxis);
    AD.y      = lfmYAxis.Dot(D);
    absC[1].x = std::abs(C[1].x);
    absC[1].y = std::abs(C[1].y);
    absC[1].z = std::abs(C[1].z);
    R         = std::abs(AD.y);
    R1  = rfmSize.x * absC[1].x + rfmSize.y * absC[1].y + rfmSize.z * absC[1].z;
    R01 = lfmSize.y + R1;

    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    if (minDistance > R01 - R)
    {
        minDistance = R01 - R;
        normal      = -lfmYAxis;
        if (AD.y < 0)
        {
            normal *= -1;
        }
    }

    // A2 (lfm zAxis)
    C[2].x    = lfmZAxis.Dot(rfmXAxis);
    C[2].y    = lfmZAxis.Dot(rfmYAxis);
    C[2].z    = lfmZAxis.Dot(rfmZAxis);
    AD.z      = lfmZAxis.Dot(D);
    absC[2].x = std::abs(C[2].x);
    absC[2].y = std::abs(C[2].y);
    absC[2].z = std::abs(C[2].z);
    R         = std::abs(AD.z);
    R1  = rfmSize.x * absC[2].x + rfmSize.y * absC[2].y + rfmSize.z * absC[2].z;
    R01 = lfmSize.z + R1;

    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    if (minDistance > R01 - R)
    {
        minDistance = R01 - R;
        normal      = -lfmZAxis;
        if (AD.z < 0)
        {
            normal *= -1;
        }
    }

    // B0
    R   = std::abs(rfmXAxis.Dot(D));
    R0  = lfmSize.x * absC[0].x + lfmSize.y * absC[1].x + lfmSize.z * absC[2].x;
    R01 = R0 + rfmSize.x;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    if (minDistance > R01 - R)
    {
        minDistance = R01 - R;
        normal      = rfmXAxis;

        if (rfmXAxis.Dot(D) > 0)
        {
            normal *= -1;
        }
    }

    // B1
    R   = std::abs(rfmYAxis.Dot(D));
    R0  = lfmSize.x * absC[0].y + lfmSize.y * absC[1].y + lfmSize.z * absC[2].y;
    R01 = R0 + rfmSize.y;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    if (minDistance > R01 - R)
    {
        minDistance = R01 - R;
        normal      = rfmYAxis;
        if (rfmYAxis.Dot(D) > 0)
        {
            normal *= -1;
        }
    }

    // B2
    R   = std::abs(rfmZAxis.Dot(D));
    R0  = lfmSize.x * absC[0].z + lfmSize.y * absC[1].z + lfmSize.z * absC[2].z;
    R01 = R0 + rfmSize.z;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    if (minDistance > R01 - R)
    {
        minDistance = R01 - R;
        normal      = rfmZAxis;

        if (rfmZAxis.Dot(D) > 0)
        {
            normal *= -1;
        }
    }

    // A0xB0
    R   = std::abs(AD.z * C[1].x - AD.y * C[2].x);
    R0  = lfmSize.y * absC[2].x + lfmSize.z * absC[1].x;
    R1  = rfmSize.y * absC[0].z + rfmSize.z * absC[0].y;
    R01 = R0 + R1;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    // A0xB1
    R   = std::abs(AD.z * C[1].y - AD.y * C[2].y);
    R0  = lfmSize.y * absC[2].y + lfmSize.z * absC[1].y;
    R1  = rfmSize.x * absC[0].z + rfmSize.z * absC[0].x;
    R01 = R0 + R1;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    // A0xB2
    R   = std::abs(AD.z * C[1].z - AD.y * C[2].z);
    R0  = lfmSize.y * absC[2].z + lfmSize.z * absC[1].z;
    R1  = rfmSize.x * absC[0].y + rfmSize.y * absC[0].x;
    R01 = R0 + R1;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    // A1xB0
    R   = std::abs(AD.x * C[2].x - AD.z * C[0].x);
    R0  = lfmSize.x * absC[2].x + lfmSize.z * absC[0].x;
    R1  = rfmSize.y * absC[1].z + rfmSize.z * absC[1].y;
    R01 = R0 + R1;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    // A1xB1
    R   = std::abs(AD.x * C[2].y - AD.z * C[0].y);
    R0  = lfmSize.x * absC[2].y + lfmSize.z * absC[0].y;
    R1  = rfmSize.x * absC[1].z + rfmSize.z * absC[1].x;
    R01 = R0 + R1;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    // A1xB2
    R   = std::abs(AD.x * C[2].z - AD.z * C[0].z);
    R0  = lfmSize.x * absC[2].z + lfmSize.z * absC[0].z;
    R1  = rfmSize.x * absC[1].y + rfmSize.y * absC[1].x;
    R01 = R0 + R1;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    // A2xB0
    R   = std::abs(AD.y * C[0].x - AD.x * C[1].x);
    R0  = lfmSize.x * absC[1].x + lfmSize.y * absC[0].x;
    R1  = rfmSize.y * absC[2].z + rfmSize.z * absC[2].y;
    R01 = R0 + R1;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    // A2xB1
    R   = std::abs(AD.y * C[0].y - AD.x * C[1].y);
    R0  = lfmSize.x * absC[1].y + lfmSize.y * absC[0].y;
    R1  = rfmSize.x * absC[2].z + rfmSize.z * absC[2].x;
    R01 = R0 + R1;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    // A2xB2
    R   = std::abs(AD.y * C[0].z - AD.x * C[1].z);
    R0  = lfmSize.x * absC[1].z + lfmSize.y * absC[0].z;
    R1  = rfmSize.x * absC[2].y + rfmSize.y * absC[2].x;
    R01 = R0 + R1;
    if (R > R01)
    {
        collision.isCollide = false;
        return collision;
    }

    collision.isCollide = true;

    collision.intersectionLength = minDistance;
    collision.separationNormal   = normal.Normalize();

    return collision;
}

ImpEngineModule::Physics::Collision ImpEngineModule::Physics::Collide(
    SphereCollider* lfm,
    SphereCollider* rfm)
{
    using namespace ImpMath;

    // 구와 구의 충돌
    Collision collision{};
    collision.lfm = lfm;
    collision.rfm = rfm;

    Vector3 lfmCenterPos = lfm->GetCenterPosition();
    Vector3 rfmCenterPos = rfm->GetCenterPosition();

    Vector3 direct = lfmCenterPos - rfmCenterPos;
    // 두 원점 거리
    float distance = direct.Length();

    float lfmRadius = lfm->GetRadius();
    float rfmRadius = rfm->GetRadius();

    // 겹친 길이
    float intersection = lfmRadius + rfmRadius - distance;

    collision.intersectionLength = intersection;

    // rfm->lfm 방향 단위 벡터
    collision.separationNormal = direct.Normalize();

    if (intersection < 0)
    {
        collision.isCollide = false;
    }
    else
    {
        collision.isCollide = true;
    }

    return collision;
}

ImpEngineModule::Physics::Collision ImpEngineModule::Physics::Collide(
    BoxCollider* box,
    SphereCollider* sphere)
{
    Collision collision{};
    collision.lfm = box;
    collision.rfm = sphere;

    ImpMath::Vector3 sphereCenterPos = sphere->GetCenterPosition();
    ImpMath::Vector3 boxCenterPos    = box->GetCenterPosition();
    float radius                     = sphere->GetRadius();

    ImpMath::Vector3 closetPoint{};

    if (box->IsRotatable()) // OBB
    {
        closetPoint = ClosestPonitToOBB(sphereCenterPos, box);
    }
    else // AABB
    {
        closetPoint = ClosestPonitToAABB(sphereCenterPos, box);
    }

    float lengthSquared = (closetPoint - sphereCenterPos).LengthSquared();

    if (radius * radius >= lengthSquared)
    {
        collision.isCollide = true;

        ImpMath::Vector3 closetPointToSphereCenter = closetPoint - sphereCenterPos;
        collision.separationNormal = closetPointToSphereCenter.Normalize();

        collision.intersectionLength = radius - closetPointToSphereCenter.Length();
    }
    else
    {
        collision.isCollide = false;
    }

    return collision;
}

ImpEngineModule::Physics::Collision ImpEngineModule::Physics::Collide(
    SphereCollider* lfm,
    CapsuleCollider* rfm)
{
    using namespace ImpMath;

    Collision collision{};
    collision.lfm = lfm;
    collision.rfm = rfm;

    Vector3 lfmCenter = lfm->GetCenterPosition();
    Vector3 rfmCenter = rfm->GetCenterPosition();
    Vector3 top, bottom;

    rfm->GetTopBottomPoint(bottom, top);

    // ClosestPtPointSegment
    // top : a  ,bottom : b
    Vector3 ab = bottom - top;
    Vector3 closestPoint;

    float d = (lfmCenter - top).Dot(ab);

    if (d <= 0.f)
    {
        closestPoint = top;
    }
    else
    {
        float denom = ab.Dot(ab);
        if (d >= denom)
        {
            closestPoint = bottom;
        }
        else
        {
            d            = d / denom;
            closestPoint = top + ab * d;
        }
    }

    // 구와 구의 충돌로 해결
    Vector3 direct = lfmCenter - closestPoint;
    // 두 원점 거리
    float distance = direct.Length();

    float lfmRadius = lfm->GetRadius();
    float rfmRadius = rfm->GetRadius();

    // 겹친 길이
    float intersection = lfmRadius + rfmRadius - distance;

    collision.intersectionLength = intersection;

    // rfm->lfm 방향 단위 벡터
    collision.separationNormal = direct.Normalize();

    if (intersection < 0)
    {
        collision.isCollide = false;
    }
    else
    {
        collision.isCollide = true;
    }


    return collision;
}

ImpEngineModule::Physics::Collision ImpEngineModule::Physics::Collide(
    BoxCollider* lfm,
    CapsuleCollider* rfm)
{
    using namespace ImpMath;

    Collision collision{};
    collision.lfm = lfm;
    collision.rfm = rfm;

    Vector3 bottom, top;
    rfm->GetTopBottomPoint(bottom, top);
    float capsuleRadius   = rfm->GetRadius();
    Vector3 capsuleCenter = rfm->GetCenterPosition();
    Vector3 capsuleNormal = (top - bottom).Normalize();
    // Vector3 lineEndoff    = capsuleNormal * capsuleRadius;
    // Vector3 A             = top + lineEndoff;
    // Vector3 B             = bottom - lineEndoff;

    // 박스 경계 상자의 8개의 꼭지점
    Vector3 boxSize   = lfm->GetSize();
    Vector3 boxCenter = lfm->GetCenterPosition();

    Vector3 minPoint = -boxSize * 0.5f;
    Vector3 maxPoint = boxSize * 0.5f;

    Matrix rotM = Matrix::Identity();
    if (lfm->IsRotatable())
    {
        rotM = Matrix::MakeFromQuaternion(lfm->GetRotation());
    }

    Vector3 v[8];
    // UP
    v[0] = Vector3(minPoint.x, maxPoint.y, minPoint.z).Transform(rotM) + boxCenter;
    v[1] = Vector3(maxPoint.x, maxPoint.y, minPoint.z).Transform(rotM) + boxCenter;
    v[2] = maxPoint.Transform(rotM) + boxCenter;
    v[3] = Vector3(minPoint.x, maxPoint.y, maxPoint.z).Transform(rotM) + boxCenter;

    // Down
    v[4] = minPoint.Transform(rotM) + boxCenter;
    v[5] = Vector3(maxPoint.x, minPoint.y, minPoint.z).Transform(rotM) + boxCenter;
    v[6] = Vector3(maxPoint.x, minPoint.y, maxPoint.z).Transform(rotM) + boxCenter;
    v[7] = Vector3(minPoint.x, minPoint.y, maxPoint.z).Transform(rotM) + boxCenter;

    // up
    {
        Vector3 N = ((v[3] - v[0]).Cross(v[1] - v[0])).Normalize();

        float NCrossCaplsuleNormal = N.Dot(capsuleNormal);
        float t;
        Vector3 linePlaneIntersection;
        Vector3 boxClosestPoint;

        // 평행한 경우
        if (std::abs(NCrossCaplsuleNormal) <= 0.000001f)
        {
        }
        else
        {
            t = N.Dot((v[0] - bottom) / NCrossCaplsuleNormal);

            linePlaneIntersection = bottom + capsuleNormal * t;
            boxClosestPoint       = FindReferencePoint(
                v[0], v[3], v[2], v[1], linePlaneIntersection, N);
        }

        Vector3 capsuleClosestPoint = ClosestPointSegment(
            top, bottom, boxClosestPoint);

        float lengthSquared = (boxClosestPoint - capsuleClosestPoint).LengthSquared();

        if (capsuleRadius * capsuleRadius >= lengthSquared)
        {
            collision.isCollide = true;

            ImpMath::Vector3 closetPointToSphereCenter = boxClosestPoint -
                                                         capsuleClosestPoint;
            collision.separationNormal = closetPointToSphereCenter.Normalize();

            collision.intersectionLength = capsuleRadius -
                                           closetPointToSphereCenter.Length();

            return collision;
        }
    }

    // down
    {
        Vector3 N = ((v[5] - v[4]).Cross(v[7] - v[4])).Normalize();

        float NCrossCaplsuleNormal = N.Dot(capsuleNormal);
        float t;
        Vector3 linePlaneIntersection;
        Vector3 boxClosestPoint;

        // 평행한 경우
        if (std::abs(NCrossCaplsuleNormal) <= 0.000001f)
        {
        }
        else
        {
            t = N.Dot((v[4] - bottom) / NCrossCaplsuleNormal);

            linePlaneIntersection = bottom + capsuleNormal * t;
            boxClosestPoint       = FindReferencePoint(
                v[4], v[5], v[6], v[7], linePlaneIntersection, N);
        }

        Vector3 capsuleClosestPoint = ClosestPointSegment(
            top, bottom, boxClosestPoint);

        float lengthSquared = (boxClosestPoint - capsuleClosestPoint).LengthSquared();

        if (capsuleRadius * capsuleRadius >= lengthSquared)
        {
            collision.isCollide = true;

            ImpMath::Vector3 closetPointToSphereCenter = boxClosestPoint -
                                                         capsuleClosestPoint;
            collision.separationNormal = closetPointToSphereCenter.Normalize();

            collision.intersectionLength = capsuleRadius -
                                           closetPointToSphereCenter.Length();

            return collision;
        }
    }

    // forward
    {
        Vector3 N = ((v[1] - v[0]).Cross(v[4] - v[0])).Normalize();

        float NCrossCaplsuleNormal = N.Dot(capsuleNormal);
        float t;
        Vector3 linePlaneIntersection;
        Vector3 boxClosestPoint;

        // 평행한 경우
        if (std::abs(NCrossCaplsuleNormal) <= 0.000001f)
        {
            boxClosestPoint = v[0];
        }
        else
        {
            t = N.Dot((v[0] - bottom) / NCrossCaplsuleNormal);

            linePlaneIntersection = bottom + capsuleNormal * t;
            boxClosestPoint       = FindReferencePoint(
                v[0], v[1], v[5], v[4], linePlaneIntersection, N);
        }

        Vector3 capsuleClosestPoint = ClosestPointSegment(
            top, bottom, boxClosestPoint);

        float lengthSquared = (boxClosestPoint - capsuleClosestPoint).LengthSquared();

        if (capsuleRadius * capsuleRadius >= lengthSquared)
        {
            collision.isCollide = true;

            ImpMath::Vector3 closetPointToSphereCenter = boxClosestPoint -
                                                         capsuleClosestPoint;
            collision.separationNormal = closetPointToSphereCenter.Normalize();

            collision.intersectionLength = capsuleRadius -
                                           closetPointToSphereCenter.Length();

            return collision;
        }
    }

    return collision;
}

ImpEngineModule::Physics::Collision ImpEngineModule::Physics::Collide(
    CapsuleCollider* lfm,
    CapsuleCollider* rfm)
{
    using namespace ImpMath;

    Collision collision{};
    collision.lfm = lfm;
    collision.rfm = rfm;

    Vector3 lfmTop, lfmBottom, rfmTop, rfmBottom;
    lfm->GetTopBottomPoint(lfmBottom, lfmTop);
    rfm->GetTopBottomPoint(rfmBottom, rfmTop);

    Vector3 lfmClosestPoint, rfmClosestPoint;

    // 최단거리 두점 구하기
    ClosestPtSegmentToSegment(
        lfmTop, rfmTop, lfmBottom, rfmBottom, lfmClosestPoint, rfmClosestPoint);

    Vector3 direct = lfmClosestPoint - rfmClosestPoint;
    // 두 원점 거리
    float distance = direct.Length();

    float lfmRadius = lfm->GetRadius();
    float rfmRadius = rfm->GetRadius();

    // 겹친 길이
    float intersection = lfmRadius + rfmRadius - distance;

    collision.intersectionLength = intersection;

    // rfm->lfm 방향 단위 벡터
    collision.separationNormal = direct.Normalize();

    if (intersection < 0)
    {
        collision.isCollide = false;
    }
    else
    {
        collision.isCollide = true;
    }

    return collision;


    return collision;
}

ImpMath::Vector3 ImpEngineModule::Physics::ClosestPonitToAABB(ImpMath::Vector3 point,
    BoxCollider* box)
{
    using namespace ImpMath;
    assert(!box->IsRotatable());

    Vector3 closestPoint = point;

    Vector3 maxPoint, minPoint;
    box->GetMinMaxPoint(minPoint, maxPoint);

    if (point.x < minPoint.x)
    {
        closestPoint.x = minPoint.x;
    }
    else if (point.x > maxPoint.x)
    {
        closestPoint.x = maxPoint.x;
    }

    if (point.y < minPoint.y)
    {
        closestPoint.y = minPoint.y;
    }
    else if (point.y > maxPoint.y)
    {
        closestPoint.y = maxPoint.y;
    }

    if (point.z < minPoint.z)
    {
        closestPoint.z = minPoint.z;
    }
    else if (point.z > maxPoint.z)
    {
        closestPoint.z = maxPoint.z;
    }

    return closestPoint;
}

ImpMath::Vector3 ImpEngineModule::Physics::ClosestPonitToOBB(ImpMath::Vector3 point,
    BoxCollider* box)
{
    using namespace ImpMath;
    assert(box->IsRotatable());

    Matrix rotM = Matrix::MakeFromQuaternion(box->GetRotation());

    Matrix inversM = rotM.Inverse();

    Vector3 localPoint = point - box->GetCenterPosition();
    localPoint         = localPoint.Transform(inversM);

    Vector3 minPoint = ImpMath::Vector3(-box->GetSize() * 0.5f + box->GetOffset());
    Vector3 maxPoint = ImpMath::Vector3(box->GetSize() * 0.5f + box->GetOffset());

    Vector3 closestPoint = localPoint;

    if (localPoint.x < minPoint.x)
    {
        closestPoint.x = minPoint.x;
    }
    else if (localPoint.x > maxPoint.x)
    {
        closestPoint.x = maxPoint.x;
    }

    if (localPoint.y < minPoint.y)
    {
        closestPoint.y = minPoint.y;
    }
    else if (localPoint.y > maxPoint.y)
    {
        closestPoint.y = maxPoint.y;
    }

    if (localPoint.z < minPoint.z)
    {
        closestPoint.z = minPoint.z;
    }
    else if (localPoint.z > maxPoint.z)
    {
        closestPoint.z = maxPoint.z;
    }

    closestPoint = closestPoint.Transform(rotM) + box->GetCenterPosition();

    return closestPoint;
}

void ImpEngineModule::Physics::ClosestPtSegmentToSegment(ImpMath::Vector3 lfmTop,
    ImpMath::Vector3 rfmTop,
    ImpMath::Vector3 lfmBottom,
    ImpMath::Vector3 rfmBottom,
    ImpMath::Vector3& lfmClosestPoint,
    ImpMath::Vector3& rfmClosestPoint)
{
    using namespace ImpMath;

    // 오차를 위한 아주작은 0에 근접한 수
    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    // p1: lfmTop     p2: rfmTop
    // q1: lfmBottom  q2: rfmBottom
    Vector3 d1 = lfmBottom - lfmTop;
    Vector3 d2 = rfmBottom - rfmTop;
    Vector3 r  = lfmTop - rfmTop;

    float a = d1.Dot(d1);
    float e = d2.Dot(d2);
    float f = d2.Dot(r);

    float s = 0.f, t = 0.f;

    // 선분이 겹쳐짐
    if (a <= epsilon && e <= epsilon)
    {
        lfmClosestPoint = lfmTop;
        rfmClosestPoint = rfmTop;
        return;
    }
    if (a <= epsilon)
    {
        s = 0.0f;
        t = f / e;
        t = std::clamp(t, 0.f, 1.f);
    }
    else
    {
        float c = d1.Dot(r);
        if (e <= epsilon)
        {
            t = 0.0f;
            s = std::clamp(-c / a, 0.f, 1.f);
        }
        else
        {
            float b     = d1.Dot(d2);
            float denom = a * e - b * b;
            if (denom != 0.0f)
            {
                s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
            }
            else
            {
                s = 0.0f;
            }

            t = (b * s + f) / e;

            if (t < 0.0f)
            {
                t = 0.0f;
                s = std::clamp(-c / a, 0.0f, 1.0f);
            }
            else if (t > 1.f)
            {
                t = 1.f;
                s = std::clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }

    lfmClosestPoint = lfmTop + d1 * s;
    rfmClosestPoint = rfmTop + d2 * t;
}

ImpMath::Vector3 ImpEngineModule::Physics::ClosestPointOnLineSegment(
    ImpMath::Vector3 top,
    ImpMath::Vector3 bottom,
    ImpMath::Vector3 point)
{
    ImpMath::Vector3 ab = bottom - top;
    float t             = (point - top).Dot(ab) / ab.Dot(ab);
    return top + ab * std::min(std::max(t, 0.f), 1.f);
}

ImpMath::Vector3 ImpEngineModule::Physics::FindReferencePoint(
    const ImpMath::Vector3& p0,
    const ImpMath::Vector3& p1,
    const ImpMath::Vector3& p2,
    const ImpMath::Vector3& p3,
    const ImpMath::Vector3& target,
    const ImpMath::Vector3& N)
{
    using namespace ImpMath;

    Vector3 c0 = (target - p0).Cross(p1 - p0);
    Vector3 c1 = (target - p1).Cross(p2 - p1);
    Vector3 c2 = (target - p2).Cross(p3 - p2);
    Vector3 c3 = (target - p3).Cross(p0 - p3);

    bool isInside = c0.Dot(N) <= 0 && c1.Dot(N) <= 0 && c2.Dot(N) <= 0 &&
                    c3.Dot(N) <= 0;

    if (isInside)
    {
        return target;
    }
    else
    {
        // Edge1
        Vector3 point1         = ClosestPointOnLineSegment(p0, p1, target);
        Vector3 v1             = target - point1;
        float distq            = v1.Dot(v1);
        float best_dist        = distq;
        Vector3 referencePoint = point1;

        // Edge2
        Vector3 point2 = ClosestPointOnLineSegment(p1, p2, target);
        Vector3 v2     = target - point2;
        distq          = v2.Dot(v2);
        if (distq < best_dist)
        {
            referencePoint = point2;
            best_dist      = distq;
        }

        // Edge3
        Vector3 point3 = ClosestPointOnLineSegment(p2, p3, target);
        Vector3 v3     = target - point3;
        distq          = v3.Dot(v3);
        if (distq < best_dist)
        {
            referencePoint = point3;
            best_dist      = distq;
        }

        // Edge4
        Vector3 point4 = ClosestPointOnLineSegment(p3, p0, target);
        Vector3 v4     = target - point4;
        distq          = v4.Dot(v4);
        if (distq < best_dist)
        {
            referencePoint = point4;
            best_dist      = distq;
        }

        return referencePoint;
    }
}

ImpMath::Vector3 ImpEngineModule::Physics::ClosestPointSegment(ImpMath::Vector3 a,
    ImpMath::Vector3 b,
    ImpMath::Vector3 c)
{
    using namespace ImpMath;
    // ClosestPtPointSegment
    // top : a  ,bottom : b
    Vector3 ab = b - a;
    Vector3 closestPoint;

    float d = (c - a).Dot(ab);

    if (d <= 0.f)
    {
        closestPoint = a;
    }
    else
    {
        float denom = ab.Dot(ab);
        if (d >= denom)
        {
            closestPoint = b;
        }
        else
        {
            d            = d / denom;
            closestPoint = a + ab * d;
        }
    }

    return closestPoint;
}
