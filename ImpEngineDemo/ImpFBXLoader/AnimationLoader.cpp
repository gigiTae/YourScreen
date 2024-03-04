#include "AnimationLoader.h"

#include <cassert>

namespace FBXLoad
{
AnimationLoader::AnimationLoader(FbxManager* manager)
    : _manager(manager)
{
    assert(manager);
}

ImpStructure::ImpAnimationObject* AnimationLoader::GetAnimationObject(
    /*const char* file*/)
{
    return nullptr;
}

std::vector<ImpStructure::Bone*> AnimationLoader::GetBoneList()
{
    return std::vector<ImpStructure::Bone*>();
}

void AnimationLoader::DisplayAnimation(FbxScene* pScene)
{
    int i{ 0 };
    unsigned int objectCount = pScene->GetSrcObjectCount<FbxAnimStack>();
    for (i = 0; i < objectCount; i++)
    {
        FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

        FBXSDK_printf("Animation Stack Name: %s\n", (char*)lAnimStack->GetName());

        DisplayAnimation(lAnimStack, pScene->GetRootNode());
    }
}

void AnimationLoader::DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
{
    int l;
    int nbAnimLayers  = pAnimStack->GetMemberCount<FbxAnimLayer>();
    int nbAudioLayers = pAnimStack->GetMemberCount<FbxAudioLayer>();
    FbxString lOutputString;

    lOutputString = "   contains ";
    if (nbAnimLayers == 0 && nbAudioLayers == 0)
        lOutputString += "no layers";

    if (nbAnimLayers)
    {
        lOutputString += nbAnimLayers;
        lOutputString += " Animation Layer";
        if (nbAnimLayers > 1)
            lOutputString += "s";
    }

    if (nbAudioLayers)
    {
        if (nbAnimLayers)
            lOutputString += " and ";

        lOutputString += nbAudioLayers;
        lOutputString += " Audio Layer";
        if (nbAudioLayers > 1)
            lOutputString += "s";
    }
    lOutputString += "\n\n";
    FBXSDK_printf(lOutputString);

    for (l = 0; l < nbAnimLayers; l++)
    {
        FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

        lOutputString = "AnimLayer ";
        lOutputString += l;
        lOutputString += "\n";
        FBXSDK_printf(lOutputString);

        DisplayAnimation(lAnimLayer, pNode, isSwitcher);
    }

    //for (l = 0; l < nbAudioLayers; l++)
    //{
    //    FbxAudioLayer* lAudioLayer = pAnimStack->GetMember<FbxAudioLayer>(l);
    //
    //    lOutputString = "AudioLayer ";
    //    lOutputString += l;
    //    lOutputString += "\n";
    //    FBXSDK_printf(lOutputString);
    //
    //    LoadAnimation(lAudioLayer, isSwitcher);
    //    FBXSDK_printf("\n");
    //}
}

void AnimationLoader::DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
{
    int lModelCount;
    FbxString lOutputString;

    FBXSDK_printf("     Node Name: %s", pNode->GetName());
    unsigned int NodeChiledCount = pNode->GetChildCount();
    FBXSDK_printf("( Node Chiled : %d )\n", NodeChiledCount);

    LoadChannels(pNode, pAnimLayer, isSwitcher);
    FBXSDK_printf("\n");

    for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
        DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
    }
}

/*
void AnimationLoader::LoadAnimation(FbxAudioLayer* pAudioLayer, bool isSwitcher)
{
    int lClipCount;
    FbxString lOutputString;

    lClipCount = pAudioLayer->GetMemberCount<FbxAudio>();

    lOutputString = "     Name: ";
    lOutputString += pAudioLayer->GetName();
    lOutputString += "\n\n";
    lOutputString += "     Nb Audio Clips: ";
    lOutputString += lClipCount;
    FBXSDK_printf(lOutputString.Buffer());

    for (int i = 0; i < lClipCount; i++)
    {
        FbxAudio* lClip = pAudioLayer->GetMember<FbxAudio>(i);
        lOutputString   = "        Clip[";
        lOutputString += i;
        lOutputString += "]:\t";
        lOutputString += lClip->GetName();
        // DisplayString(lOutputString.Buffer());
    }
}
*/

