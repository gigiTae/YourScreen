#include "ImpToolModulePCH.h"
#include "Save.h"

void ImpToolModule::Save::SaveEntity(const std::filesystem::path& path, ImpEngineModule::Entity* ent)
{
	using namespace ImpReflection;
	using namespace ImpEngineModule;

	std::filesystem::path entPath = path;

	// name���� �����Ѵ�
	entPath += ent->GetName();
	entPath += ".ent";

	std::ofstream output(entPath);

	if (output.is_open())
	{
		std::string writeData = "ID<";
		writeData += std::to_string(ent->GetID());
		writeData += ">";

		Transform* parentT = ent->GetComponent<Transform>()->GetParent();
		if (parentT != nullptr)
		{
			std::string parentName = parentT->GetEntity()->GetName();
			writeData += " Parent<" + parentName + ">";
		}
		writeData += '\n';
	
		// Component���� ��ȸ�ϸ鼭 �����Ѵ�.
		for (auto& iter : ent->GetComponents())
		{
			Type* type = TypeManager::GetInstance()->GetType(iter.first);

			Component* component = iter.second;

			type->Write(component, writeData);

			writeData += '\n';
		}

		output << writeData;

		output.close();
	}
}
