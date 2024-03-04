#pragma once

namespace ImpEngineModule::Physics
{
	// Tag �� Tag ������ �浹 ������ �����Ѵ�.
	struct TagMap
	{
		TagMap() :tagCheck{}{}

		void Reset();

		std::bitset<32> tagCheck[static_cast<size_t>(Tag::END)];
	};
}