#include "EngineModulePCH.h"
#include "AnimationClip.h"

ImpEngineModule::AnimationClip::AnimationClip()
    : _isChangedAnimation(false)
    , _animations{}
    , _isBindGraphics(false)
    , _currentAnimation{}
    , _playbackSpeed(1.f)
    , _isStop(false)
    , _isLoop(true)
    , _nextAnimation{}
{
}

ImpEngineModule::AnimationClip::~AnimationClip()
{
}


ImpEngineModule::Component* ImpEngineModule::AnimationClip::Clone(
    Component* clone /*= nullptr*/) const
{
    AnimationClip* animation = static_cast<AnimationClip*>(clone);

    if (animation == nullptr)
    {
        animation = new AnimationClip(*this);
    }
    else
    {
        *animation = *this;
    }

    animation->_isBindGraphics = false;

    return animation;
}


void ImpEngineModule::AnimationClip::Start()
{
    if (_currentAnimation.empty())
    {
        _isChangedAnimation = false;
    }
    else
    {
        _isChangedAnimation = true;
    }
}

void ImpEngineModule::AnimationClip::PlayAnimation(const std::wstring& name)
{
    if (!HasAnimation(name) || _currentAnimation == name)
    {
        return;
    }

    _isChangedAnimation = true;
    _currentAnimation   = name;
    _isLoop             = true;
}


void ImpEngineModule::AnimationClip::PlayAnimationNoneLoop(size_t playAniIndex,
    size_t nextAniIndex)
{
    if (_animations.size() <= playAniIndex || _animations.size() <= nextAniIndex)
    {
        return;
    }

    if (_animations[playAniIndex] == _currentAnimation)
    {
        return;
    }


    _isLoop             = false;
    _isChangedAnimation = true;
    _currentAnimation   = _animations[playAniIndex];
    _nextAnimation      = _animations[nextAniIndex];
}

void ImpEngineModule::AnimationClip::PlayAnimation(size_t index)
{
    if (_animations.size() <= index)
    {
        return;
    }

    if (_animations[index] == _currentAnimation)
    {
        return;
    }

    _currentAnimation   = _animations[index];
    _isChangedAnimation = true;
    _isLoop             = true;
}

bool ImpEngineModule::AnimationClip::HasAnimation(const std::wstring& name) const
{
    for (const auto& ani : _animations)
    {
        if (ani == name)
        {
            return true;
        }
    }

    return false;
}


float ImpEngineModule::AnimationClip::GetPlaybackSpeed() const
{
    if (_isStop)
    {
        return 0.f;
    }

    return _playbackSpeed;
}

void ImpEngineModule::AnimationClip::SetPlaybackSpeed(float speed)
{
    _playbackSpeed = std::clamp(speed, 0.f, maxPlaybackSpeed);
}
