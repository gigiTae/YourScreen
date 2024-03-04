#pragma once


namespace ImpEngineModule
{
	namespace NameCheck
	{
		/// �ߺ��� �̸��� üũ�ؼ� �ߺ����� ���� �̸����� �ٲ��ش�.
		void CheckNameDuplication(ImpEngineModule::World* world, ImpEngineModule::Entity* ent,std::string& newName);

		/// �ߺ��� �̸��� �����ϴ��� �˻��Ѵ�.
		bool FindSameName(const std::vector<std::shared_ptr<ImpEngineModule::Entity>>& entities, ImpEngineModule::Entity* ent, std::string& newName);

		/// ������ �̸��� ���ο� ��ȣ�� �ο��Ѵ�.
		void AddIndexName(std::string& name);

	}


}