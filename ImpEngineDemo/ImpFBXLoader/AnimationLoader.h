/// AnimationLoader.h
///
/// 2024-01-24

#pragma once

#include "../ImpStructure/ImpMesh.h"

#include <fbxsdk.h>

namespace FBXLoad
{

class AnimationLoader
{
public:
    AnimationLoader(FbxManager* manager);

public:
    ImpStructure::ImpAnimationObject* GetAnimationObject(/*const char* file*/);
    std::vector<ImpStructure::Bone*> GetBoneList();

    void DisplayAnimation(FbxScene* pScene);
private:

    void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher = false);
    void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher = false);
    //void LoadAnimation(FbxAudioLayer* pAudioLayer, bool isSwitcher = false);
    
    void LoadChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, bool isSwitcher);

    void LoadCurveKeys(FbxAnimCurve* pCurve); 
    void LoadListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);

    int InterpolationFlagToIndex(int flags);
    int ConstantmodeFlagToIndex(int flags);
    int TangentmodeFlagToIndex(int flags);
    int TangentweightFlagToIndex(int flags);
    int TangentVelocityFlagToIndex(int flags);


private:
    FbxManager* _manager{ nullptr };
};

} // namespace FBXLoad