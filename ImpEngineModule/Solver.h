#pragma once


namespace ImpEngineModule::Physics
{
	struct Collision;

	/// <summary>
	/// �浹�� ���� ó���� ����Ѵ�.
	/// </summary>
	class Solver
	{
	public:
		Solver();
		~Solver();

		void SolveCollisions(const std::vector<Collision>& collisions);
	};

}
