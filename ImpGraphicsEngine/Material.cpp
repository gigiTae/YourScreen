#include "pch.h"
#include "Material.h"
#include "Resource.h"
#include "ResourceManager.h"

ImpGraphics::Material::Material(ResourceManager* resourceManager,
                                        MaterialPath& paths)
{
    if (paths.baseColor != L"")
    {
        _baseColor = resourceManager->GetResource<Texture>(paths.baseColor);
    }
    if (paths.metallicRoughness != L"")
    {
        _MR = resourceManager->GetResource<Texture>(paths.metallicRoughness);
    }
    if (paths.emission != L"")
    {
        _emission = resourceManager->GetResource<Texture>(paths.emission);
    }
    if (paths.emission != L"")
    {
        _normalMap = resourceManager->GetResource<Texture>(paths.normalMap);
    }
}

ImpGraphics::Material::~Material()
{

}