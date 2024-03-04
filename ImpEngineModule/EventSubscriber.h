#pragma once
#include "ISubscriber.h"

namespace ImpEngineModule
{
	class World;

	/// <summary>
	/// 다중상속을 사용한 이벤트 시스템 
	/// </summary>
	template <typename Event>
	class EventSubscriber : public ISubscriber
	{
	public:
		virtual ~EventSubscriber() {}

		// Event 타입으로 상속받으면 해당 이벤트에 대한 콜백함수
		virtual void Receive(World* world, const Event& event)abstract;
	};

}
