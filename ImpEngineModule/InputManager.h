#pragma once

namespace ImpEngineModule
{
/// <summary>
/// �Է¿� ���� ó���� �ϴ� Ŭ����
/// ex) ���콺, Ű���� ���
/// </summary>
class InputManager
{
    static constexpr float VibrationEnd = -1.f;

    struct KeyInfomation
    {
        KeyState state = KeyState::None;
        bool prevPush  = false;
    };

    struct PadStickInfomation
    {
        float leftX  = 0.f;
        float leftY  = 0.f;
        float rightX = 0.f;
        float rightY = 0.f;
    };

    struct VibrationInfomation
    {
        unsigned short leftIntensity  = 0;
        unsigned short rightIntensity = 0;

        float leftDuration  = VibrationEnd;
        float rightDuration = VibrationEnd;
    };

public:
    struct Vibration
    {
        VibrationMode _mode     = VibrationMode::Both;
        unsigned int _intensity = 30000;
        float _second           = 1.f;
    };

public:
    InputManager();
    ~InputManager();

    void Initalize(HWND main);
    void Update(float dt);

    void OffAllPadVibration();
    void Finalize();

    // Ű���� Ű�� ������¸� ��ȯ
    KeyState GetKeyState(Key key) const;
    bool IsKeyState(const Key& _key, const KeyState& _state) const;

    // GamePadKey ������¸� ��ȯ
    KeyState GetPadKeyState(unsigned int id, PadKey key) const;
    bool IsPadKeyState(unsigned int id, PadKey key, KeyState keyState) const;
    void SetVibration(unsigned int id,
                      VibrationMode mode,
                      unsigned short intensity,
                      float duration);

    // -1.f ~ 1.f ������ ��ƽ�� ���������� ��ȯ
    float GetStickInfomation(unsigned int id, PadStick padStick) const;

    // �̹� ������ ���콺 ��ġ
    POINT GetMousePosition() const
    {
        return currentMousePosition;
    }
    // �̹� ������ ���콺 ��ȭ��
    POINT GetDeltaMousePosition() const
    {
        return deltaMousePosition;
    }

    bool IsActive() const
    {
        return _isActive;
    }
    void SetActive(bool isActive)
    {
        _isActive = isActive;
    }

    RECT GetScreenRect() const;

private:
    void PadUpdate(float dt);

    float AdjustStickBias(short value);

private:
    HWND _hwnd; // ���� ������ �ڵ�
    KeyInfomation _keyInfomations[static_cast<size_t>(Key::Last)]; // Ű���������� ���� �迭

    KeyInfomation _padKeyInfomations[XUSER_MAX_COUNT][static_cast<size_t>(
        PadKey::Last)]; // �����е� Ű����
    PadStickInfomation _padStickInfomations[XUSER_MAX_COUNT]; // ��ƽ�������� ����

    VibrationInfomation _padVibrationInfomations[XUSER_MAX_COUNT]; // �е� ���� ����

    const int _matchVK[static_cast<size_t>(Key::Last)]; // KEY ���� ����Ű���� ��Ī�����ִ� �迭
    const int _matchPadKey[static_cast<size_t>(
        PadKey::LeftTrigger)]; // PadKey ���� ����Ű���� ��Ī��Ű�� �迭

    POINT currentMousePosition; // ���� ������ ���콺 ��ġ
    POINT prevMousePosition;    // ���� ������ ���콺 ��ġ
    POINT deltaMousePosition;   // �̹������� ���콺 ��ȭ��

    bool _isActive; // ���� Ű�Է��� �޴���
};
} // namespace ImpEngineModule
