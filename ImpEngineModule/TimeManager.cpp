#include "EngineModulePCH.h"
#include "TimeManager.h"

ImpEngineModule::TimeManager::TimeManager()
    : _deltaTime(0.)
    , _frequency()
    , _prevCount()
    , _curentCount()
    , _elapsedTime(0.)
    , _fps(0)
    , _updateCount(0)
{
}

ImpEngineModule::TimeManager::~TimeManager()
{
}

void ImpEngineModule::TimeManager::Initialize()
{
    QueryPerformanceFrequency(&_frequency); // ���μ����� ī���� ���� ��
    QueryPerformanceCounter(&_prevCount); // ���� �������� ī���� ��
    QueryPerformanceCounter(&_curentCount);
}

float ImpEngineModule::TimeManager::Update()
{
    QueryPerformanceCounter(&_curentCount);

    _deltaTime = (_curentCount.QuadPart - _prevCount.QuadPart) /
                 (double)_frequency.QuadPart;

    // �ʹݿ� �ε�â���� dt�� Ŀ���°��� ����
    constexpr double minDeltaTime = 1. / 30.;
    _deltaTime = std::min(_deltaTime, minDeltaTime);

    _prevCount = _curentCount;

    ++_updateCount;
    _elapsedTime += _deltaTime;
    if (_elapsedTime >= 1.)
    {
        _elapsedTime -= 1.;
        _fps         = _updateCount;
        _updateCount = 0;
    }

    return static_cast<float>(_deltaTime);
}

void ImpEngineModule::TimeManager::Finalize()
{
}
