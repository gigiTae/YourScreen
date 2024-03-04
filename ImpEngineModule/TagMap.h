#pragma once

namespace ImpEngineModule::Physics
{
	// Tag 와 Tag 사이의 충돌 정보를 저장한다.
	struct TagMap
	{
		TagMap() :tagCheck{}{}

		void Reset();

		std::bitset<32> tagCheck[static_cast<size_t>(Tag::END)];
	};
}