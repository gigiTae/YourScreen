#include "ImpGameEnginePCH.h"
#include "RenderingSystem.h"
#include "../ImpGraphicsEngine/ImpGraphicsEngine.h"

ImpGameEngine::RenderingSystem::RenderingSystem()
    : _graphicsEngine(nullptr)
    , _engineModule(nullptr)
    , _currenEntityID(0)
    , _bindMeshID{}
{
    _clearRenderMatrix     = ImpMath::Matrix::Identity();
    _clearRenderMatrix._44 = 0.f;
}

ImpGameEngine::RenderingSystem::~RenderingSystem()
{
}

void ImpGameEngine::RenderingSystem::Initialize(
    ImpGraphics::IImpGraphicsEngine* graphics,
    ImpEngineModule::EngineModule* module)
{
    using namespace ImpEngineModule;

    _graphicsEngine = graphics;
    _engineModule   = module;

    EventManager* eventMgr = _engineModule->GetEventManager();
    eventMgr->SubScribe<Event::OnEntityCreated>(this);
    eventMgr->SubScribe<Event::OnEntityStarted>(this);
    eventMgr->SubScribe<Event::OnEntityDestroyed>(this);
    eventMgr->SubScribe<Event::LoadCubeMap>(this);
    eventMgr->SubScribe<Event::LoadedWorld>(this);
}


void ImpGameEngine::RenderingSystem::UnloadEntityResources()
{
    if (_bindMeshID.empty()) return;

    for (size_t id : _bindMeshID)
    {
        _graphicsEngine->DeleteMeshObject(id);
    }

    _bindMeshID.clear();
}

