#pragma once
#include "Component.h"


namespace ImpEngineModule
{
/// <summary>
/// �ִϸ��̼� ������Ʈ
/// </summary>
class AnimationClip: public Component
{
public:
    AnimationClip();
    ~AnimationClip();

    Component* Clone(Component* clone = nullptr) const override;

    void Start() override;


public:
    // �ִϸ��̼Ǹ���� ��ȯ�Ѵ�.
    const std::vector<std::wstring>& GetAnimations() const
    {
        return _animations;
    }

    // ���� ������� �ִϸ��̼��� �����´�
    const std::wstring& GetCurrentAnimation() const
    {
        return _currentAnimation;
    }

    // �ִϸ��̼� ���
    void PlayAnimation(const std::wstring& name);

    void PlayAnimation(size_t index);

    // �ִϸ��̼� �ѹ��� ���
    void PlayAnimationNoneLoop(size_t playAniIndex, size_t nextAniIndex);

    // �׷��Ƚ� ���ε��뵵�� �Լ�
    bool IsChangedAnimation() const
    {
        return _isChangedAnimation;
    }

    // �׷��Ƚ� ���ε��뵵�� �Լ�
    void SetIsChangedAnimation(bool val)
    {
        _isChangedAnimation = val;
    }

    bool HasAnimation(const std::wstring& name) const;

    // Animation �ӵ� = dt * playbackSpeed
    float GetPlaybackSpeed() const;

    // �ִϸ��̼� ����ӵ��� �����Ѵ�.
    void SetPlaybackSpeed(float speed);

    // ���� �ִϸ��̼��� ������������
    bool IsStop() const
    {
        return _isStop;
    }

    // �ִϸ��̼� ����
    void SetStop(bool isStop)
    {
        _isStop = isStop;
    }

    // �ݺ���� �ִϸ��̼�����
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
    std::vector<std::wstring> _animations; // �ִϸ��̼� fbx ��ε��� ��´�.
    std::wstring _currentAnimation;
    std::wstring _nextAnimation; // �������� ����Ǵ� �ִϸ��̼�

    float _playbackSpeed; // �ִϸ��̼� ����ӵ�

    bool _isLoop;
    bool _isBindGraphics;
    bool _isChangedAnimation;
    bool _isStop;

    REFLECTION_FRIEND(AnimationClip);
};

} // namespace ImpEngineModule