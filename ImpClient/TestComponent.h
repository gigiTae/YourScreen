#pragma once

namespace ImpClient
{
	/// <summary>
	/// 테스트 용도의 컴포넌트
	/// </summary>
	class TestComponent : public ImpEngineModule::Component
	{
	public:
		TestComponent();
		~TestComponent();

		// 해당 컴포넌트의 복사본을 생성한다( 반드시 정의해야한다!!)
		Component* Clone(Component* clone = nullptr)const override;

		void Update(float dt)override;

		void Start() override;

	private:
		std::wstring _prefab;
		ImpEngineModule::EntityResource* _entityResource;

		float _moveSpeed = 100.f;
        float _elapesedTime = 0.f;

		// 리플렉션 선언
		REFLECTION_FRIEND(TestComponent);
	};

	// 리플렉션을 등록하는방법
	// 1.ImpClientHeader.h 현재 헤더를 추가한다.
	// 
	// 2.private 변수를 등록하고 싶은경우에는 위처럼 선언한다.
	// 
	// 3.ImpGameEngine->source->99.Reflection에 리플렉션 선언을한다.
	// (템플릿 클래스 특수화때문에 구조가 이상하지만 다른방법을 모르겠다...)
	// 
	// 4.Tool를 사용해서 컴포넌트를 추가한다.


}