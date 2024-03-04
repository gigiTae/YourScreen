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

	/// 초기화 함수
	void Initialize(HINSTANCE hInstance, Mode mode);
	
	/// Run Application
	void Process();

	/// 종료
	void Finalize();
	
private:
	void InitializeWindow(HINSTANCE hInstance, Mode mode);

	void LoadScreenInfomation();

private:
	ImpGameEngine::IImpEngine* _gameEngine; 
	ImpGameEngine::WindowInfomation _info;
	
};

