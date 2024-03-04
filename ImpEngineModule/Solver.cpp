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

        // 1.�Ѵ� Rigidbody X
        if ((lfmRigid == nullptr && rfmRigid == nullptr) ||
            collision.lfm->IsTrigger() || collision.rfm->IsTrigger())
        {
            // �������� ó���� ���� �ʴ´�.
            continue;
        }
        // 3.�Ѵ� Rigidbody O
        else if (lfmRigid != nullptr && rfmRigid != nullptr)
        {
            bool lfmIsKinematic = lfmRigid->IsKinematic();
            bool rfmIsKinematic = rfmRigid->IsKinematic();

            // 1.�Ѵ� kinematic O
            if (lfmIsKinematic && rfmIsKinematic)
            {
                // �ƹ��� ó���� ���� �ʴ´�.
                continue;
            }
            // 2.�Ѵ� kinematic X
            else if (!lfmIsKinematic && !rfmIsKinematic)
            {
                // �ݹݾ� �о��
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

                // �浹2

                // 1. ��� �ӵ� ���
                ImpMath::Vector3 realtiveVelocity = lfmV - rfmV;

                // 2. ��� �ӵ��� �浹 ���� �������� ��ȯ
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
                //  ���� ��ź�� �浹
                if (lfmIsKinematic)
                {
                    rfmRigid->AcmulatePushDistance(
                        -collision.separationNormal * collision.intersectionLength);

                    float distance = collision.separationNormal.Dot(
                        rfmRigid->GetVelocity());

                    ImpMath::Vector3 pushV = -collision.separationNormal * distance;

                    // ���� ó��
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

                    // ���� ó��
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
        // 2.���ʸ� Rigidbody�� �ִ� ���
        // ���� ��ź�� �浹�� �����Ѵ�.
        else
        {
            if (lfmRigid == nullptr)
            {
                // ���� �浹ü�� �������� �ʴ´�.
                rfmTransfrom->AddPosition(
                    -collision.separationNormal * collision.intersectionLength);

                // ���� ��ź�� �浹
                float distance = collision.separationNormal.Dot(
                    rfmRigid->GetVelocity());
                rfmRigid->AddVelocity(-collision.separationNormal * distance);
            }
            else
            {
                // ������ �浹ü�� �������� �ʴ´�.
                lfmTransfrom->AddPosition(
                    collision.separationNormal * collision.intersectionLength);

                // ���� ��ź�� �浹
                float distance = collision.separationNormal.Dot(
                    lfmRigid->GetVelocity());
                lfmRigid->AddVelocity(-collision.separationNormal * distance);
            }
        }
    }
}
