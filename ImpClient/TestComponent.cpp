#include "ImpClientPCH.h"
#include "TestComponent.h"
#include "PlayerCamera.h"

ImpClient::TestComponent::TestComponent()
	:_entityResource(nullptr)
{

}

ImpClient::TestComponent::~TestComponent()
{

}

ImpEngineModule::Component* ImpClient::TestComponent::Clone(Component* clone /*= nullptr*/) const
{
	TestComponent* testComponent = static_cast<TestComponent*>(clone);

	if (testComponent == nullptr) // ���� �����ؼ� ���纻�� �ش�
	{
		testComponent = new TestComponent(*this);
	}
	else // clone�� �����͸� �����Ѵ�.
	{
		// �⺻ ���� ������ ȣ���Ѵ�. 
		*testComponent = *this;
	}

	return testComponent;
}

void ImpClient::TestComponent::Start()
{
	ImpEngineModule::World* world = GetWorld();

	// 1. ����Ǿ��ִ� ��θ� ���ؼ� ������ �����ϱ� ������ ��ƼƼ ���ҽ��� ����ִ´�.
	_entityResource = world->GetResourceManager()->GetEntityResource(_prefab);
}

void ImpClient::TestComponent::Update(float dt)
{
    _elapesedTime += dt;

	// InputManager �޾ƿ��� (auto�� ���ϴ�***)

	if (_elapesedTime >= 0.5f)
	{
		// 2. ����� ��ƼƼ ���ҽ��� ���忡 ����ϰ� �Ѿ˷� �����Ѵ�.
		ImpEngineModule::Entity* ent = GetWorld()->RegisterEntity(_entityResource->GetEntity());

		// Entity�� ���¸� ���� ���¿� �°� �ʱ�ȭ�Ѵ�. 

        _elapesedTime -= 0.5f;
	}
}