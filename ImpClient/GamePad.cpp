#include "ImpClientPCH.h"
#include "GamePad.h"

ImpClient::GamePad::GamePad()
    : _padID(0)
{
}

ImpClient::GamePad::~GamePad()
{
}

ImpEngineModule::Component* ImpClient::GamePad::Clone(Component* clone) const
{
    GamePad* gamePad = static_cast<GamePad*>(clone);

    if (gamePad == nullptr) // 새로 생성해서 복사본을 준다
    {
        gamePad = new GamePad(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *gamePad = *this;
    }

    return gamePad;
}
