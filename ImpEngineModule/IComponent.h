#pragma once


namespace ImpEngineModule
{

	/// <summary>
	/// 컴포넌트 추상 클래스
	/// </summary>
	class IComponent
	{
	public:
		virtual ~IComponent() {}

		virtual void Start() abstract;

		virtual void Update(float dt) abstract;
	};

}