void ImpGameEngine::RenderingSystem::LoadEntityResources()
{
    using namespace ImpEngineModule;
    using namespace ImpGraphics;

    _currenEntityID = entityResourcesID;
    const auto& entityResources = _engineModule->GetResourceManager()->GetEntityResources();

    for (const auto& entRes : entityResources)
    {
        const auto& prototype = entRes.second->GetPrototype();

        if (prototype->HasComponent<ImpEngineModule::Mesh>())
        {
            auto mesh = prototype->GetComponent<ImpEngineModule::Mesh>();

            MeshObjectInfo meshInfo;

            meshInfo._objectID                   = _currenEntityID;
            meshInfo._meshPath                   = mesh->GetFBXPath();
            meshInfo._pisxelShaderPath           = mesh->GetPixelShaderPath();
            meshInfo._vertexShaderPath           = mesh->GetVertexShaderPath();
            meshInfo._material.baseColor         = mesh->GetBaseColor();
            meshInfo._material.metallicRoughness = mesh->GetMetalic();
            meshInfo._material.emission          = mesh->GetEmission();
            meshInfo._material.normalMap         = mesh->GetNormal();
            meshInfo._isOpaque                   = mesh->IsOpaque();
            meshInfo._isCullNone                 = mesh->IsCullNone();

            // 바인딩할 정보를 확인한다

            // 1. 경로가 없는 경우 바인딩하지 않는다.
            if (meshInfo._meshPath.empty() || meshInfo._pisxelShaderPath.empty() ||
                meshInfo._vertexShaderPath.empty())
            {
                mesh->SetBindGraphics(false);
                return;
            }

            // 2. 확장자명을 확인하고 맞지 않으면 초기화한다.

            // FBX
            std::wstring meshPathExtension = PathManager::GetExtension(
                meshInfo._meshPath);
            if (!(meshPathExtension != L".fbx" || meshPathExtension != L".imp"))
            {
                mesh->SetFBXPath(std::wstring());
                return;
            }

            // hlsl , cso
            std::wstring pixelExtension = PathManager::GetExtension(
                meshInfo._pisxelShaderPath);
            if (!(pixelExtension == L".cso" || pixelExtension == L".hlsl"))
            {
                mesh->SetPixelShaderPath(std::wstring());
                return;
            }

            // hlsl , cso
            std::wstring vertexExtension = PathManager::GetExtension(
                meshInfo._vertexShaderPath);
            if (!(vertexExtension == L".cso" || vertexExtension == L".hlsl"))
            {
                mesh->SetVertexShaderPath(std::wstring());
                return;
            }

            // 3. 그래픽스엔진에 바인딩한다.
            _graphicsEngine->AddMeshObejct(meshInfo);
            mesh->SetBindGraphics(true);

            ImpMath::Matrix mat = ImpMath::Matrix::Identity();
            mat._44             = 0;
            _graphicsEngine->UpdateMeshObject(_currenEntityID, mat);

            _bindMeshID.push_back(_currenEntityID++);
        }
    }
}
void ImpGameEngine::RenderingSystem::Update()
{
    using namespace ImpEngineModule;
    World* world = _engineModule->GetWorld();

    // Mesh Binding
    world->Each<Transform, Mesh>(
        [this](Entity* ent,
            ComponentHandle<Transform> transform,
            ComponentHandle<Mesh> mesh)
        {
            if (mesh->IsBindGraphics())
            {
                if (mesh->IsChangedAlpha() && !mesh->IsOpaque())
                {
                    _graphicsEngine->SetTransparentObjectAlpha(
                        ent->GetID(), mesh->GetAlpha());

                    mesh->SetIsChangedAlpha(false);
                }

                if (mesh->IsChangedEmissionIntensity())
                {
                    _graphicsEngine->SetObjectEmissionCoefficient(
                        ent->GetID(), mesh->GetEmissionIntensity());

                    mesh->SetIsChangedEmissionIntensity(false);
                }

                _graphicsEngine->UpdateMeshObject(
                    ent->GetID(), transform->GetWorldMatrix());
            }
        },
        false);

    // SkeletalMesh
    world->Each<Transform, SkeletalMesh>(
        [this](Entity* ent,
            ComponentHandle<Transform> transform,
            ComponentHandle<SkeletalMesh> mesh)
        {
            if (mesh->IsBindGraphics())
            {
                auto type = mesh->GetRenderType();

                if (mesh->IsChangedAlpha() && !mesh->IsOpaque())
                {
                    _graphicsEngine->SetTransparentObjectAlpha(
                        ent->GetID(), mesh->GetAlpha());

                    mesh->SetIsChangedAlpha(false);
                }

                if (mesh->IsChangedEmissionIntensity())
                {
                    _graphicsEngine->SetObjectEmissionCoefficient(
                        ent->GetID(), mesh->GetEmissionIntensity());

                    mesh->SetIsChangedEmissionIntensity(false);
                }

                if (type == SkeletalMesh::RenderType::Right ||
                    type == SkeletalMesh::RenderType::Clear)
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), _clearRenderMatrix);
                }
                else
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), transform->GetWorldMatrix());
                }
            }
        },
        false);

    // Animation 재생속도
    world->Each<SkeletalMesh, AnimationClip>(
        [this](Entity* ent,
            ComponentHandle<SkeletalMesh> mesh,
            ComponentHandle<AnimationClip> aniClip)
        {
            if (mesh->IsBindGraphics() && aniClip->IsChangedAnimation())
            {
                if (!aniClip->IsLoop()) // 루프 애니메이션이 아니면 한번만 재생한다
                {
                    // 다음 애니메이션 지정
                    _graphicsEngine->SetDefaultSkeltalAnimation(
                        ent->GetID(), aniClip->GetNextAnimation());

                    // 이번 애니메이션 지정
                    _graphicsEngine->PlaySkeletalAnimationNonLoop(
                        ent->GetID(), aniClip->GetCurrentAnimation());
                }
                else
                {
                    _graphicsEngine->PlaySkeletalAnimation(
                        ent->GetID(), aniClip->GetCurrentAnimation());
                }

                aniClip->SetIsChangedAnimation(false);
            }

            if (mesh->IsBindGraphics() && !aniClip->GetCurrentAnimation().empty())
            {
                _graphicsEngine->UpdateSkeletalAnimationSpeed(ent->GetID(),
                    aniClip->GetCurrentAnimation(),
                    aniClip->GetPlaybackSpeed());
            }
        },
        false);

    // Light Binding
    world->Each<Transform, Light>(
        [this](Entity* ent,
            ComponentHandle<Transform> transform,
            ComponentHandle<Light> light)
        {
            if (light->IsBindGraphics())
            {
                _graphicsEngine->UpdateLight(
                    ent->GetID(), transform->GetWorldMatrix());
            }
        },
        false);


    // UIBinding
    world->Each<UI>(
        [this](Entity* ent, ComponentHandle<UI> ui)
        {
            if (ui->IsBindGraphics())
            {
                if (ui->IsChangedUIInfomation())
                {
                    ui->ChangeUIInfomation(false);

                    ImpGraphics::UIInfo info;
                    info._layer       = ui->GetLayer();
                    info._texturePath = ui->GetTexturePath();
                    info._UIID        = ent->GetID();
                    info._UIPos       = ui->GetUIPosition();
                    _graphicsEngine->SetUI(ent->GetID(), info);
                }

                if (ui->IsChangedXRatio())
                {
                    _graphicsEngine->SetUIXRatio(ent->GetID(), ui->GetXRatio());
                    ui->SetIsChangedXRatio(false);
                }

                if (ui->IsChangedNoise())
                {
                    _graphicsEngine->SetUINoise(ent->GetID(), ui->OnNoise());
                    ui->SetIsChangedNoise(false);
                }

                auto uiPos  = ui->GetFinalUIPosition();
                float alpha = ui->GetAlpha();
                _graphicsEngine->UpdateUI(ent->GetID(), uiPos.x, uiPos.y, alpha);
            }
        },
        false);
}


