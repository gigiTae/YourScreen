#pragma once
#include "Component.h"


namespace ImpEngineModule
{
/// <summary>
/// 애니메이션 컴포넌트
/// </summary>
class AnimationClip: public Component
{
public:
    AnimationClip();
    ~AnimationClip();

    Component* Clone(Component* clone = nullptr) const override;

    void Start() override;


public:
    // 애니메이션목록을 반환한다.
    const std::vector<std::wstring>& GetAnimations() const
    {
        return _animations;
    }

    // 현재 재생중인 애니메이션을 가져온다
    const std::wstring& GetCurrentAnimation() const
    {
        return _currentAnimation;
    }

    // 애니메이션 재생
    void PlayAnimation(const std::wstring& name);

    void PlayAnimation(size_t index);

    // 애니메이션 한번만 재생
    void PlayAnimationNoneLoop(size_t playAniIndex, size_t nextAniIndex);

    // 그래픽스 바인딩용도의 함수
    bool IsChangedAnimation() const
    {
        return _isChangedAnimation;
    }

    // 그래픽스 바인딩용도의 함수
    void SetIsChangedAnimation(bool val)
    {
        _isChangedAnimation = val;
    }

    bool HasAnimation(const std::wstring& name) const;

    // Animation 속도 = dt * playbackSpeed
    float GetPlaybackSpeed() const;

    // 애니메이션 재생속도를 지정한다.
    void SetPlaybackSpeed(float speed);

    // 현재 애니메이션이 정지상태인지
    bool IsStop() const
    {
        return _isStop;
    }

    // 애니메이션 정지
    void SetStop(bool isStop)
    {
        _isStop = isStop;
    }

    // 반복재생 애니메이션인지
    bool IsLoop() const
    {
        return _isLoop;
    }

    const std::wstring& GetNextAnimation() const
    {
        return _nextAnimation;
    }

    void SetCurrentAnimation(const std::wstring& animation)
    {
        _currentAnimation = animation;
    }

public:
    static constexpr float maxPlaybackSpeed = 100.f;

private:
    std::vector<std::wstring> _animations; // 애니메이션 fbx 경로들을 담는다.
    std::wstring _currentAnimation;
    std::wstring _nextAnimation; // 다음으로 재생되는 애니메이션

    float _playbackSpeed; // 애니메이션 재생속도

    bool _isLoop;
    bool _isBindGraphics;
    bool _isChangedAnimation;
    bool _isStop;

    REFLECTION_FRIEND(AnimationClip);
};

} // namespace ImpEngineModule