// #include "Common.h"
// #include "Loader.h"
// #include "Geometry.h"

#include "FbxLoader.h"
#include "GetLoader.h"
#include "PoseLoader.h"

#include <iostream>

int main()
{
    const char* fileName = "waking.fbx";
    /*
    FBXLoad::IFBXLoader* loader;
    FBXLoad::GetLoader a;

    loader = a.GetFBXLoader();

    auto something1       = loader->GetAnimationObject(fileName);
    */

    ////////////////////////////////////////////////////////////////////
    FBXLoad::FbxLoader loader2;

    loader2.LoadAll(fileName);
    //auto b = loader2.GetMeshAll(fileName);
    //auto c = loader2.GetAnimationObject(fileName);
}