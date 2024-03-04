#pragma once


/// <summary>
/// 리소스 타입
/// </summary>
enum class ResourceCategory
{
	Entity, 
	Sound,
	Mesh,
	CubeMap,
	TagMap,

	END,
};

namespace ImpPlayer
{
constexpr size_t left  = 100000;
constexpr size_t right = 100001;
}