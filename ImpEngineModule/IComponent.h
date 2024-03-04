#pragma once


namespace ImpEngineModule
{

	/// <summary>
	/// ������Ʈ �߻� Ŭ����
	/// </summary>
	class IComponent
	{
	public:
		virtual ~IComponent() {}

		virtual void Start() abstract;

		virtual void Update(float dt) abstract;
	};

}
