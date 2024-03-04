#include "ImpGameEnginePCH.h"
#include "TestComponent.h"

Imp::TestComponent::TestComponent()
{

}


ImpEngineModule::Component* Imp::TestComponent::Clone() const
{
	TestComponent* clone = new TestComponent(*this);
	
	return clone;
}


