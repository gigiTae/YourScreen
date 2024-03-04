#pragma once

namespace ImpEngineModule
{

	/// <summary>
	/// Entity�� �߻� Ŭ����
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