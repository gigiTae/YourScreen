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
    QueryPerformanceFrequency(&_frequency); // 프로세서의 카운터 값의 빈도
    QueryPerformanceCounter(&_prevCount); // 이전 프레임의 카운터 값
    QueryPerformanceCounter(&_curentCount);
}

float ImpEngineModule::TimeManager::Update()
{
    QueryPerformanceCounter(&_curentCount);

    _deltaTime = (_curentCount.QuadPart - _prevCount.QuadPart) /
                 (double)_frequency.QuadPart;

    // 초반에 로딩창에서 dt가 커지는것을 방지
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
