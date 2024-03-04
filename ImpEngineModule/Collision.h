#pragma once

namespace ImpEngineModule
{
class Collider;
class Entity;
} // namespace ImpEngineModule


namespace ImpEngineModule::Physics
{
/// <summary>
/// 충돌 정보를 저장하는 구조체
/// </summary>
struct Collision
{
    Collider* lfm  = nullptr;
    Collider* rfm  = nullptr;
    bool isCollide = false;
    float dt       = 0.f;

    float intersectionLength = 0.f; // 겹친 길이
    ImpMath::Vector3 separationNormal = { 0.f, 0.f, 0.f }; // rfm->lfm 밀어내는 방향 단위 벡터

    Collider* GetOtherCollider(Entity* ent)const;
};


} // namespace ImpEngineModule::Physics