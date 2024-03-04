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

    if (playerState == nullptr) // 새로 생성해서 복사본을 준다
    {
        playerState = new PlayerAnimationSpeed(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *playerState = *this;
    }

    return playerState;
}
