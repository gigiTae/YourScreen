#pragma once

#include <fbxsdk.h>
#include <cassert>
#include <vector>

namespace FBXLoad
{ 
	inline namespace Test
	{
		struct SceneData;

		void Import(const char* file);
	}
}