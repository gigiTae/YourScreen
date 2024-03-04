#include "ImpClientPCH.h"
#include "DeubugKey.h"

ImpClient::DeubugKey::DeubugKey()
{
}

ImpClient::DeubugKey::~DeubugKey()
{
}

ImpEngineModule::Component* ImpClient::DeubugKey::Clone(Component* clone) const
{
    DeubugKey* hackableObject = static_cast<DeubugKey*>(clone);

    if (hackableObject == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        hackableObject = new DeubugKey(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *hackableObject = *this;
    }

    return hackableObject;
}


void ImpClient::DeubugKey::Update(float dt)
{
    auto input = GetWorld()->GetInputManager();

    if (input->IsKeyState(Key::F2, KeyState::Tap))
    {
        GetWorld()->GetEventManager()->ChangeWorld(L"Loby");
    }
    else if (input->IsKeyState(Key::F3, KeyState::Tap))
    {
        GetWorld()->GetEventManager()->ChangeWorld(L"StartAnimation");
    }
    else if (input->IsKeyState(Key::F4, KeyState::Tap))
    {
        GetWorld()->GetEventManager()->ChangeWorld(L"Stage01");
    }

}
