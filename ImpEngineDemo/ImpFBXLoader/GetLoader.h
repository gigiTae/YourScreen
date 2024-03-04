#pragma once

namespace FBXLoad
{
	class IFBXLoader;

	class GetLoader
	{
	public:
		IFBXLoader* GetFBXLoader();
	};
}