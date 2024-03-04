#include "ImpToolModulePCH.h"
#include "Rebind.h"
#include "../ImpGraphicsEngine/ImpGraphicsEngine.h"

void ImpToolModule::Bind::RebindMeshObject(ImpEngineModule::Entity* ent,
    ImpGraphics::IImpGraphicsEngine* graphicsEngine)
{
    using namespace ImpEngineModule;
    using namespace ImpGraphics;

    if (ent->HasComponent<Mesh>())
    {
        Mesh* mesh = ent->GetComponent<Mesh>();

        // 이전에 그래픽스 엔진에 바인딩된 경우 바인딩을 해제한다.
        if (mesh->IsBindGraphics())
        {
            mesh->SetBindGraphics(false);
            graphicsEngine->DeleteMeshObject(ent->GetID());
        }

        // 컴포넌트 비활성화이므로 바인딩 해제
        if (!mesh->IsActive())
        {
            return;
        }

        MeshObjectInfo meshInfo;

        meshInfo._objectID                   = ent->GetID();
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
        graphicsEngine->AddMeshObejct(meshInfo);
        mesh->SetBindGraphics(true);
    }
}


void ImpToolModule::Bind::RebindSkelatalMeshObject(ImpEngineModule::Entity* ent,
    ImpGraphics::IImpGraphicsEngine* graphicsEngine)
{
    using namespace ImpEngineModule;
    using namespace ImpGraphics;

    if (ent->HasComponent<SkeletalMesh>())
    {
        SkeletalMesh* mesh = ent->GetComponent<SkeletalMesh>();

        // 이전에 그래픽스 엔진에 바인딩된 경우 바인딩을 해제한다.
        if (mesh->IsBindGraphics())
        {
            mesh->SetBindGraphics(false);
            // graphicsEngine->DeleteMeshObject(ent->GetID());
        }

        // 컴포넌트 비활성화이므로 바인딩 해제
        if (!mesh->IsActive())
        {
            return;
        }

        MeshObjectInfo meshInfo;

        meshInfo._objectID                   = ent->GetID();
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
        static int count = 0;
        ++count;

        if (count <= 2)
        {
            graphicsEngine->AddSkeletalAnimationObject(meshInfo);
        }

        mesh->SetBindGraphics(true);
    }
}


void ImpToolModule::Bind::RebindLightObject(ImpEngineModule::Entity* ent,
    ImpGraphics::IImpGraphicsEngine* graphicsEngine)
{
    using namespace ImpEngineModule;
    using namespace ImpGraphics;

    if (ent == nullptr)
    {
        return;
    }

    if (ent->HasComponent<Light>())
    {
        Light* light = ent->GetComponent<Light>();

        // 이전에 그래픽스 엔진에 바인딩된 경우 바인딩을 해제한다.
        if (light->IsBindGraphics())
        {
            light->SetBindGraphics(false);
            graphicsEngine->DeleteLight(ent->GetID());
        }

        // 컴포넌트 비활성화이므로 바인딩 해제
        if (!light->IsActive())
        {
            return;
        }

        LightInfo info;

        // 0 Directional, 1 Spot, 2 Point
        switch (light->GetLightType())
        {
            case Light::LightType::Directional:
            {
                info._lightType = 0;
            }
            break;
            case Light::LightType::Spot:
            {
                info._lightType = 1;
            }
            break;
            case Light::LightType::Point:
            {
                info._lightType = 2;
            }
            break;
            default:
                break;
        }

        ImpStructure::ImpColor color = light->GetColor();
        info._color     = Vector4(color.r, color.g, color.b, color.a);
        info._intensity = light->GetIntensity();
        info._range     = light->GetRange();
        info._spot      = light->GetSpotRadius();
        info._lightID   = ent->GetID();

        // 그래픽스 엔진에 바인딩한다.
        graphicsEngine->AddLight(info);
        graphicsEngine->UpdateLight(
            ent->GetID(), ent->GetComponent<Transform>()->GetWorldMatrix());
        light->SetBindGraphics(true);
    }
}


void ImpToolModule::Bind::Rebind(ImpEngineModule::Entity* ent,
    ImpGraphics::IImpGraphicsEngine* graphicsEngine)
{
    if (ent == nullptr)
    {
        return;
    }

    RebindMeshObject(ent, graphicsEngine);

    RebindSkelatalMeshObject(ent, graphicsEngine);

    RebindLightObject(ent, graphicsEngine);

    RebindUIObject(ent, graphicsEngine);
}

void ImpToolModule::Bind::RebindUIObject(ImpEngineModule::Entity* ent,
    ImpGraphics::IImpGraphicsEngine* graphicsEngine)
{

    using namespace ImpEngineModule;
    using namespace ImpGraphics;

    if (ent == nullptr)
    {
        return;
    }

    if (ent->HasComponent<UI>())
    {
        UI* ui = ent->GetComponent<UI>();

        // 이전에 그래픽스 엔진에 바인딩된 경우 바인딩을 해제한다.
        if (ui->IsBindGraphics())
        {
            ui->SetBindGraphics(false);
            graphicsEngine->DeleteUI(ent->GetID());
        }

        // 컴포넌트 비활성화이므로 바인딩 해제
        if (!ui->IsActive())
        {
            return;
        }

        UIInfo info;

        info._UIID        = ent->GetID();
        info._layer       = ui->GetLayer();
        info._texturePath = ui->GetTexturePath();
        info._UIPos       = ui->GetUIPosition();

        if (info._texturePath.empty())
        {
            return;
        }

        // 그래픽스 엔진에 바인딩한다.
        graphicsEngine->AddUI(ent->GetID(), info);
        graphicsEngine->UpdateUI(
            ent->GetID(), info._UIPos.x, info._UIPos.y, ui->GetAlpha());
        ui->SetBindGraphics(true);
    }
}