void ImpGameEngine::RenderingSystem::UpdateLeft()
{
    using namespace ImpEngineModule;
    World* world = _engineModule->GetWorld();

    // Mesh Binding
    world->Each<Transform, Mesh>(
        [this](Entity* ent,
            ComponentHandle<Transform> transform,
            ComponentHandle<Mesh> mesh)
        {
            if (mesh->IsBindGraphics())
            {
                auto type = mesh->GetRenderType();

                if (mesh->IsChangedAlpha() && !mesh->IsOpaque())
                {
                    _graphicsEngine->SetTransparentObjectAlpha(
                        ent->GetID(), mesh->GetAlpha());

                    mesh->SetIsChangedAlpha(false);
                }

                if (mesh->IsChangedEmissionIntensity())
                {
                    _graphicsEngine->SetObjectEmissionCoefficient(
                        ent->GetID(), mesh->GetEmissionIntensity());

                    mesh->SetIsChangedEmissionIntensity(false);
                }

                if (type == Mesh::RenderType::Right ||
                    type == Mesh::RenderType::Clear)
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), _clearRenderMatrix);
                }
                else
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), transform->GetWorldMatrix());
                }
            }
        },
        false);

    // SkeltalMeshBinding
    world->Each<Transform, SkeletalMesh>(
        [this](Entity* ent,
            ComponentHandle<Transform> transform,
            ComponentHandle<SkeletalMesh> mesh)
        {
            if (mesh->IsBindGraphics())
            {
                auto type = mesh->GetRenderType();

                if (mesh->IsChangedAlpha() && !mesh->IsOpaque())
                {
                    _graphicsEngine->SetTransparentObjectAlpha(
                        ent->GetID(), mesh->GetAlpha());

                    mesh->SetIsChangedAlpha(false);
                }

                if (mesh->IsChangedEmissionIntensity())
                {
                    _graphicsEngine->SetObjectEmissionCoefficient(
                        ent->GetID(), mesh->GetEmissionIntensity());

                    mesh->SetIsChangedEmissionIntensity(false);
                }

                if (type == SkeletalMesh::RenderType::Right ||
                    type == SkeletalMesh::RenderType::Clear)
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), _clearRenderMatrix);
                }
                else
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), transform->GetWorldMatrix());
                }
            }
        },
        false);


    // 애니메이션 처리
    world->Each<Transform, SkeletalMesh, AnimationClip>(
        [this](Entity* ent,
            ComponentHandle<Transform> transform,
            ComponentHandle<SkeletalMesh> mesh,
            ComponentHandle<AnimationClip> aniClip)
        {
            if (mesh->IsBindGraphics() && aniClip->IsChangedAnimation())
            {
                if (!aniClip->IsLoop()) // 루프 애니메이션이 아니면 한번만 재생한다
                {
                    // 다음 애니메이션 지정
                    _graphicsEngine->SetDefaultSkeltalAnimation(
                        ent->GetID(), aniClip->GetNextAnimation());

                    // 이번 애니메이션 지정
                    _graphicsEngine->PlaySkeletalAnimationNonLoop(
                        ent->GetID(), aniClip->GetCurrentAnimation());
                }
                else
                {
                    _graphicsEngine->PlaySkeletalAnimation(
                        ent->GetID(), aniClip->GetCurrentAnimation());
                }

                aniClip->SetIsChangedAnimation(false);
            }

            if (mesh->IsBindGraphics() && !aniClip->GetCurrentAnimation().empty())
            {
                _graphicsEngine->UpdateSkeletalAnimationSpeed(ent->GetID(),
                    aniClip->GetCurrentAnimation(),
                    aniClip->GetPlaybackSpeed());
            }
        },
        false);


    // Light Binding
    world->Each<Transform, Light>(
        [this](Entity* ent,
            ComponentHandle<Transform> transform,
            ComponentHandle<Light> light)
        {
            if (light->IsBindGraphics())
            {
                _graphicsEngine->UpdateLight(
                    ent->GetID(), transform->GetWorldMatrix());
            }
        },
        false);


    // UIBinding
    world->Each<UI>(
        [this](Entity* ent, ComponentHandle<UI> ui)
        {
            if (ui->IsBindGraphics())
            {
                if (ui->IsChangedUIInfomation())
                {
                    ui->ChangeUIInfomation(false);

                    ImpGraphics::UIInfo info;
                    info._layer       = ui->GetLayer();
                    info._texturePath = ui->GetTexturePath();
                    info._UIID        = ent->GetID();
                    info._UIPos       = ui->GetUIPosition();
                    _graphicsEngine->SetUI(ent->GetID(), info);
                }

                if (ui->IsChangedNoise())
                {
                    _graphicsEngine->SetUINoise(ent->GetID(), ui->OnNoise());
                    ui->SetIsChangedNoise(false);
                }

                if (ui->IsChangedXRatio())
                {
                    _graphicsEngine->SetUIXRatio(ent->GetID(), ui->GetXRatio());
                    ui->SetIsChangedXRatio(false);
                }

                auto uiPos  = ui->GetFinalUIPosition();
                float alpha = ui->GetAlpha();
                _graphicsEngine->UpdateUI(ent->GetID(), uiPos.x, uiPos.y, alpha);
            }
        },
        false);
}

