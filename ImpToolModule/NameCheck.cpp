#include "ImpToolModulePCH.h"
#include "NameCheck.h"

void ImpEngineModule::NameCheck::CheckNameDuplication(
    ImpEngineModule::World* world,
    ImpEngineModule::Entity* ent,
    std::string& newName)
{
    const auto& entities = world->GetEntities();

    while (FindSameName(entities, ent, newName))
    {
        AddIndexName(newName);
    }
}

bool ImpEngineModule::NameCheck::FindSameName(
    const std::vector<std::shared_ptr<ImpEngineModule::Entity>>& entities,
    ImpEngineModule::Entity* ent,
    std::string& newName)
{
    for (const std::shared_ptr<ImpEngineModule::Entity>& entity : entities)
    {
        if (ent != entity.get() && newName == entity->GetName() &&
            entity->GetState() == ImpEngineModule::Entity::EntityState::Active)
        {
            return true;
        }
    }

    return false;
}

void ImpEngineModule::NameCheck::AddIndexName(std::string& name)
{
    size_t open = name.find('(');

    if (open == std::string::npos)
    {
        name += "(1)";
        return;
    }

    size_t close = name.find(')');

    assert(close != std::string::npos && "이상한 이름은 짓지 말아주세요");

    std::string sIndex = name.substr(open + 1, close - open - 1);
    int index          = std::stoi(sIndex);
    ++index;

    std::string newName = name.substr(0, open);

    newName += '(';
    newName += std::to_string(index);
    newName += ')';

    name = newName;
}
