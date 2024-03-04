#include "ImpToolModulePCH.h"
#include "FileEditor.h"

ImpToolModule::FileEditor::FileEditor()
{

}

ImpToolModule::FileEditor::~FileEditor()
{

}

void ImpToolModule::FileEditor::Update(ImpEngineModule::World* world)
{
	ImGui::Begin("File");

	if (ImGui::BeginMenu("dd"))
	{

		ImGui::EndMenu();
	}



	ImGui::End();
}

void ImpToolModule::FileEditor::LoadWorld(ImpEngineModule::World* world)
{

}

void ImpToolModule::FileEditor::SaveWorld(ImpEngineModule::World* world)
{

}