void ImpGameEngine::RenderingSystem::UpdateRight()
{
    using namespace ImpEngineModule;
    World* world = _engineModule->GetWorld();

    // Mesh Binding
    world->Each<Transform, Mesh>(
        [this](Entity* ent,
            ComponentHandle<Transform> transform,
            ComponentHandle<Mesh> mesh)
        {
            if (mesh->IsBindGraphics())
            {
                auto type = mesh->GetRenderType();

                if (type == Mesh::RenderType::Left || type == Mesh::RenderType::Clear)
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), _clearRenderMatrix);
                }
                else
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), transform->GetWorldMatrix());
                }
            }
        },
        false);


    world->Each<Transform, SkeletalMesh>(
        [this](Entity* ent,
            ComponentHandle<Transform> transform,
            ComponentHandle<SkeletalMesh> mesh)
        {
            if (mesh->IsBindGraphics())
            {
                auto type = mesh->GetRenderType();

                if (type == SkeletalMesh::RenderType::Left ||
                    type == SkeletalMesh::RenderType::Clear)
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), _clearRenderMatrix);
                }
                else
                {
                    _graphicsEngine->UpdateMeshObject(
                        ent->GetID(), transform->GetWorldMatrix());
                }
            }
        },
        false);

    // 애니메이션 처리
    //world->Each<Transform, SkeletalMesh, AnimationClip>(
    //    [this](Entity* ent,
    //        ComponentHandle<Transform> transform,
    //        ComponentHandle<SkeletalMesh> mesh,
    //        ComponentHandle<AnimationClip> aniClip)
    //    {
    //        if (mesh->IsBindGraphics() && aniClip->IsChangedAnimation())
    //        {
    //            _graphicsEngine->PlaySkeletalAnimation(
    //                ent->GetID(), aniClip->GetCurrentAnimation());

    //            aniClip->SetIsChangedAnimation(false);
    //        }
    //    },
    //    false);
}

