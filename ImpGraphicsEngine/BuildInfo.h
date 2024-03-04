#pragma once
#include <string>

namespace ImpGraphics
{
	enum class ObjectType
	{
		MeshObject,
		LineListObject,
		QuadObject,
		SkyBoxObject,
	};

	enum class TextureType
	{
		BaseColor
	};

/*
	// 오브젝트 타입도 알아야 만들 수 있음
	// 셰이더, Mesh Path는 무조건 필요함
	// 그치만 Texture는 0~X개가 필요함
	template<typename ...Types>
	struct BuildInfo
	{
		ObjectType _objectType;

		std::string _meshPath;
		std::string _texturePath;
		std::string _shaderPath;
	};*/
}