void AnimationLoader::LoadChannels(FbxNode* pNode,
                                      FbxAnimLayer* pAnimLayer,
                                      bool isSwitcher)
{
    FbxAnimCurve* lAnimCurve = NULL;

    // Display general curves.
    if (!isSwitcher)
    {
        lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer,
                                                    FBXSDK_CURVENODE_COMPONENT_X);
        if (lAnimCurve)
        {
            FBXSDK_printf("        TX\n");
            LoadCurveKeys(lAnimCurve);
        }
        lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer,
                                                    FBXSDK_CURVENODE_COMPONENT_Y);
        if (lAnimCurve)
        {
            FBXSDK_printf("        TY\n");
            LoadCurveKeys(lAnimCurve);
        }
        lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer,
                                                    FBXSDK_CURVENODE_COMPONENT_Z);
        if (lAnimCurve)
        {
            FBXSDK_printf("        TZ\n");
            LoadCurveKeys(lAnimCurve);
        }

        lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer,
                                                 FBXSDK_CURVENODE_COMPONENT_X);
        if (lAnimCurve)
        {
            FBXSDK_printf("        RX\n");
            LoadCurveKeys(lAnimCurve);
        }
        lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer,
                                                 FBXSDK_CURVENODE_COMPONENT_Y);
        if (lAnimCurve)
        {
            FBXSDK_printf("        RY\n");
            LoadCurveKeys(lAnimCurve);
        }
        lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer,
                                                 FBXSDK_CURVENODE_COMPONENT_Z);
        if (lAnimCurve)
        {
            FBXSDK_printf("        RZ\n");
            LoadCurveKeys(lAnimCurve);
        }

        lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer,
                                                FBXSDK_CURVENODE_COMPONENT_X);
        if (lAnimCurve)
        {
            FBXSDK_printf("        SX\n");
            LoadCurveKeys(lAnimCurve);
        }
        lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer,
                                                FBXSDK_CURVENODE_COMPONENT_Y);
        if (lAnimCurve)
        {
            FBXSDK_printf("        SY\n");
            LoadCurveKeys(lAnimCurve);
        }
        lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer,
                                                FBXSDK_CURVENODE_COMPONENT_Z);
        if (lAnimCurve)
        {
            FBXSDK_printf("        SZ\n");
            LoadCurveKeys(lAnimCurve);
        }
    }

    // Display curves specific to a light or marker.
    FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

    if (lNodeAttribute)
    {
        lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer,
                                                    FBXSDK_CURVENODE_COLOR_RED);
        if (lAnimCurve)
        {
            FBXSDK_printf("        Red\n");
            LoadCurveKeys(lAnimCurve);
        }
        lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer,
                                                    FBXSDK_CURVENODE_COLOR_GREEN);
        if (lAnimCurve)
        {
            FBXSDK_printf("        Green\n");
            LoadCurveKeys(lAnimCurve);
        }
        lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer,
                                                    FBXSDK_CURVENODE_COLOR_BLUE);
        if (lAnimCurve)
        {
            FBXSDK_printf("        Blue\n");
            LoadCurveKeys(lAnimCurve);
        }

        // Display curves specific to a light.
        FbxLight* light = pNode->GetLight();
        if (light)
        {
            lAnimCurve = light->Intensity.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Intensity\n");
                LoadCurveKeys(lAnimCurve);
            }

            lAnimCurve = light->OuterAngle.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Outer Angle\n");
                LoadCurveKeys(lAnimCurve);
            }

            lAnimCurve = light->Fog.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Fog\n");
                LoadCurveKeys(lAnimCurve);
            }
        }

        // Display curves specific to a camera.
        FbxCamera* camera = pNode->GetCamera();
        if (camera)
        {
            lAnimCurve = camera->FieldOfView.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Field of View\n");
                LoadCurveKeys(lAnimCurve);
            }

            lAnimCurve = camera->FieldOfViewX.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Field of View X\n");
                LoadCurveKeys(lAnimCurve);
            }

            lAnimCurve = camera->FieldOfViewY.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Field of View Y\n");
                LoadCurveKeys(lAnimCurve);
            }

            lAnimCurve = camera->OpticalCenterX.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Optical Center X\n");
                LoadCurveKeys(lAnimCurve);
            }

            lAnimCurve = camera->OpticalCenterY.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Optical Center Y\n");
                LoadCurveKeys(lAnimCurve);
            }

            lAnimCurve = camera->Roll.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Roll\n");
                LoadCurveKeys(lAnimCurve);
            }
        }

        // Display curves specific to a geometry.
        if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
            lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
            lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
        {
            FbxGeometry* lGeometry = (FbxGeometry*)lNodeAttribute;

            int lBlendShapeDeformerCount = lGeometry->GetDeformerCount(
                FbxDeformer::eBlendShape);
            for (int lBlendShapeIndex = 0;
                 lBlendShapeIndex < lBlendShapeDeformerCount;
                 ++lBlendShapeIndex)
            {
                FbxBlendShape* lBlendShape = (FbxBlendShape*)lGeometry
                                                 ->GetDeformer(lBlendShapeIndex,
                                                               FbxDeformer::eBlendShape);

                int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
                for (int lChannelIndex = 0; lChannelIndex < lBlendShapeChannelCount;
                     ++lChannelIndex)
                {
                    FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(
                        lChannelIndex);
                    const char* lChannelName = lChannel->GetName();

                    lAnimCurve = lGeometry->GetShapeChannel(lBlendShapeIndex,
                                                            lChannelIndex,
                                                            pAnimLayer,
                                                            true);
                    if (lAnimCurve)
                    {
                        FbxString tmp = FbxString("        Shape");
                        tmp += lChannelName;
                        FBXSDK_printf(tmp.Buffer());
                        LoadCurveKeys(lAnimCurve);
                    }
                }
            }
        }
    }

    // Display curves specific to properties
    FbxProperty lProperty = pNode->GetFirstProperty();
    while (lProperty.IsValid())
    {
        if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
        {
            FbxString lFbxFCurveNodeName = lProperty.GetName();
            FbxAnimCurveNode* lCurveNode = lProperty.GetCurveNode(pAnimLayer);

            if (!lCurveNode)
            {
                lProperty = pNode->GetNextProperty(lProperty);
                continue;
            }

            FbxDataType lDataType = lProperty.GetPropertyDataType();
            if (lDataType.GetType() == eFbxBool ||
                lDataType.GetType() == eFbxDouble ||
                lDataType.GetType() == eFbxFloat || lDataType.GetType() == eFbxInt)
            {
                FbxString lMessage;

                lMessage = "        Property ";
                lMessage += lProperty.GetName();
                if (lProperty.GetLabel().GetLen() > 0)
                {
                    lMessage += " (Label: ";
                    lMessage += lProperty.GetLabel();
                    lMessage += ")";
                };

                FBXSDK_printf(lMessage.Buffer());

                for (int c = 0; c < lCurveNode->GetCurveCount(0U); c++)
                {
                    lAnimCurve = lCurveNode->GetCurve(0U, c);
                    if (lAnimCurve)
                        LoadCurveKeys(lAnimCurve);
                }
            }
            else if (lDataType.GetType() == eFbxDouble3 ||
                     lDataType.GetType() == eFbxDouble4 ||
                     lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT))
            {
                char* lComponentName1 = (lDataType.Is(FbxColor3DT) ||
                                         lDataType.Is(FbxColor4DT))
                                            ? (char*)FBXSDK_CURVENODE_COLOR_RED
                                            : (char*)"X";
                char* lComponentName2 = (lDataType.Is(FbxColor3DT) ||
                                         lDataType.Is(FbxColor4DT))
                                            ? (char*)FBXSDK_CURVENODE_COLOR_GREEN
                                            : (char*)"Y";
                char* lComponentName3 = (lDataType.Is(FbxColor3DT) ||
                                         lDataType.Is(FbxColor4DT))
                                            ? (char*)FBXSDK_CURVENODE_COLOR_BLUE
                                            : (char*)"Z";
                FbxString lMessage;

                lMessage = "        Property ";
                lMessage += lProperty.GetName();
                if (lProperty.GetLabel().GetLen() > 0)
                {
                    lMessage += " (Label: ";
                    lMessage += lProperty.GetLabel();
                    lMessage += ")";
                }
                FBXSDK_printf(lMessage.Buffer());

                for (int c = 0; c < lCurveNode->GetCurveCount(0U); c++)
                {
                    lAnimCurve = lCurveNode->GetCurve(0U, c);
                    if (lAnimCurve)
                    {
                        FBXSDK_printf("        Component %s", lComponentName1);
                        LoadCurveKeys(lAnimCurve);
                    }
                }

                for (int c = 0; c < lCurveNode->GetCurveCount(1U); c++)
                {
                    lAnimCurve = lCurveNode->GetCurve(1U, c);
                    if (lAnimCurve)
                    {
                        FBXSDK_printf("        Component %s", lComponentName2);
                        LoadCurveKeys(lAnimCurve);
                    }
                }

                for (int c = 0; c < lCurveNode->GetCurveCount(2U); c++)
                {
                    lAnimCurve = lCurveNode->GetCurve(2U, c);
                    if (lAnimCurve)
                    {
                        FBXSDK_printf("        Component %s", lComponentName3);
                        LoadCurveKeys(lAnimCurve);
                    }
                }
            }
            else if (lDataType.GetType() == eFbxEnum)
            {
                FbxString lMessage;

                lMessage = "        Property ";
                lMessage += lProperty.GetName();
                if (lProperty.GetLabel().GetLen() > 0)
                {
                    lMessage += " (Label: ";
                    lMessage += lProperty.GetLabel();
                    lMessage += ")";
                };
                FBXSDK_printf(lMessage.Buffer());

                for (int c = 0; c < lCurveNode->GetCurveCount(0U); c++)
                {
                    lAnimCurve = lCurveNode->GetCurve(0U, c);
                    if (lAnimCurve)
                        LoadListCurveKeys(lAnimCurve, &lProperty);
                }
            }
        }

        lProperty = pNode->GetNextProperty(lProperty);
    } // while
}

