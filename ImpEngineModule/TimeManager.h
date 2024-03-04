#pragma once

namespace ImpEngineModule
{
	/// <summary>
	/// dt, fps�� �ð����� ����� �ϴ� Ŭ����
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