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

    if (gamePad == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        gamePad = new GamePad(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *gamePad = *this;
    }

    return gamePad;
}
