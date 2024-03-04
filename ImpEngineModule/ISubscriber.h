#pragma once


namespace ImpEngineModule
{
	class World;

	/// <summary>
	/// 이벤트를 받는 구독자들의 인터페이스
	/// </summary>
	class ISubscriber
	{
	public:
		virtual ~ISubscriber() {}
	};


}