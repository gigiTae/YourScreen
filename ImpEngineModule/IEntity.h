#pragma once

namespace ImpEngineModule
{

	/// <summary>
	/// Entity의 추상 클래스
	/// </summary>
	class IEntity
	{
	public:
		virtual ~IEntity(){}

		virtual void Start() abstract;

		virtual void Update(float dt) abstract;

		virtual void DestroyAllComponents() abstract;
	};

}