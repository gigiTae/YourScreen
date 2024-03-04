#pragma once


namespace ImpToolModule
{
	/// <summary>
	/// 월드의 정보를 로드하고 저장하는 역할이다. 
	/// 
	/// 
	/// </summary>
	class FileEditor
	{
	public:
		FileEditor();
		~FileEditor();

	public:
		/// FileEditor의 로직을 수행한다.
		void Update(ImpEngineModule::World* world);

		/// 저장된 월드데이터를 불러온다.
		void LoadWorld(ImpEngineModule::World* world);

		/// 현재 월드 데이터를 저장한다.
		void SaveWorld(ImpEngineModule::World* world);

	private:
	};
}