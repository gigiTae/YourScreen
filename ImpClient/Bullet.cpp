#include "ImpClientPCH.h"
#include "Bullet.h"


ImpClient::Bullet::Bullet()
    : _moveSpeed(0.f)
    , _createTime(0.f)
{
}

ImpClient::Bullet::~Bullet()
{
}

ImpEngineModule::Component* ImpClient::Bullet::Clone(Component* clone) const
{
    Bullet* bullet = static_cast<Bullet*>(clone);

    if (bullet == nullptr)
    {
        bullet = new Bullet(*this);
    }
    else
    {
        *bullet = *this;
    }

    return bullet;
}

void ImpClient::Bullet::Start()
{
    if (GetComponent<ImpEngineModule::Camera>()->IsMain())
    {
        _player = GetWorld()->GetEntity("Player1");
    }
    else
    {
        _player = GetWorld()->GetEntity("Player2");
    }
}

void ImpClient::Bullet::Update(float dt)
{
    auto cameraT = GetComponent<ImpEngineModule::Transform>();
    auto playerT = _player->GetComponent<ImpEngineModule::Transform>();

    ImpMath::Vector3 playerPos = playerT->GetWorldPostiion();
    
    if (_player->GetName() == "Player1")
    {
        ImpMath::Vector3 cameraPos = playerPos + ImpMath::Vector3(0.f, 25.f, -50.f);
        cameraT->SetPosition(cameraPos); 
    }
    else
    {
        ImpMath::Vector3 cameraPos = playerPos + ImpMath::Vector3(0.f , 25.f, 50.f);
        cameraT->SetPosition(cameraPos);
    }

}
