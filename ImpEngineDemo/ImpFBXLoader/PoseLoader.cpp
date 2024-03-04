#include "PoseLoader.h"

namespace FBXLoad
{
void PoseLoader::DisplayPose(FbxScene* pScene)
{
    int i, j, k, lPoseCount;
    FbxString lName;

    lPoseCount = pScene->GetPoseCount();

    FBXSDK_printf("Pose Count: %d\n", lPoseCount);

    FBXSDK_printf("\n");

    for (i = 0; i < lPoseCount; i++)
    {
        FbxPose* lPose = pScene->GetPose(i);

        lName = lPose->GetName();
        FBXSDK_printf("Pose Name: %s\n", lName.Buffer());

        FBXSDK_printf("    Is a bind pose: %s\n", lPose->IsBindPose() ? "true" : "flase");

        FBXSDK_printf("    Number of items in the pose: %d\n", lPose->GetCount());

        FBXSDK_printf("\n");

        for (j = 0; j < lPose->GetCount(); j++)
        {
            lName = lPose->GetNodeName(j).GetCurrentName();
            FBXSDK_printf("    Item name: %s\n", lName.Buffer());

            if (!lPose->IsBindPose())
            {
                // Rest pose can have local matrix
                FBXSDK_printf("    Is local space matrix: %s\n", lPose->IsLocalMatrix(j)? "true" : "false");
            }
            FBXSDK_printf("    Is local space matrix: %s\n",
                          lPose->IsLocalMatrix(j) ? "true" : "false");

            FBXSDK_printf("    Matrix value: \n");

            FbxString lMatrixValue;

            for (k = 0; k < 4; k++)
            {
                FbxMatrix lMatrix = lPose->GetMatrix(j);
                FbxVector4 lRow   = lMatrix.GetRow(k);
                char lRowValue[1024];

                FBXSDK_sprintf(lRowValue,
                               1024,
                               "%9.4f %9.4f %9.4f %9.4f\n",
                               lRow[0],
                               lRow[1],
                               lRow[2],
                               lRow[3]);
                lMatrixValue += FbxString("        ") + FbxString(lRowValue);
            }

            FBXSDK_printf(lMatrixValue.Buffer());

            FBXSDK_printf("\n");
        }
    }

    lPoseCount = pScene->GetCharacterPoseCount();

    FBXSDK_printf("Character Pose Count: %d\n", lPoseCount);

    for (i = 0; i < lPoseCount; i++)
    {
        FbxCharacterPose* lPose  = pScene->GetCharacterPose(i);
        FbxCharacter* lCharacter = lPose->GetCharacter();

        if (!lCharacter)
            break;

        FBXSDK_printf("Character Pose Name: %s", lCharacter->GetName());

        FbxCharacterLink lCharacterLink;
        FbxCharacter::ENodeId lNodeId = FbxCharacter::eHips;

        while (lCharacter->GetCharacterLink(lNodeId, &lCharacterLink))
        {
            FbxAMatrix& lGlobalPosition = lCharacterLink.mNode->EvaluateGlobalTransform(
                FBXSDK_TIME_ZERO);

            FBXSDK_printf("    Matrix value: ", "");

            FbxString lMatrixValue;

            for (k = 0; k < 4; k++)
            {
                FbxVector4 lRow = lGlobalPosition.GetRow(k);
                char lRowValue[1024];

                FBXSDK_sprintf(lRowValue,
                               1024,
                               "%9.4f %9.4f %9.4f %9.4f\n",
                               lRow[0],
                               lRow[1],
                               lRow[2],
                               lRow[3]);
                lMatrixValue += FbxString("        ") + FbxString(lRowValue);
            }

            FBXSDK_printf(lMatrixValue.Buffer());

            lNodeId = FbxCharacter::ENodeId(int(lNodeId) + 1);
        }
    }
}

void PoseLoader::LoadPose(FbxScene* pScene, ImpStructure::ImpAnimationObject& obj)
{
}

} // namespace FBXLoad