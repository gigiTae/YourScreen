#pragma once
#include "ResourceManager.h"
#include "Resource.h"

// IObject
#include "MeshObject.h"
#include "QuadObject.h"
#include "LineListObject.h"
#include "SkyBoxObject.h"
#include "AnimationObject.h"
#include "SkinnedMeshObject.h"
#include "PBRMeshObject.h"
#include "BloomQuadObject.h"

#include "ForwardObject.h"
#include "ForwardOITQuadObject.h"
// IBoudningVolumeObject
#include "BoundingVolumeObject.h"
// IUIObject
#include "UIObject.h"

namespace ImpGraphics
{
	class IObject;

	class ResourceManager;

	struct MaterialPath;

	class Builder
	{
	public:
		Builder(ResourceManager* resourceManager)
			: _resourceManager(resourceManager) {}
		~Builder() {};

		template <typename ObjectType, typename... Args>
		struct CreateObject;

		template <typename... Args>
		struct CreateObject<MeshObject, Args...>
		{
			IObject* operator()(Builder* builder, Args... args)
			{
				return builder->CreateMeshObject(args...);
			}
		};

		template <typename... Args>
		struct CreateObject<LineListObject, Args...>
		{
			IObject* operator()(Builder* builder, Args... args)
			{
				return builder->CreateLineListObject(args...);
			}
		};

		template <typename... Args>
		struct CreateObject<QuadObject, Args...>
		{
			IObject* operator()(Builder* builder, Args... args)
			{
				return builder->CreateQuadObject(args...);
			}
		};

		template <typename... Args>
		struct CreateObject<BloomQuadObject, Args...>
		{
			IObject* operator()(Builder* builder, Args... args)
			{
				return builder->CreateBloomQuadObject(args...);
			}
		};

		template <typename... Args>
		struct CreateObject<SkyBoxObject, Args...>
		{
			IObject* operator()(Builder* builder, Args... args)
			{
				return builder->CreateSkyBoxObject(args...);
			}
		};

		template <typename... Args>
		struct CreateObject<AnimationObject, Args...>
		{
			IObject* operator()(Builder* builder, Args... args)
			{
				return builder->CreateAnimationObject(args...);
			}
		};

		template <typename... Args>
		struct CreateObject<SkinnedMeshObject, Args...>
		{
			IObject* operator()(Builder* builder, Args... args)
			{
				return builder->CreateSkinnedMeshObject(args...);
			}
		};

		template<typename... Args>
        struct CreateObject<PBRMeshObject, Args...>
        {
            IObject* operator()(Builder* builder, Args... args)
            {
                return builder->CreatePBRMeshObject(args...);
            }
        };

		template<typename... Args>
        struct CreateObject<ForwardObject, Args...>
        {
            IObject* operator()(Builder* builder, Args... args)
            {
                return builder->CreateForwardObject(args...);
            }
        };

		template<typename... Args>
        struct CreateObject<ForwardOITQuadObject, Args...>
        {
            IObject* operator()(Builder* builder, Args... args)
            {
                return builder->CreateForwardOITQuadObject(args...);
            }
        };

		/// <summary>
		///  바운딩 볼륨
		/// </summary>
		/// <typeparam name="...Args"></typeparam>
		template <typename... Args>
		struct CreateObject<BoundingVolumeObject, Args...>
		{
			IBoundingVolumeObject* operator()(Builder* builder, Args... args)
			{
				return builder->CreateBoundingVolumeObject(args...);
			}
		};

		/// <summary>
		/// UI
		/// </summary>
		/// <typeparam name="...Args"></typeparam>
		template<typename... Args>
        struct CreateObject<UIObject, Args...>
        {
            IUIObject* operator()(Builder* builder, Args... args)
            {
                return builder->CreateUIObject(args...);
            }
        };

		/// 가장 바깥
		template <typename ObjectType, typename... Args>
		IObject* CreateIObject(Args... args)
		{
			return CreateObject<ObjectType, Args...>()(this, args...);
		}

		template <typename ObjectType, typename... Args>
		IBoundingVolumeObject* CreateIBoundingVolumeObject(Args... args)
		{
			return CreateObject<ObjectType, Args...>()(this, args...);
		}

		template<typename ObjectType, typename... Args>
        IUIObject* CreateIUIObject(Args... args)
        {
            return CreateObject<ObjectType, Args...>()(this, args...);
        }

		MeshObject* CreateMeshObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath, std::wstring texturePath = L"");
		PBRMeshObject* CreatePBRMeshObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath, MaterialPath& material);
		ForwardObject* CreateForwardObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath, MaterialPath& material);
		ForwardOITQuadObject* CreateForwardOITQuadObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath);
		AnimationObject* CreateAnimationObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath, std::wstring texturePath = L"");
		SkyBoxObject* CreateSkyBoxObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath, std::wstring texturePath);
		QuadObject* CreateQuadObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath);
		BloomQuadObject* CreateBloomQuadObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath);
		SkinnedMeshObject* CreateSkinnedMeshObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath, MaterialPath& material, size_t objectID);

		BoundingVolumeObject* CreateBoundingVolumeObject(Vector3 center, Vector3 length);
		BoundingVolumeObject* CreateBoundingVolumeObject(Vector3 center, float radius);

		UIObject* CreateUIObject(Vector4 uiPos, DirectX::XMFLOAT2 screenSize, size_t layer, std::wstring texturePath);

		// 모양이 정해진 imp로 읽어오는 파일
		LineListObject* CreateLineListObject(std::wstring meshPath, std::wstring vertexShaderPath, std::wstring pixelShaderPath);
		
	private:
		ResourceManager* _resourceManager;
 	};
}