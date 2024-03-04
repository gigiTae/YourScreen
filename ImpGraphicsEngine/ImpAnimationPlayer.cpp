#include "pch.h"
#include "ImpAnimationPlayer.h"

#include "../ImpStructure/ImpMesh.h"

//temp
#include <algorithm>

#include "SkinnedMeshObject.h"

using namespace ImpGraphics;

ImpAnimationPlayer::~ImpAnimationPlayer()
{
	for (const auto& object : _animationObjects)
	{
		delete object.second;
	}
}

void ImpGraphics::ImpAnimationPlayer::Initialize()
{

}

std::wstring ImpAnimationPlayer::GetAnimation(size_t objectID)
{
    return _playingAnimation[objectID].substr(6, _playingAnimation[objectID].size());
}

void ImpAnimationPlayer::SetSkinningObject(size_t objectID,
    SkinnedMeshObject* skinnedObject)
{
    _skinningAnimationObject[objectID] = skinnedObject;
}

void ImpAnimationPlayer::SetDefaultAnimation(size_t objectID,
    const std::wstring& animationName)
{
    _defaultAnimation[objectID] = animationName;
}

void ImpAnimationPlayer::AddInterpolationObject(const std::wstring& animationName,
    SkinnedMeshObject* skinnedObject)
{
    _interpolating[animationName] = skinnedObject;
}

void ImpAnimationPlayer::Update(float deltaTime)
{
	// if 프레임 시간이 지났다면 Translate
	//_time += deltaTime * 90000000.0f; // fbx
    //_time += deltaTime * 5000; // 조
	//_time = 0;
	for (const auto& object : _animationObjects)
	{
		if (_isPlayings[object.first])
		{
            _playDeltaTime[object.first] += deltaTime;
            //if (_playDeltaTime[object.first] > 5.0f / 30)
            if (_playDeltaTime[object.first] > _playSpeed[object.first] / 30)
            {
                //_playDeltaTime[object.first] -= 1 / 10;
                _playDeltaTime[object.first] = 0;
                _playTime[object.first]++;

				for (const auto& interpolationObject : _interpolating)
				{
					if (interpolationObject.first == object.first)
					{
                        interpolationObject.second->_changingCount++;
						if (interpolationObject.second->_changingCount = -100)
						{
                            interpolationObject.second->_changingCount = -2000000;
						}
						if (interpolationObject.second->_changingCount == 6)
						{
                            interpolationObject.second->_changingCount = -2000000;
                            _interpolating.erase(interpolationObject.first);
                            break;
						}
					}
				}
            }
			if (_isOne[object.first] && (object.second->_maxAnimTime - 5 == _playTime[object.first]))
            {
                size_t objectID = _wtoi(object.first.substr(0, 6).c_str());
                _skinningAnimationObject[objectID]->SetAnimation(_defaultAnimation[objectID]);
                PlayLoopAnimation(objectID, _defaultAnimation[objectID]);

                AddInterpolationObject(_defaultAnimation[objectID], _skinningAnimationObject[objectID]);
                _skinningAnimationObject[objectID]->_changingCount = 1;

                _playTime[object.first]   = 0;
                _isPlayings[object.first] = false;
                continue;
            }
			if (_playTime[object.first] > object.second->_maxAnimTime)
			//if (_playTime[object.first] > 20)
			{
                _playTime[object.first] = 0;
			}

			for (const auto& node : object.second->_nodes)
			{
				if (node.second->_animation != nullptr)
				{
					if (node.second->_animation->_positions.size() > 2)
					{
                        if (_playTime[object.first] < node.second->_animation->_positions.size())
						node.second->Translate(node.second->_animation->_positions[_playTime[object.first]]->_pos);
					}
					if (node.second->_animation->_rotations.size() > 2)
					{
						if (_playTime[object.first] < node.second->_animation->_rotations.size())
						node.second->Rotate(node.second->_animation->_rotations[_playTime[object.first]]->_rotQT_accumulation);
					}
				}
			}
		}
	}

	/*if (_time > 103488000)
	{
        _time = 0;
	}
	
	for (const auto& object : _animationObject)
	{
		for (const auto& node : object->_nodes)
		{
			if (node.second != nullptr)
			{
                if (node.second->_animation != nullptr)
			{
                    if (node.second->_animation->_positions.size() > 0)
				{
					for (int j = node.second->_animation->_positions.size() - 1; j >= 0; j--)
					{
                        if (_time > node.second->_animation->_positions[j]->_tick)
						{
                            if (node.first == "Pelvis")
                            {
                                // node.second->Translate(
                                //     node.second->_animation->_positions[j]->_pos);
                            }

 							node.second->Translate(node.second->_animation->_positions[j]->_pos);

							break;
						}
					}
				}
                if (node.second->_animation->_rotations.size() > 0)
				{
					for (int j = node.second->_animation->_rotations.size() - 1; j >= 0; j--)
					{
                        if (_time > node.second->_animation->_rotations[j]->_tick)
						{
							node.second->Rotate(node.second->_animation->_rotations[j]->_rotQT_accumulation);
                             if (node.first == "Pelvis")
                             {
//                                  node.second->Rotate(
//                                      node.second->_animation->_rotations[j]
//                                          ->_rotQT_accumulation);
                             }

							break;
						}
					}
				}
			}
			}
		}
	}*/
}

void ImpAnimationPlayer::UpdatePlaySpeed(const std::wstring& animationName, float speed)
{
    _playSpeed[animationName] = 1 / speed;
}

void ImpAnimationPlayer::PlayLoopAnimation(size_t objectID, const std::wstring& animationName)
{
    _playingAnimation[objectID]     = animationName;

    _isPlayings[animationName] = true;
    _playTime[animationName]   = 0;
    _playDeltaTime[animationName]   = 0;
    _playSpeed[animationName]     = 1;
    _isOne[animationName]           = false;
}

void ImpAnimationPlayer::PlayAnimation(size_t objectID, const std::wstring& animationName)
{
    _playingAnimation[objectID] = animationName;

    _isPlayings[animationName] = true;
    _playTime[animationName]      = 0;
    _playDeltaTime[animationName] = 0;
    _playSpeed[animationName]     = 1;
    _isOne[animationName]         = true;
}

void ImpAnimationPlayer::AddObject(const std::wstring& animationName, ImpStructure::ImpAnimationObject* object)
{
    _animationObjects[animationName] = object;
}

void ImpAnimationPlayer::DeleteObject()
{

}
