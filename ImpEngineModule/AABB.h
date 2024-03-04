#pragma once

namespace ImpEngineModule
{
	class Collider;
}


namespace ImpEngineModule::Physics
{

	/// <summary>
	/// AABB Ŭ����
	/// </summary>
	struct AABB
	{
	public:
		AABB();
		AABB(const ImpMath::Vector3& min, const ImpMath::Vector3& max);

		// �ݶ��̴��� Ȯ���� �簢�� ���ο� �ִ���
		bool Contains(Collider* collider)const;

		// ���� ��ȯ
		float GetVolume()const;

		// �ٸ� AABB�� �浹�ϴ��� 
		bool IsCollision(const AABB& other)const;

		// AABB�� ���� ��ģ��.
		static AABB Union(const AABB& lfm, const AABB& rfm);

		ImpMath::Vector3 minPoint;
		ImpMath::Vector3 maxPoint;
	};


}