#pragma once
#include <memory>
#include "Resource.h"

// 팩토리
// 빌더
// 리소스 매니저
// 로더

// T CreateObject(BuildInfo* info);
// IObjectBase* _newObject = CreateObject<MeshObject*>(buildInfo);
// 일단 책을 읽고 각 객체가 해야 하는 일이 뭐일지 부터 정해보자

namespace FBXLoad { class IFBXLoader; }
namespace ASELoader { class IASELoader; }

namespace ImpGraphics
{
	class ImpDevice;
	class ImpLoader;

	class GeometryMaker;

	class ImpAnimationPlayer;

	class ResourceManager 
	{
	public:
		ResourceManager(ImpDevice* device, ImpAnimationPlayer* animationPlayer);
		~ResourceManager();

		//std::shared_ptr<Resource> GetResource(const std::string& key) ;

		// 사용할 셰이더 버텍스, 픽셸
		// 버텍스 버퍼, 인덱스 버퍼

		template<typename ResourceType>
		std::shared_ptr<ResourceType> GetResource(const std::wstring& path)
		{
			std::shared_ptr<Resource> resource = _resources[path].lock();
			std::shared_ptr<ResourceType> resourceTypePtr;
			if (!resource)
			{
				_resources[path] = resource = std::make_shared<ResourceType>(this);
				resourceTypePtr = std::dynamic_pointer_cast<ResourceType>(resource);
				resourceTypePtr->Load(path);
			}
			else
			{
				resourceTypePtr = std::dynamic_pointer_cast<ResourceType>(resource);
			}

			return resourceTypePtr;
		}

		template<typename ResourceType, typename ...Types>
		std::shared_ptr<ResourceType> GetResource(const std::wstring& path, Types... args)
		{
			// sizeof...(args)
			std::shared_ptr<Resource> resource = _resources[path].lock();
			std::shared_ptr<ResourceType> resourceTypePtr;
			if (!resource)
			{
				_resources[path] = resource = std::make_shared<ResourceType>(this);
				resourceTypePtr = std::dynamic_pointer_cast<ResourceType>(resource);
				resourceTypePtr->Load(path, args...);
			}
			else
			{
				resourceTypePtr = std::dynamic_pointer_cast<ResourceType>(resource);
			}

			return resourceTypePtr;
		}

		ImpDevice* GetDevice() { return _device; }
		ImpLoader* GetImpLoader() { return _impLoader; }
		FBXLoad::IFBXLoader* GetFBXLoader() { return _fbxLoader; }
		ASELoader::IASELoader* GetASELoader() { return _aseLoader; }
		GeometryMaker* GetGeometryMaker() { return _geometryMaker; }

		ImpAnimationPlayer* GetAniamtionPlayer() { return _animationPlayer; };
	private:
		std::unordered_map<std::wstring, std::weak_ptr<Resource>> _resources;

		ImpDevice* _device; // DirectX 관련 리소스들 Load용
		// FBX 로더 
		ImpLoader* _impLoader;
		FBXLoad::IFBXLoader* _fbxLoader;
		ASELoader::IASELoader* _aseLoader;
		GeometryMaker* _geometryMaker;

		ImpAnimationPlayer* _animationPlayer;
	};
}