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

// CRTP ����?

/*
��������� �����ؾ� �ϴ� ��Ȳ�� ���� �� �𸣰ڴ�.
�ϴ� �������� 1�� �����Ǵϱ� �Լ� ȣ�� ������ �����ǵ��� �ؼ�
���α׷��� ������ ��(static ������ scope�� ��� ��) �ϰ� �����ϴ� �͵� ������ �� ����.*/

/* ���� �ڵ�
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
/ *__declspec(selectany) Alt+C�� �����µ� �ڵ����� �����. ���� �𸥴�.* /

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