#include "ImpClientPCH.h"
#include "SoundStore.h"

ImpClient::SoundStore::SoundStore()
    : _soundPath{}
    , _soundPath2{}, _time(0.0f), _isSetSoundPath2(false)
{
}

ImpClient::SoundStore::~SoundStore()
{
}

ImpEngineModule::Component* ImpClient::SoundStore::Clone(
    Component* clone /*= nullptr*/) const
{
    SoundStore* store = static_cast<SoundStore*>(clone);

    if (store == nullptr) // 새로 생성해서 복사본을 준다
    {
        store = new SoundStore(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *store = *this;
    }

    return store;
}

void ImpClient::SoundStore::Start()
{
    SoundManager::GetInstance()->PlaySound(_soundPath, true, SoundChannel::BGM);
}

void ImpClient::SoundStore::Update(float dt)
{
    _time += dt;
    if (!_isSetSoundPath2 && _time > 300.0f)
    {
        _isSetSoundPath2 = true;

        SoundManager::GetInstance()->StopChannel(SoundChannel::BGM);
        SoundManager::GetInstance()->PlaySound(_soundPath2, true, SoundChannel::BGM);
    }
}