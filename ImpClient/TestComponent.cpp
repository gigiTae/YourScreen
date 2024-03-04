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

	if (testComponent == nullptr) // 새로 생성해서 복사본을 준다
	{
		testComponent = new TestComponent(*this);
	}
	else // clone에 데이터를 복사한다.
	{
		// 기본 대입 연산자 호출한다. 
		*testComponent = *this;
	}

	return testComponent;
}

void ImpClient::TestComponent::Start()
{
	ImpEngineModule::World* world = GetWorld();

	// 1. 저장되어있는 경로를 통해서 게임을 시작하기 이전에 엔티티 리소스로 들고있는다.
	_entityResource = world->GetResourceManager()->GetEntityResource(_prefab);
}

void ImpClient::TestComponent::Update(float dt)
{
    _elapesedTime += dt;

	// InputManager 받아오기 (auto가 편하다***)

	if (_elapesedTime >= 0.5f)
	{
		// 2. 저장된 엔티티 리소스를 월드에 등록하고 총알로 생성한다.
		ImpEngineModule::Entity* ent = GetWorld()->RegisterEntity(_entityResource->GetEntity());

		// Entity의 상태를 현재 상태에 맞게 초기화한다. 

        _elapesedTime -= 0.5f;
	}
}