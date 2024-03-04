#pragma once
template <typename className>
class Singleton
{
public:
	static className& GetInstance();
};

template <typename className>
className& Singleton<className>::GetInstance()
{
	static className instance;
	return instance;
}

// CRTP 패턴?

/*
명시적으로 해제해야 하는 상황이 있을 지 모르겠다.
일단 전역으로 1개 생성되니까 함수 호출 시점에 생성되도록 해서
프로그램이 종료할 때(static 변수의 scope를 벗어날 때) 일괄 해제하는 것도 괜찮을 것 같다.*/

/* 이전 코드
#pragma once
template <typename className>
class Singleton
{
public:
	static className* GetInstance();
	void ReleaseInstance();

private:
	static className* m_instance;
};

template <typename className>
__declspec(selectany) className* Singleton<className>::m_instance = nullptr;
/ *__declspec(selectany) Alt+C를 눌렀는데 자동으로 생겼다. 뭔지 모른다.* /

template <typename className>
className* Singleton<className>::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new className;
	}
	return m_instance;
}

template <typename className>
void Singleton<className>::ReleaseInstance()
{
	delete m_instance;
	m_instance = nullptr;
}*/