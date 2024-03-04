#include "ImpClientPCH.h"
#include "CullingWall.h"


ImpClient::CullingWall::CullingWall()
    : _mesh(nullptr)
{
}

ImpClient::CullingWall::~CullingWall()
{
}

ImpEngineModule::Component* ImpClient::CullingWall::Clone(
    ImpEngineModule::Component* clone) const
{
    CullingWall* attackUI = static_cast<CullingWall*>(clone);

    if (attackUI == nullptr) // 새로 생성해서 복사본을 준다
    {
        attackUI = new CullingWall(*this);
    }
    else // clone에 데이터를 복사한다.
    {
        // 기본 대입 연산자 호출한다.
        *attackUI = *this;
    }

    return attackUI;
}

void ImpClient::CullingWall::OnTriggerEnter(const Physics::Collision& collision)
{
    auto otherCollider = collision.GetOtherCollider(GetEntity());

    if (otherCollider->GetTag() == Physics::Tag::Camera)
    {
        bool isMain     = otherCollider->GetComponent<Camera>()->IsMain();
        auto renderType = _mesh->GetRenderType();

        if (isMain) // leftCulling
        {
            if (renderType == Mesh::RenderType::Default)
            {
                _mesh->SetRenderType(Mesh::RenderType::Right);
            }
            else if (renderType == Mesh::RenderType::Left)
            {
                _mesh->SetRenderType(Mesh::RenderType::Clear);
            }
        }
        else // rightCulling
        {
            if (renderType == Mesh::RenderType::Default)
            {
                _mesh->SetRenderType(Mesh::RenderType::Left);
            }
            else if (renderType == Mesh::RenderType::Right)
            {
                _mesh->SetRenderType(Mesh::RenderType::Clear);
            }
        }
    }
}


void ImpClient::CullingWall::OnTriggerExit(const Physics::Collision& collision)
{
    auto otherCollider = collision.GetOtherCollider(GetEntity());

    if (otherCollider->GetTag() == Physics::Tag::Camera)
    {
        bool isMain     = otherCollider->GetComponent<Camera>()->IsMain();
        auto renderType = _mesh->GetRenderType();

        if (isMain) // leftCulling
        {
            if (renderType == Mesh::RenderType::Right)
            {
                _mesh->SetRenderType(Mesh::RenderType::Default);
            }
            else if (renderType == Mesh::RenderType::Clear)
            {
                _mesh->SetRenderType(Mesh::RenderType::Left);
            }
        }
        else // rightCulling
        {
            if (renderType == Mesh::RenderType::Left)
            {
                _mesh->SetRenderType(Mesh::RenderType::Default);
            }
            else if (renderType == Mesh::RenderType::Clear)
            {
                _mesh->SetRenderType(Mesh::RenderType::Right);
            }
        }
    }
}

void ImpClient::CullingWall::Start()
{
    _mesh = GetComponent<Mesh>();
}