void AnimationLoader::LoadCurveKeys(FbxAnimCurve* pCurve)
{
    static const char* interpolation[] = { "?", "constant", "linear", "cubic" };
    static const char* constantMode[]  = { "?", "Standard", "Next" };
    static const char* cubicMode[]     = { "?",    "Auto",  "Auto break", "Tcb",
                                           "User", "Break", "User break" };
    static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

    FbxTime lKeyTime;
    float lKeyValue;
    char lTimeString[256];
    FbxString lOutputString;
    int lCount;

    int lKeyCount = pCurve->KeyGetCount();

    for (lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += " [ ";
        lOutputString += interpolation[InterpolationFlagToIndex(
            pCurve->KeyGetInterpolation(lCount))];
        if ((pCurve->KeyGetInterpolation(lCount) &
             FbxAnimCurveDef::eInterpolationConstant) ==
            FbxAnimCurveDef::eInterpolationConstant)
        {
            lOutputString += " | ";
            lOutputString += constantMode[ConstantmodeFlagToIndex(
                pCurve->KeyGetConstantMode(lCount))];
        }
        else if ((pCurve->KeyGetInterpolation(lCount) &
                  FbxAnimCurveDef::eInterpolationCubic) ==
                 FbxAnimCurveDef::eInterpolationCubic)
        {
            lOutputString += " | ";
            lOutputString += cubicMode[TangentmodeFlagToIndex(
                pCurve->KeyGetTangentMode(lCount))];
            lOutputString += " | ";
            lOutputString += tangentWVMode[TangentweightFlagToIndex(
                pCurve->KeyGet(lCount).GetTangentWeightMode())];
            lOutputString += " | ";
            lOutputString += tangentWVMode[TangentVelocityFlagToIndex(
                pCurve->KeyGet(lCount).GetTangentVelocityMode())];
        }
        lOutputString += " ]";
        lOutputString += "\n";
        FBXSDK_printf(lOutputString);
    }
}

