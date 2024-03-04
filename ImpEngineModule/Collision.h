#pragma once

namespace ImpEngineModule
{
class Collider;
class Entity;
} // namespace ImpEngineModule


namespace ImpEngineModule::Physics
{
/// <summary>
/// �浹 ������ �����ϴ� ����ü
/// </summary>
struct Collision
{
    Collider* lfm  = nullptr;
    Collider* rfm  = nullptr;
    bool isCollide = false;
    float dt       = 0.f;

    float intersectionLength = 0.f; // ��ģ ����
    ImpMath::Vector3 separationNormal = { 0.f, 0.f, 0.f }; // rfm->lfm �о�� ���� ���� ����

    Collider* GetOtherCollider(Entity* ent)const;
};


} // namespace ImpEngineModule::Physics