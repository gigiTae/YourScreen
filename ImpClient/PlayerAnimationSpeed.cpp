#include "ImpClientPCH.h"
#include "PlayerAnimationSpeed.h"

ImpClient::PlayerAnimationSpeed::PlayerAnimationSpeed()
{
}

ImpClient::PlayerAnimationSpeed::~PlayerAnimationSpeed()
{
}

ImpEngineModule::Component* ImpClient::PlayerAnimationSpeed::Clone(
    Component* clone) const

{
    PlayerAnimationSpeed* playerState = static_cast<PlayerAnimationSpeed*>(clone);

    if (playerState == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        playerState = new PlayerAnimationSpeed(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *playerState = *this;
    }

    return playerState;
}
