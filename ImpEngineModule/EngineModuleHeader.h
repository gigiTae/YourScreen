#pragma once

#ifndef IMP_ENGIME_MODULE_HEADER
#define IMP_ENGIME_MODULE_HEADER

#include "EngineModulePCH.h"
#include "TypeIndex.h"
#include "NameCheck.h"

#include "IEntity.h"
#include "IWorld.h"

#include "World.h"
#include "Entity.h"
#include "Component.h"
#include "Resource.h"
#include "EntityResource.h"
#include "CubeMap.h"

/// Internal
#include "EntityComponentIterator.h"
#include "EntityComponentView.h"
#include "EntityIterator.h"
#include "EntityView.h"
#include "ComponentHandle.h"

/// Manager
#include "PathManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"

/// Component
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "SkeletalMesh.h"
#include "Light.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Rigidbody.h"
#include "UI.h"
#include "AnimationClip.h"
#include "AudioClip.h"

/// Event
#include "EventSubscriber.h"
#include "CallBackEvent.h"

/// Reflection
#include "QuaternionType.h"
#include "ImpColorType.h"

/// Resource
#include "TagMapResource.h"

/// Physics
#include "Tag.h"
#include "TagMap.h"
#include "Collision.h"

/// EngineModule
#include "EngineModule.h"

#endif //IMP_ENGIME_MODULE_HEADER