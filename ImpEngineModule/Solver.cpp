#include "EngineModulePCH.h"
#include "Solver.h"
#include "Transform.h"
#include "Entity.h"
#include "Rigidbody.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Collision.h"

ImpEngineModule::Physics::Solver::Solver()
{
}

ImpEngineModule::Physics::Solver::~Solver()
{
}

void ImpEngineModule::Physics::Solver::SolveCollisions(
    const std::vector<Collision>& collisions)
{
    for (auto& collision : collisions)
    {
        Transform* lfmTransfrom = collision.lfm->GetComponent<Transform>();
        Rigidbody* lfmRigid     = collision.lfm->GetComponent<Rigidbody>();

        Transform* rfmTransfrom = collision.rfm->GetComponent<Transform>();
        Rigidbody* rfmRigid     = collision.rfm->GetComponent<Rigidbody>();

        // 1.둘다 Rigidbody X
        if ((lfmRigid == nullptr && rfmRigid == nullptr) ||
            collision.lfm->IsTrigger() || collision.rfm->IsTrigger())
        {
            // 물리적인 처리를 하지 않는다.
            continue;
        }
        // 3.둘다 Rigidbody O
        else if (lfmRigid != nullptr && rfmRigid != nullptr)
        {
            bool lfmIsKinematic = lfmRigid->IsKinematic();
            bool rfmIsKinematic = rfmRigid->IsKinematic();

            // 1.둘다 kinematic O
            if (lfmIsKinematic && rfmIsKinematic)
            {
                // 아무런 처리를 하지 않는다.
                continue;
            }
            // 2.둘다 kinematic X
            else if (!lfmIsKinematic && !rfmIsKinematic)
            {
                // 반반씩 밀어낸다
                lfmRigid->AcmulatePushDistance(
                    collision.separationNormal * collision.intersectionLength *
                    0.5f);

                rfmRigid->AcmulatePushDistance(
                    -collision.separationNormal * collision.intersectionLength *
                    0.5f);

                float lfmMass = lfmRigid->GetMass();
                float rfmMass = rfmRigid->GetMass();

                ImpMath::Vector3 lfmV = lfmRigid->GetVelocity();
                ImpMath::Vector3 rfmV = rfmRigid->GetVelocity();

                // 충돌2

                // 1. 상대 속도 계산
                ImpMath::Vector3 realtiveVelocity = lfmV - rfmV;

                // 2. 상대 속도를 충돌 법선 방향으로 변환
                float relativeSpeedAlongNormal = realtiveVelocity.Dot(
                    collision.separationNormal);
                ImpMath::Vector3 relativeSpeedNormal = collision.separationNormal *
                                                       relativeSpeedAlongNormal;
                relativeSpeedNormal = relativeSpeedNormal.Normalize();

                ImpMath::Vector3 lfmAddV = relativeSpeedNormal *
                                           (2.f * rfmMass / (lfmMass + rfmMass));
                ImpMath::Vector3 rfmAddV = relativeSpeedNormal *
                                           (2.f * lfmMass / (lfmMass + rfmMass));

                lfmRigid->AddVelocity(-lfmAddV);
                rfmRigid->AddVelocity(rfmAddV);
            }
            else
            {
                //  완전 비탄성 충돌
                if (lfmIsKinematic)
                {
                    rfmRigid->AcmulatePushDistance(
                        -collision.separationNormal * collision.intersectionLength);

                    float distance = collision.separationNormal.Dot(
                        rfmRigid->GetVelocity());

                    ImpMath::Vector3 pushV = -collision.separationNormal * distance;

                    // 경사면 처리
                    float angle = std::acos(
                        ImpMath::Vector3::UnitY.Dot(-collision.separationNormal));

                    if (angle <= rfmRigid->GetSlipAngle())
                    {
                        pushV.x = 0.f;
                        pushV.z = 0.f;
                        rfmRigid->SetOnGround(true);
                    }

                    rfmRigid->AddVelocity(pushV);
                }
                else
                {
                    lfmRigid->AcmulatePushDistance(
                        collision.separationNormal * collision.intersectionLength);
                    
                    float distance = collision.separationNormal.Dot(
                        lfmRigid->GetVelocity());

                    ImpMath::Vector3 pushV = -collision.separationNormal * distance;

                    // 경사면 처리
                    float angle = std::acos(
                        ImpMath::Vector3::UnitY.Dot(collision.separationNormal));

                    if (angle <= lfmRigid->GetSlipAngle())
                    {
                        pushV.x = 0.f;
                        pushV.z = 0.f;
                        lfmRigid->SetOnGround(true);
                    }

                    lfmRigid->AddVelocity(pushV);
                }
            }
        }
        // 2.한쪽만 Rigidbody가 있는 경우
        // 완전 비탄성 충돌을 적용한다.
        else
        {
            if (lfmRigid == nullptr)
            {
                // 왼쪽 충돌체는 움직이지 않는다.
                rfmTransfrom->AddPosition(
                    -collision.separationNormal * collision.intersectionLength);

                // 완전 비탄성 충돌
                float distance = collision.separationNormal.Dot(
                    rfmRigid->GetVelocity());
                rfmRigid->AddVelocity(-collision.separationNormal * distance);
            }
            else
            {
                // 오른쪽 충돌체는 움직이지 않는다.
                lfmTransfrom->AddPosition(
                    collision.separationNormal * collision.intersectionLength);

                // 완전 비탄성 충돌
                float distance = collision.separationNormal.Dot(
                    lfmRigid->GetVelocity());
                lfmRigid->AddVelocity(-collision.separationNormal * distance);
            }
        }
    }
}
