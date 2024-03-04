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
	// ������Ʈ Ÿ�Ե� �˾ƾ� ���� �� ����
	// ���̴�, Mesh Path�� ������ �ʿ���
	// ��ġ�� Texture�� 0~X���� �ʿ���
	template<typename ...Types>
	struct BuildInfo
	{
		ObjectType _objectType;

		std::string _meshPath;
		std::string _texturePath;
		std::string _shaderPath;
	};*/
}