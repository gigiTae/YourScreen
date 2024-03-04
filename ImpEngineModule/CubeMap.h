#pragma once
#include "Resource.h"


namespace ImpEngineModule
{
	/// <summary>
	/// 큐브맵 리소스
	/// </summary>
	class CubeMap :
		public Resource
	{
	public:

		/// <summary>
		/// CubeMap Load
		/// </summary>
		void Load()override;

		CubeMap(const std::wstring& path);
		~CubeMap();
	};

}