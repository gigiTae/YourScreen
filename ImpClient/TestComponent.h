#pragma once

namespace ImpClient
{
	/// <summary>
	/// �׽�Ʈ �뵵�� ������Ʈ
	/// </summary>
	class TestComponent : public ImpEngineModule::Component
	{
	public:
		TestComponent();
		~TestComponent();

		// �ش� ������Ʈ�� ���纻�� �����Ѵ�( �ݵ�� �����ؾ��Ѵ�!!)
		Component* Clone(Component* clone = nullptr)const override;

		void Update(float dt)override;

		void Start() override;

	private:
		std::wstring _prefab;
		ImpEngineModule::EntityResource* _entityResource;

		float _moveSpeed = 100.f;
        float _elapesedTime = 0.f;

		// ���÷��� ����
		REFLECTION_FRIEND(TestComponent);
	};

	// ���÷����� ����ϴ¹��
	// 1.ImpClientHeader.h ���� ����� �߰��Ѵ�.
	// 
	// 2.private ������ ����ϰ� ������쿡�� ��ó�� �����Ѵ�.
	// 
	// 3.ImpGameEngine->source->99.Reflection�� ���÷��� �������Ѵ�.
	// (���ø� Ŭ���� Ư��ȭ������ ������ �̻������� �ٸ������ �𸣰ڴ�...)
	// 
	// 4.Tool�� ����ؼ� ������Ʈ�� �߰��Ѵ�.


}