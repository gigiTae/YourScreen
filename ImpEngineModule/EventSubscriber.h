#pragma once
#include "ISubscriber.h"

namespace ImpEngineModule
{
	class World;

	/// <summary>
	/// ���߻���� ����� �̺�Ʈ �ý��� 
	/// </summary>
	template <typename Event>
	class EventSubscriber : public ISubscriber
	{
	public:
		virtual ~EventSubscriber() {}

		// Event Ÿ������ ��ӹ����� �ش� �̺�Ʈ�� ���� �ݹ��Լ�
		virtual void Receive(World* world, const Event& event)abstract;
	};

}
