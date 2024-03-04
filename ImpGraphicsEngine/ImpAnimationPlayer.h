#pragma once
#include <vector>
#include <string>

namespace ImpStructure { struct ImpAnimationObject; }

namespace ImpGraphics
{
	class SkinnedMeshObject;

	class ImpAnimationPlayer
	{
	public:
		~ImpAnimationPlayer();

		void Initialize();
		void Update(float deltaTime);

		void AddObject(const std::wstring& animationName, ImpStructure::ImpAnimationObject* object);
        void UpdatePlaySpeed(const std::wstring& animationName, float speed);
        void PlayLoopAnimation(size_t objectID, const std::wstring& animationName);
        void PlayAnimation(size_t objectID, const std::wstring& animationName);
		
		void AddInterpolationObject(const std::wstring& animationName, SkinnedMeshObject* skinnedObject);
        void SetDefaultAnimation(size_t objectID, const std::wstring& animationName);

		void SetSkinningObject(size_t objectID, SkinnedMeshObject* skinnedObject);

		std::wstring GetAnimation(size_t objectID);

		void DeleteObject();

	private:
		float _time;
        std::unordered_map<std::wstring, ImpStructure::ImpAnimationObject*> _animationObjects;
        std::unordered_map<std::wstring, size_t> _playTime;  // 재생중인 키 프레임
		std::unordered_map<std::wstring, bool> _isPlayings; // 
		std::unordered_map<std::wstring, float> _playDeltaTime;
		std::unordered_map<std::wstring, float> _playSpeed;
		std::unordered_map<std::wstring, bool> _isOne;

		std::unordered_map<size_t, std::wstring> _playingAnimation; // 현재 플레이중인 애니메이션
		std::unordered_map<size_t, std::wstring> _defaultAnimation; 

		std::unordered_map<size_t, SkinnedMeshObject*> _skinningAnimationObject;

		std::unordered_map<std::wstring, SkinnedMeshObject*> _interpolating;
		std::vector<ImpStructure::ImpAnimationObject*> _animationObject;
	};
}