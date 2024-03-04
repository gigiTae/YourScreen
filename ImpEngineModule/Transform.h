#pragma once
#include "Component.h"

namespace ImpEngineModule
{
	/// <summary>
	/// ��ġ, ȸ��, ũ�� ������ �����ϴ� ������Ʈ
	/// </summary>
	class Transform :
		public Component
	{
	public:
		Transform();
		~Transform() {}

		void Start() override;

		/// ���纻�� ��ȯ�Ѵ�. 
		Component* Clone(Component* clone = nullptr)const override;

	public:
		// ��Ŀ� �ش��ϴ� Ʈ���������� �����Ѵ�
        void SetLocalMatrix(ImpMath::Matrix& local);

		// ���� ������ ��ġ�� ��ȯ
		ImpMath::Vector3 GetLocalPosition()const { return  _position; }
		
		// ������� ��ġ��ȯ
		ImpMath::Vector3 GetWorldPostiion()const;

		void SetPosition(const ImpMath::Vector3& pos);

		void AddPosition(const ImpMath::Vector3& deltaPosition);

		ImpMath::Quaternion GetRotation()const { return _rotation; }
		void SetRotation(const ImpMath::Quaternion& rotation);

		// Y��ȸ��
		void AddYAxisRotation(float radian);
		// Z��ȸ��
		void AddZAxisRotation(float radian);
		// X��ȸ��
		void AddXAxisRotation(float radian);

		void AddYawRotation(float radian);
		void AddPitchRotation(float radian);
		void AddRollRotation(float radian);

		ImpMath::Vector3 GetScale()const { return _scale; }
		void SetScale(const ImpMath::Vector3& scale) { _scale = scale; }
		void AddScale(const ImpMath::Vector3& deltaScale) { _scale += deltaScale; }

		// ��������� ��´�.
		ImpMath::Matrix GetWorldMatrix()const { return _worldMatrix; }

		// ��������� �����´�.
		ImpMath::Matrix GetLocalMatrix()const { return _localMatrix; }

#pragma region Hierarchy

		// �θ� Ʈ�������� ���´�.
		ImpEngineModule::Transform* GetParent() const { return _parent; }

		// �θ� �����Ѵ�.
		void SetParent(ImpEngineModule::Transform* parent);

		// �ڽĵ��� ���´�.
		std::vector<Transform*>& GetChildren() { return _children; }

		// �ڽ��� �߰��Ѵ�.
		void AddChild(Transform* addChild);

		void RemoveChild(Transform* child);

		// �θ� ������
		bool IsRoot()const { return _parent == nullptr; }

		// �ڽ��� ������
		bool IsLeaf()const { return _children.empty(); }

		// �ڽ��� ���� ������ �ִ��� Ȯ���Ѵ�.
		bool IsDescendant(Transform* transform);

#pragma endregion Hierarchy

		// ���� ����� ����Ѵ�.
		void UpdateLocalMatrix();

		// ���� ����� ����Ѵ�.
		void UpdateWorldlMatrix();

		void UpdateMatrix();

	private:
		ImpMath::Vector3 _position = { 0.f,0.f,0.f };
		ImpMath::Quaternion _rotation = { 0.f,0.f,0.f,1.f };
		ImpMath::Vector3 _scale = { 1.f,1.f,1.f };

		ImpMath::Matrix _localMatrix; // �ڽ��� ���
		ImpMath::Matrix _worldMatrix; // �ڽ��� ������� 

		Transform* _parent;
		std::vector<Transform*> _children;

		// ���÷��� ����
		REFLECTION_FRIEND(Transform)
	};
}