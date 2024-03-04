#pragma once


namespace ImpEngineModule::Physics
{
	struct Collision;

	/// <summary>
	/// 충돌에 대한 처리를 담당한다.
	/// </summary>
	class Solver
	{
	public:
		Solver();
		~Solver();

		void SolveCollisions(const std::vector<Collision>& collisions);
	};

}
