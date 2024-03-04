#pragma once
#include "Resource.h"
#include "TagMap.h"

namespace ImpEngineModule
{
	/// <summary>
	/// Tag들의 상관관계를 저장하는 리소스
	/// </summary>
	class TagMapResource :	public Resource
	{
	public:
		TagMapResource(const std::wstring& path);
		~TagMapResource();
	
		// 리소스를 로드한다.
		void Load()override;

		const Physics::TagMap& GetTagMap()const { return _tagMap; }

	private:
		Physics::TagMap _tagMap;
	};

}
