#include "EngineModulePCH.h"
#include "Transform.h"
#include "InputManager.h"

ImpEngineModule::Transform::Transform() :_parent(nullptr), _children{},
_worldMatrix{}, _localMatrix{}
{}


void ImpEngineModule::Transform::Start()
{
	if (IsRoot())
	{
		UpdateMatrix();
	}
}

ImpEngineModule::Component* ImpEngineModule::Transform::Clone(Component* clone /*= nullptr*/) const
{
	Transform* transform = static_cast<Transform*>(clone);

	if (transform == nullptr)
	{
		transform = new Transform(*this);
	}
	else
	{
		*transform = *this;
	}

	// 부모관계는 설정하지 않는다.
	transform->_children = {};
	transform->_worldMatrix = _localMatrix;
	transform->_parent = nullptr;

	return transform;
}

void ImpEngineModule::Transform::SetLocalMatrix(ImpMath::Matrix& local)
{
    local.Decompose(_scale, _rotation, _position);
}

void ImpEngineModule::Transform::UpdateMatrix()
{
	UpdateLocalMatrix();
	UpdateWorldlMatrix();

	for (auto child : _children)
	{
		child->UpdateMatrix();
	}
}

ImpMath::Vector3 ImpEngineModule::Transform::GetWorldPostiion() const
{
	if (IsRoot())
	{
		return _position;
	}

	return _position.Transform(_parent->GetWorldMatrix());
}

void ImpEngineModule::Transform::SetPosition(const ImpMath::Vector3& pos)
{
	_position = pos;
	UpdateMatrix();
}

void ImpEngineModule::Transform::AddPosition(const ImpMath::Vector3& deltaPosition)
{
	_position += deltaPosition;
	UpdateMatrix();
}

void ImpEngineModule::Transform::SetRotation(const ImpMath::Quaternion& rotation)
{
	_rotation = rotation;
	UpdateMatrix();
}

void ImpEngineModule::Transform::AddYAxisRotation(float radian)
{
	using namespace ImpMath;
	Quaternion q = Quaternion::MakeFromAxis(Vector3::UnitY, radian);
	_rotation *= q;
	UpdateMatrix();
}

void ImpEngineModule::Transform::AddZAxisRotation(float radian)
{
	using namespace ImpMath;
	Quaternion q = Quaternion::MakeFromAxis(Vector3::UnitZ, radian);
	_rotation *= q;
	UpdateMatrix();
}

void ImpEngineModule::Transform::AddXAxisRotation(float radian)
{
	using namespace ImpMath;
	Quaternion q = Quaternion::MakeFromAxis(Vector3::UnitX, radian);
	_rotation *= q;
	UpdateMatrix();
}

void ImpEngineModule::Transform::AddYawRotation(float radian)
{
	using namespace ImpMath;
	Quaternion q = Quaternion::MakeFromAxis(Vector3::UnitY, radian);
	_rotation = q * _rotation;
	UpdateMatrix();
}

void ImpEngineModule::Transform::AddPitchRotation(float radian)
{
	using namespace ImpMath;
	Quaternion q = Quaternion::MakeFromAxis(Vector3::UnitX, radian);
	_rotation = q * _rotation;
	UpdateMatrix();
}

void ImpEngineModule::Transform::AddRollRotation(float radian)
{
	using namespace ImpMath;
	Quaternion q = Quaternion::MakeFromAxis(Vector3::UnitZ, radian);
	_rotation = q * _rotation;
	UpdateMatrix();
}

void ImpEngineModule::Transform::UpdateLocalMatrix()
{
	using namespace ImpMath;

	Matrix rot = Matrix::MakeFromQuaternion(_rotation);
	Matrix scale = Matrix::MakeScaleMatrix(_scale);
	Matrix translation = Matrix::MakeTranslationMatrix(_position);

	// SRT
	_localMatrix = scale * rot * translation;
}

void ImpEngineModule::Transform::UpdateWorldlMatrix()
{
	if (IsRoot())
	{
		_worldMatrix = _localMatrix;
	}
	else
	{
		ImpMath::Matrix parentMatrix = _parent->GetWorldMatrix();
		_worldMatrix = _localMatrix * parentMatrix;
	}
}

void ImpEngineModule::Transform::SetParent(ImpEngineModule::Transform* parent)
{
	if (parent == _parent || parent == this)
	{
		return;
	}

	_parent = parent;

	if (_parent != nullptr)
	{
		_parent->AddChild(this);
	}
}

void ImpEngineModule::Transform::AddChild(Transform* addChild)
{
	if (addChild == this)
	{
		return;
	}

	for (Transform* child : _children)
	{
		if (addChild == child)
		{
			return;
		}
	}

	Transform* prevParent = addChild->GetParent();

	if (prevParent != nullptr)
	{
		prevParent->RemoveChild(addChild);
	}

	_children.push_back(addChild);

	addChild->SetParent(this);
}

void ImpEngineModule::Transform::RemoveChild(Transform* child)
{
	// 자식목록에서 제거한다.
	_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());

	// 자식쪽 부모도 연결을 해제한다.
	child->SetParent(nullptr);
}

bool ImpEngineModule::Transform::IsDescendant(Transform* transform)
{
	std::queue<Transform*> q;

	q.push(this);

	while (!q.empty())
	{
		Transform* tmp = q.front();
		q.pop();

		if (tmp == transform)
		{
			return true;
		}

		if (!tmp->IsLeaf())
		{
			for (Transform* child : tmp->GetChildren())
			{
				q.push(child);
			}
		}
	}

	return false;
}
