#pragma once

namespace ImpClient
{
	/// <summary>
	/// ¿¹½Ã¿ë ÃÑ¾Ë ÄÄÆ÷³ÍÆ®
	/// </summary>
	class Bullet : public ImpEngineModule::Component
	{
	public:
		Bullet();
		~Bullet();

		Component* Clone(Component* clone)const override;

		void Start()override;
		void Update(float dt);

		float GetMoveSpeed() const { return _moveSpeed; }
		void SetMoveSpeed(float val) { _moveSpeed = val; }
		void Set() { _createTime = 0.f; }

	private:
		float _moveSpeed;
		float _createTime;

		ImpEngineModule::Entity* _player;


		REFLECTION_FRIEND(Bullet);
	};
}