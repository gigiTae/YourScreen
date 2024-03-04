#pragma once


namespace ImpEngineModule
{
	namespace NameCheck
	{
		/// 중복된 이름을 체크해서 중복되지 않은 이름으로 바꿔준다.
		void CheckNameDuplication(ImpEngineModule::World* world, ImpEngineModule::Entity* ent,std::string& newName);

		/// 중복된 이름이 존재하는지 검사한다.
		bool FindSameName(const std::vector<std::shared_ptr<ImpEngineModule::Entity>>& entities, ImpEngineModule::Entity* ent, std::string& newName);

		/// 기존의 이름에 새로운 번호를 부여한다.
		void AddIndexName(std::string& name);

	}


}