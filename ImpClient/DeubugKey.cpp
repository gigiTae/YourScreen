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

    if (hackableObject == nullptr) // 새로 생성해서 복사본을 준다
    {
        hackableObject = new DeubugKey(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
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
