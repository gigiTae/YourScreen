#pragma once

namespace ImpToolModule
{
	namespace Save
	{
		/// Entity의 정보를 저장한다. 
		void SaveEntity(const std::filesystem::path& path, ImpEngineModule::Entity* ent);
	}
}

