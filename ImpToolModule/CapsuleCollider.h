#pragma once
#include "DebugObject.h"

namespace ImpToolModule::Debug
{

/// <summary>
/// CapuleCollider 디버그 용도
/// </summary>
class CapsuleCollider: public DebugObject
{
public:
    CapsuleCollider(ImpGraphics::IImpGraphicsEngine* graphics,
                    ImpEngineModule::Entity* ent);
    ~CapsuleCollider();

    void Update() override;

private:
    ImpEngineModule::Entity* _ent;
    ImpEngineModule::Transform* _transform;
    ImpEngineModule::CapsuleCollider* _capsule;

    const size_t capsuleID1 = 500000000000000000;
    const size_t capsuleID2 = 600000000000000000;
    const size_t capsuleID3 = 700000000000000000;
    /*
    const size_t capsuleID4 = 800000000000000000;
    const size_t capsuleID5 = 900000000000000000;*/
};

} // namespace ImpToolModule::Debug