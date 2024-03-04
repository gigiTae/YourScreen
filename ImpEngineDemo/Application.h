#pragma once

namespace ImpGameEngine { class IImpEngine; }


enum class Mode
{
	Game,
	Tool,
};

class Application
{
public:
	Application();
	~Application();

public:

	/// �ʱ�ȭ �Լ�
	void Initialize(HINSTANCE hInstance, Mode mode);
	
	/// Run Application
	void Process();

	/// ����
	void Finalize();
	
private:
	void InitializeWindow(HINSTANCE hInstance, Mode mode);

	void LoadScreenInfomation();

private:
	ImpGameEngine::IImpEngine* _gameEngine; 
	ImpGameEngine::WindowInfomation _info;
	
};

