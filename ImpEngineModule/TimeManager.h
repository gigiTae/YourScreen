#pragma once

namespace ImpEngineModule
{
	/// <summary>
	/// dt, fps등 시간관련 계산을 하는 클래스
	/// </summary>
	class TimeManager
	{
	public:
		TimeManager();
		~TimeManager();

		void Initialize();
		float Update();
		void Finalize();

	public:
		float GetDeltaTime() const { return static_cast<float>(_deltaTime); }
		int GetFPS() { return _fps; }

	private:
		LARGE_INTEGER _curentCount;
		LARGE_INTEGER _prevCount;
		LARGE_INTEGER _frequency;

		double _deltaTime;
		double _elapsedTime;
		int _updateCount;
		int _fps;
	};
}