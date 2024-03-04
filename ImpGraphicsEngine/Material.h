#pragma once

#include "IImpGraphicsEngine.h"

namespace ImpGraphics
{
    class Texture;
    class ResourceManager;

    class Material
    {
    public:
        Material(ResourceManager* resourceManager, MaterialPath& paths);
        ~Material();

        std::shared_ptr<Texture> GetBaseColor()
        {
            return _baseColor;
        }
        std::shared_ptr<Texture> GetMetallicRoughness()
        {
            return _MR;
        }
        std::shared_ptr<Texture> GetEmission()
        {
            return _emission;
        }
        std::shared_ptr<Texture> GetNormalMap()
        {
            return _normalMap;
        }

    private:
        std::shared_ptr<Texture> _baseColor;
        std::shared_ptr<Texture> _MR;
        std::shared_ptr<Texture> _emission;
        std::shared_ptr<Texture> _normalMap;
    };
}