void AnimationLoader::LoadListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
{
    FbxTime lKeyTime;
    int lKeyValue;
    char lTimeString[256];
    FbxString lListValue;
    FbxString lOutputString;
    int lCount;

    int lKeyCount = pCurve->KeyGetCount();

    for (lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += " (";
        lOutputString += pProperty->GetEnumValue(lKeyValue);
        lOutputString += ")";

        lOutputString += "\n";
        FBXSDK_printf(lOutputString);
    }
}

int AnimationLoader::InterpolationFlagToIndex(int flags)
{
    if ((flags & FbxAnimCurveDef::eInterpolationConstant) ==
        FbxAnimCurveDef::eInterpolationConstant)
        return 1;
    if ((flags & FbxAnimCurveDef::eInterpolationLinear) ==
        FbxAnimCurveDef::eInterpolationLinear)
        return 2;
    if ((flags & FbxAnimCurveDef::eInterpolationCubic) ==
        FbxAnimCurveDef::eInterpolationCubic)
        return 3;
    return 0;
}

int AnimationLoader::ConstantmodeFlagToIndex(int flags)
{
    if ((flags & FbxAnimCurveDef::eConstantStandard) ==
        FbxAnimCurveDef::eConstantStandard)
        return 1;
    if ((flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext)
        return 2;
    return 0;
}

int AnimationLoader::TangentmodeFlagToIndex(int flags)
{
    if ((flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto)
        return 1;
    if ((flags & FbxAnimCurveDef::eTangentAutoBreak) ==
        FbxAnimCurveDef::eTangentAutoBreak)
        return 2;
    if ((flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB)
        return 3;
    if ((flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser)
        return 4;
    if ((flags & FbxAnimCurveDef::eTangentGenericBreak) ==
        FbxAnimCurveDef::eTangentGenericBreak)
        return 5;
    if ((flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak)
        return 6;
    return 0;
}

int AnimationLoader::TangentweightFlagToIndex(int flags)
{
    if ((flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone)
        return 1;
    if ((flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight)
        return 2;
    if ((flags & FbxAnimCurveDef::eWeightedNextLeft) ==
        FbxAnimCurveDef::eWeightedNextLeft)
        return 3;
    return 0;
}

int AnimationLoader::TangentVelocityFlagToIndex(int flags)
{
    if ((flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone)
        return 1;
    if ((flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight)
        return 2;
    if ((flags & FbxAnimCurveDef::eVelocityNextLeft) ==
        FbxAnimCurveDef::eVelocityNextLeft)
        return 3;
    return 0;
}


} // namespace FBXLoad