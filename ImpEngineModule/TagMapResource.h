#pragma once
#include "Resource.h"
#include "TagMap.h"

namespace ImpEngineModule
{
	/// <summary>
	/// Tag���� ������踦 �����ϴ� ���ҽ�
	/// </summary>
	class TagMapResource :	public Resource
	{
	public:
		TagMapResource(const std::wstring& path);
		~TagMapResource();
	
		// ���ҽ��� �ε��Ѵ�.
		void Load()override;

		const Physics::TagMap& GetTagMap()const { return _tagMap; }

	private:
		Physics::TagMap _tagMap;
	};

}
