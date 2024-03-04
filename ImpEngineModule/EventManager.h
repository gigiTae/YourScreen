#pragma once

#include "EventSubscriber.h"

namespace ImpEngineModule
{
class World;
class WorldManager;

/// <summary>
/// World에서 발생하는 다양한 이벤트들을 처리한다.
/// 다중상속을 이용한 이벤트 시스템
/// </summary>
class EventManager
{
public:
    EventManager();
    ~EventManager();

    void Initialize(World* world, WorldManager* worldMgr);

    void Finalize();

    // 쌓아둔 이벤트들을 처리한다.
    void Update();

    // 해당하는 포인터의 구독자들을 모두 해제한다.
    void UnSubScribeAll(void* subscriber);

    // 이벤트 구독자들에게 이벤트 콜백함수를 호출한다.
    template<typename T>
    void Emit(const T& event);

    // 해당하는 이벤트의 콜백을 받을 수 있도록 구독한다.
    template<typename T>
    void SubScribe(EventSubscriber<T>* subscriber);

    // 해당하는 이벤트의 콜백을 더 이상 받지 않는다.
    template<typename T>
    void UnSubScribe(EventSubscriber<T>* subscriber);

    // 후처리 이벤를 추가한다.
    void PushPostEvent(const std::function<void()>& func);

    // 월드를 변경한다.
    void ChangeWorld(const std::wstring& worldName);

private:
    std::queue<std::function<void()>> _eventQueue;
    std::unordered_map<TypeIndex, std::vector<ISubscriber*>> _subscribers;
    World* _world;
    WorldManager* _worldManager;
};

template<typename T>
void ImpEngineModule::EventManager::Emit(const T& event)
{
    auto iter = _subscribers.find(GetTypeIndex<T>());

    if (iter != _subscribers.end())
    {
        for (ISubscriber* iSubScriber : iter->second)
        {
            EventSubscriber<T>* subscriber = reinterpret_cast<EventSubscriber<T>*>(
                iSubScriber);

            subscriber->Receive(_world, event);
        }
    }
}

template<typename T>
void ImpEngineModule::EventManager::UnSubScribe(EventSubscriber<T>* subscriber)
{
    TypeIndex index = GetTypeIndex<T>();
    auto iter       = _subscribers.find(index);

    if (iter != _subscribers.end())
    {
        auto& vec = iter->second;

        vec.erase(std::find(vec.begin(), vec.end(), subscriber), vec.end());

        if (vec.size() == 0)
        {
            _subscribers.erase(iter);
        }
    }
}

template<typename T>
void ImpEngineModule::EventManager::SubScribe(EventSubscriber<T>* subscriber)
{
    TypeIndex index = GetTypeIndex<T>();
    auto iter       = _subscribers.find(index);

    if (iter == _subscribers.end())
    {
        std::vector<ISubscriber*> vec;
        vec.push_back(subscriber);
        _subscribers.insert({ index, vec });
    }
    else
    {
        iter->second.push_back(subscriber);
    }
}

} // namespace ImpEngineModule