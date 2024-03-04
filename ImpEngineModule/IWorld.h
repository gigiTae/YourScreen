#pragma once



namespace ImpEngineModule
{
	class Entity;

	/// <summary>
	/// World�� �߻�Ŭ����
	/// </summary>
	class IWorld
	{
	public:
		virtual ~IWorld() {}

		virtual void Reset() abstract;

		virtual void Start() abstract;

		virtual void Update(float dt) abstract;
	};

}