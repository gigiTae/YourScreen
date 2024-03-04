#include "EngineModulePCH.h"
#include "Collision.h"
#include "Entity.h"
#include "Collider.h"

ImpEngineModule::Collider* ImpEngineModule::Physics::Collision::GetOtherCollider(
    Entity* ent) const
{
    if (lfm->GetEntity()->GetID() == ent->GetID())
    {
        return rfm;
    }

    return lfm;
}
