#pragma once
#include <string>

namespace ImpStructure { struct ImpMesh; }

namespace ImpGraphics
{
	class ImpLoader
	{
	public:
		ImpStructure::ImpMesh* GetMesh(const std::wstring& path);
		void SaveMesh(ImpStructure::ImpMesh* mesh, std::wstring fileName);
	};
}