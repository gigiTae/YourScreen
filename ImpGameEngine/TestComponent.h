#pragma once

#include "TestEnum.h"

namespace Imp
{
	class TestComponent :
		public ImpEngineModule::Component
	{
	public:
		TestComponent();

		Component* Clone()const override;

		float x =10.f;
		float y =10.f;
		float z =10.f;

		Imp::Game myLoveGame = Imp::Game::DarkSoul;

		bool _isConst = false;
		std::wstring _path = L"Cdofdkl";

		std::vector<int> vec;
	};

}




