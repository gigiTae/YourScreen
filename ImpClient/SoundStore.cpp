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

    if (store == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        store = new SoundStore(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
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