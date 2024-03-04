#pragma once

namespace ImpEngineModule
{
class BoxCollider;
class SphereCollider;
class CapsuleCollider;
} // namespace ImpEngineModule


namespace ImpEngineModule::Physics
{
struct Collision;

// Box to Box
Collision Collide(BoxCollider* lfm, BoxCollider* rfm);

// Sphere to Sphere
Collision Collide(SphereCollider* lfm, SphereCollider* rfm);

// Capsule to Capsule
Collision Collide(CapsuleCollider* lfm, CapsuleCollider* rfm);

// Box to Sphere
Collision Collide(BoxCollider* box, SphereCollider* sphere);

// Box to Capsule
Collision Collide(BoxCollider* lfm, CapsuleCollider* rfm);

// Sphere to Capsule
Collision Collide(SphereCollider* lfm, CapsuleCollider* rfm);

// point 와 가장 AABB의 가까운 면의 점을 반환
ImpMath::Vector3 ClosestPonitToAABB(ImpMath::Vector3 point, BoxCollider* box);

// point 와 가장 OBB의 가까운 면의 점을 반환
ImpMath::Vector3 ClosestPonitToOBB(ImpMath::Vector3 point, BoxCollider* box);


/// Segment 와 Segment 사이의 최단거리점을 반환한다.
void ClosestPtSegmentToSegment(ImpMath::Vector3 lfmTop,
    ImpMath::Vector3 rfmTop,
    ImpMath::Vector3 lfmBottom,
    ImpMath::Vector3 rfmBottom,
    ImpMath::Vector3& lfmClosestPoint,
    ImpMath::Vector3& rfmClosestPoint);

ImpMath::Vector3 ClosestPointOnLineSegment(ImpMath::Vector3 top,
    ImpMath::Vector3 bottom,
    ImpMath::Vector3 point);


ImpMath::Vector3 ClosestPointSegment(ImpMath::Vector3 a,
    ImpMath::Vector3 b,
    ImpMath::Vector3 c);

ImpMath::Vector3 FindReferencePoint(const ImpMath::Vector3& p0,
    const ImpMath::Vector3& p1,
    const ImpMath::Vector3& p2,
    const ImpMath::Vector3& p3,
    const ImpMath::Vector3& target,
    const ImpMath::Vector3& N);

} // namespace ImpEngineModule::Physics
