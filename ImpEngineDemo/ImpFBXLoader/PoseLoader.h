/// 2024-01-24


#pragma once

#include "../ImpStructure/ImpMesh.h"

#include <fbxsdk.h>
#include <memory>

namespace FBXLoad
{

class PoseLoader
{
public:
    void DisplayPose(FbxScene* pScene);
    void LoadPose(FbxScene* pScene, ImpStructure::ImpAnimationObject& obj);
};

} // namespace FBXLoad