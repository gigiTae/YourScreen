#pragma once

namespace ImpEngineModule
{
	class Collider;
}


namespace ImpEngineModule::Physics
{

	/// <summary>
	/// AABB 클래스
	/// </summary>
	struct AABB
	{
	public:
		AABB();
		AABB(const ImpMath::Vector3& min, const ImpMath::Vector3& max);

		// 콜라이더가 확장한 사각형 내부에 있는지
		bool Contains(Collider* collider)const;

		// 부피 반환
		float GetVolume()const;

		// 다른 AABB와 충돌하는지 
		bool IsCollision(const AABB& other)const;

		// AABB를 서로 합친다.
		static AABB Union(const AABB& lfm, const AABB& rfm);

		ImpMath::Vector3 minPoint;
		ImpMath::Vector3 maxPoint;
	};


}