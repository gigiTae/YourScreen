#pragma once
#include "IImp3DRenderer.h"
#include "Builder.h"

namespace ImpGraphics
{
	class ObjectManager;
	class ResourceManager;

	class Factory
	{
	public:
		Factory(ResourceManager* resourceManager);
		~Factory();

		template <typename ObjectType, typename ...types>
		ObjectType* CreateIObject(types... args)
		{
			return dynamic_cast<ObjectType*>(_builder->CreateIObject<ObjectType>(args...));
		}

		template<typename ObjectType, typename ...types>
		ObjectType* CreateBoundingVolumeObject(types... args)
		{
			return dynamic_cast<ObjectType*>(_builder->CreateIBoundingVolumeObject<ObjectType>(args...));
		}

		template<typename ObjectType, typename ...types>
		ObjectType* CreateIUIObject(types... args)
		{
			return dynamic_cast<ObjectType*>(_builder->CreateIUIObject<ObjectType>(args...));
		}

	private:
		Builder* _builder;

		ObjectManager* _objectManager;
		ResourceManager* _resourceManager;
	};
}