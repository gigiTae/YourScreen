#include "GetLoader.h"
#include "FBXLoader.h"
#include "IFBXLoader.h"

FBXLoad::IFBXLoader* FBXLoad::GetLoader::GetFBXLoader()
{
	return new FBXLoad::FbxLoader();
}