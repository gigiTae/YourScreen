#include "ImpClientPCH.h"
#include "HackingArrow.h"
#include "PlayerState.h"

ImpClient::HackingArrow::HackingArrow()
    : _offset(3.f)
    , _target(nullptr)
    , _targetName{}
    , _transform(nullptr)
    , _prevPlayer(nullptr)
{
}

ImpClient::HackingArrow::~HackingArrow()
{
}

ImpEngineModule::Component* ImpClient::HackingArrow::Clone(
    Component* clone /*= nullptr*/) const
{
    HackingArrow* hackingArrow = static_cast<HackingArrow*>(clone);

    if (hackingArrow == nullptr) // ���� �����ؼ� ���纻�� �ش�
    {
        hackingArrow = new HackingArrow(*this);
    }
    else // clone�� �����͸� �����Ѵ�.
    {
        // �⺻ ���� ������ ȣ���Ѵ�.
        *hackingArrow = *this;
    }

    hackingArrow->_target = nullptr;

    return hackingArrow;
}

void ImpClient::HackingArrow::Start()
{
    _transform = GetComponent<ImpEngineModule::Transform>();
}

void ImpClient::HackingArrow::Update(float dt)
{
    if (_target == nullptr)
    {
        _target = GetWorld()->GetEntity(_targetName);
    }

    if (_target == nullptr)
    {
        return;
    }

    auto parentT = _transform->GetParent();

    if (parentT == nullptr)
    {
        return;
    }

    auto targetT = _target->GetComponent<ImpEngineModule::Transform>();

    ImpMath::Vector3 parentPos           = parentT->GetWorldPostiion();
    ImpMath::Quaternion inverseParentRot = parentT->GetRotation().Inverse();
    ImpMath::Matrix rotM = ImpMath::Matrix::MakeFromQuaternion(inverseParentRot);

    ImpMath::Vector3 targetPos = targetT->GetWorldPostiion();
    // parent->target ���⺤��
    ImpMath::Vector3 direct = targetPos - parentPos;

    direct.y = 0.f;
    direct   = direct.Normalize();

    ImpMath::Quaternion rot = ImpMath::Quaternion::MakeLookRotation(
        direct, ImpMath::Vector3::UnitY);

    // ȭ��ǥ�� 90�� ��������Ѵ�
    rot *= Quaternion::MakeFromAxis(Vector3::UnitY, -ImpMath::HalfPI);

    _transform->SetRotation(inverseParentRot * rot);

    // �θ���Ŀ��� ȸ���߱⶧���� ���������� ȸ���ؾ��Ѵ�.
    ImpMath::Vector3 pos = (direct * _offset).Transform(rotM);

    _transform->SetPosition(pos);

    CheckReturnPrevPlayer();
}


void ImpClient::HackingArrow::Steal(ImpEngineModule::Entity* stealPlayer)
{
    _prevPlayer = _transform->GetParent()->GetEntity();

    _transform->SetParent(stealPlayer->GetComponent<Transform>());

    auto mesh = GetComponent<Mesh>();

    auto renderType = mesh->GetRenderType();

    if (renderType == Mesh::RenderType::Left)
    {
        mesh->SetRenderType(Mesh::RenderType::Right);
    }
    else if (renderType == Mesh::RenderType::Right)
    {
        mesh->SetRenderType(Mesh::RenderType::Left);
    }
}

void ImpClient::HackingArrow::CheckReturnPrevPlayer()
{
    if (_prevPlayer == nullptr) return;

    auto state = _prevPlayer->GetComponent<PlayerState>()->GetActionState();

    // ���� �÷��̾�� ���ư���.
    if (state != PlayerState::ActionState::Sturn)
    {
        _transform->SetParent(_prevPlayer->GetComponent<Transform>());
        _prevPlayer     = nullptr;
        auto mesh       = GetComponent<Mesh>();
        auto renderType = mesh->GetRenderType();

        if (renderType == Mesh::RenderType::Left)
        {
            mesh->SetRenderType(Mesh::RenderType::Right);
        }
        else if (renderType == Mesh::RenderType::Right)
        {
            mesh->SetRenderType(Mesh::RenderType::Left);
        }
    }
}
