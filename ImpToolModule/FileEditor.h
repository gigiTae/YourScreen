#pragma once


namespace ImpToolModule
{
	/// <summary>
	/// ������ ������ �ε��ϰ� �����ϴ� �����̴�. 
	/// 
	/// 
	/// </summary>
	class FileEditor
	{
	public:
		FileEditor();
		~FileEditor();

	public:
		/// FileEditor�� ������ �����Ѵ�.
		void Update(ImpEngineModule::World* world);

		/// ����� ���嵥���͸� �ҷ��´�.
		void LoadWorld(ImpEngineModule::World* world);

		/// ���� ���� �����͸� �����Ѵ�.
		void SaveWorld(ImpEngineModule::World* world);

	private:
	};
}