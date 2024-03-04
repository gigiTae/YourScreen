#pragma once
#include "Resource.h"


namespace ImpEngineModule
{
	/// <summary>
	/// ť��� ���ҽ�
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