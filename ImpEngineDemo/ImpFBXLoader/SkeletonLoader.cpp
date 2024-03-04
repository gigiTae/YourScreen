#include "SkeletonLoader.h"

namespace FBXLoad
{
void SkeletonLoader::DisplaySkeleton(FbxNode* pNode)
{
    FbxSkeleton* lSkeleton = (FbxSkeleton*)pNode->GetNodeAttribute();

    FBXSDK_printf("Skeleton Name: %s\n", (char*)pNode->GetName());

    int nbMetaData = lSkeleton->GetSrcObjectCount<FbxObjectMetaData>();
    if (nbMetaData > 0)
        FBXSDK_printf("    MetaData connections ");

     for (int i = 0; i < nbMetaData; i++)
    {
        FbxObjectMetaData* metaData = lSkeleton->GetSrcObject<FbxObjectMetaData>(i);
        FBXSDK_printf("        Name: %s\n", (char*)metaData->GetName());
    }

    const char* lSkeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };

    FBXSDK_printf("    Type: %s\n", lSkeletonTypes[lSkeleton->GetSkeletonType()]);

    if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimb)
    {
        FBXSDK_printf("    Limb Length: %f\n", lSkeleton->LimbLength.Get());
    }
    else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
    {
        FBXSDK_printf("    Limb Node Size: %f\n", lSkeleton->Size.Get());
    }
    else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eRoot)
    {
        FBXSDK_printf("    Limb Root Size: %f\n", lSkeleton->Size.Get());
    }

    FBXSDK_printf("    Color: %f, %f, %f\n", lSkeleton->GetLimbNodeColor());

    FBXSDK_printf("\n");
}
} // namespace FBXLoad