void ImpGameEngine::RenderingSystem::Finalize()
{
    using namespace ImpEngineModule;

    EventManager* eventMgr = _engineModule->GetEventManager();
    eventMgr->UnSubScribe<Event::OnEntityCreated>(this);
    eventMgr->UnSubScribe<Event::OnEntityStarted>(this);
    eventMgr->UnSubScribe<Event::OnEntityDestroyed>(this);
    eventMgr->UnSubScribe<Event::LoadCubeMap>(this);
}

void ImpGameEngine::RenderingSystem::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::OnEntityCreated& event)
{
}


void ImpGameEngine::RenderingSystem::LoadAnimation(ImpEngineModule::Entity* ent)
{
    auto aniClip = ent->GetComponent<ImpEngineModule::AnimationClip>();

    const auto& animations = aniClip->GetAnimations();

    static int count = 0;
    ++count;
    if (count >= 3)
    {
        return;
    }   

    for (const auto& animationPath : animations)
    {
        if (!animationPath.empty())
            _graphicsEngine->AddSkeletalAnimation(ent->GetID(), animationPath);
    }

    const auto& currentAni = aniClip->GetCurrentAnimation();

    if (currentAni.empty()) return;
    _graphicsEngine->PlaySkeletalAnimation(ent->GetID(), currentAni);
}

void ImpGameEngine::RenderingSystem::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::LoadedWorld& event)
{
    UnloadEntityResources();

   

    LoadEntityResources();
}

void ImpGameEngine::RenderingSystem::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::LoadCubeMap& event)
{
    _graphicsEngine->SetSkyBox(event.path);
}

void ImpGameEngine::RenderingSystem::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::OnEntityStarted& event)
{
    using namespace ImpEngineModule;

    Entity* ent = event.entity;

    // 그래픽스 엔진에 필요한 정보를 바인딩한다.
    ImpToolModule::Bind::Rebind(ent, _graphicsEngine);

    // 애니메이션을 가지는 경우
    if (ent->HasComponent<AnimationClip>())
    {
        LoadAnimation(ent);
    }
}

void ImpGameEngine::RenderingSystem::Receive(ImpEngineModule::World* world,
    const ImpEngineModule::Event::OnEntityDestroyed& event)
{
    using namespace ImpEngineModule;

    // 그래픽스 엔진에 소멸정보를 전달한다.
    Entity* ent = event.entity;

    // 메쉬 바인드 해제
    if (ent->HasComponent<Mesh>() && ent->GetComponent<Mesh>()->IsBindGraphics())
    {
        _graphicsEngine->DeleteMeshObject(ent->GetID());
    }

    // 스켈레탈 메쉬 해제
    if (ent->HasComponent<SkeletalMesh>() &&
        ent->GetComponent<SkeletalMesh>()->IsBindGraphics())
    {
        //_graphicsEngine->DeleteMeshObject(ent->GetID());
    }

    // 라이트 바인드 해제
    if (ent->HasComponent<Light>() && ent->GetComponent<Light>()->IsBindGraphics())
    {
        _graphicsEngine->DeleteLight(ent->GetID());
    }

    // UI 해제
    if (ent->HasComponent<UI>() && ent->GetComponent<UI>()->IsBindGraphics())
    {
        _graphicsEngine->DeleteUI(ent->GetID());
    }
}