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

        // ������ �׷��Ƚ� ������ ���ε��� ��� ���ε��� �����Ѵ�.
        if (mesh->IsBindGraphics())
        {
            mesh->SetBindGraphics(false);
            graphicsEngine->DeleteMeshObject(ent->GetID());
        }

        // ������Ʈ ��Ȱ��ȭ�̹Ƿ� ���ε� ����
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

        // ���ε��� ������ Ȯ���Ѵ�

        // 1. ��ΰ� ���� ��� ���ε����� �ʴ´�.
        if (meshInfo._meshPath.empty() || meshInfo._pisxelShaderPath.empty() ||
            meshInfo._vertexShaderPath.empty())
        {
            mesh->SetBindGraphics(false);
            return;
        }

        // 2. Ȯ���ڸ��� Ȯ���ϰ� ���� ������ �ʱ�ȭ�Ѵ�.

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


        // 3. �׷��Ƚ������� ���ε��Ѵ�.
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

        // ������ �׷��Ƚ� ������ ���ε��� ��� ���ε��� �����Ѵ�.
        if (mesh->IsBindGraphics())
        {
            mesh->SetBindGraphics(false);
            // graphicsEngine->DeleteMeshObject(ent->GetID());
        }

        // ������Ʈ ��Ȱ��ȭ�̹Ƿ� ���ε� ����
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

        // ���ε��� ������ Ȯ���Ѵ�

        // 1. ��ΰ� ���� ��� ���ε����� �ʴ´�.
        if (meshInfo._meshPath.empty() || meshInfo._pisxelShaderPath.empty() ||
            meshInfo._vertexShaderPath.empty())
        {
            mesh->SetBindGraphics(false);
            return;
        }

        // 2. Ȯ���ڸ��� Ȯ���ϰ� ���� ������ �ʱ�ȭ�Ѵ�.

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

        // 3. �׷��Ƚ������� ���ε��Ѵ�.
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

        // ������ �׷��Ƚ� ������ ���ε��� ��� ���ε��� �����Ѵ�.
        if (light->IsBindGraphics())
        {
            light->SetBindGraphics(false);
            graphicsEngine->DeleteLight(ent->GetID());
        }

        // ������Ʈ ��Ȱ��ȭ�̹Ƿ� ���ε� ����
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

        // �׷��Ƚ� ������ ���ε��Ѵ�.
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

        // ������ �׷��Ƚ� ������ ���ε��� ��� ���ε��� �����Ѵ�.
        if (ui->IsBindGraphics())
        {
            ui->SetBindGraphics(false);
            graphicsEngine->DeleteUI(ent->GetID());
        }

        // ������Ʈ ��Ȱ��ȭ�̹Ƿ� ���ε� ����
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

        // �׷��Ƚ� ������ ���ε��Ѵ�.
        graphicsEngine->AddUI(ent->GetID(), info);
        graphicsEngine->UpdateUI(
            ent->GetID(), info._UIPos.x, info._UIPos.y, ui->GetAlpha());
        ui->SetBindGraphics(true);
    }
}
