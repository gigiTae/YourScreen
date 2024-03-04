#include "CASEParser.h"

using namespace ASELoader;

ASELoader::IASELoader* ASELoaderExpoter::GetLoader()
{
	if (_loader == nullptr)
	{
		_loader = new CASEParser;
	}
	return _loader;
}

void ASELoaderExpoter::DeleteLoader()
{
	delete _loader;
}

ASELoader::IASELoader* ASELoader::ASELoaderExpoter::_loader;
