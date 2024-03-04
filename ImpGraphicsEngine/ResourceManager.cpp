#include "pch.h"
#include "ResourceManager.h"

#include "ImpLoader.h"
#include "../ImpFBXLoader/GetLoader.h"
#include "../ImpFBXLoader/IFBXLoader.h"
#include "../ImpASELoader/IASELoader.h"

#include "ImpAnimationPlayer.h"

using namespace ImpGraphics;

ImpGraphics::ResourceManager::ResourceManager(ImpDevice* device, ImpAnimationPlayer* animationPlayer) :_resources(), 
	_device(device),
	_impLoader(nullptr),
	_fbxLoader(nullptr),
	_aseLoader(nullptr)
{
	_impLoader = new ImpLoader;

	FBXLoad::GetLoader GetLoader;
	_fbxLoader = GetLoader.GetFBXLoader();

	_aseLoader = ASELoader::ASELoaderExpoter::GetLoader();

	_geometryMaker = new GeometryMaker;

	_animationPlayer = animationPlayer;
}

ResourceManager::~ResourceManager()
{
	delete _geometryMaker;
	delete _impLoader;
	// temp
	delete _fbxLoader;
	ASELoader::ASELoaderExpoter::DeleteLoader();
}
