#pragma once

namespace ImpEngineModule
{
/// <summary>
/// 입력에 관한 처리를 하는 클래스
/// ex) 마우스, 키보드 등등
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

    // 키보드 키의 현재상태를 반환
    KeyState GetKeyState(Key key) const;
    bool IsKeyState(const Key& _key, const KeyState& _state) const;

    // GamePadKey 현재상태를 반환
    KeyState GetPadKeyState(unsigned int id, PadKey key) const;
    bool IsPadKeyState(unsigned int id, PadKey key, KeyState keyState) const;
    void SetVibration(unsigned int id,
                      VibrationMode mode,
                      unsigned short intensity,
                      float duration);

    // -1.f ~ 1.f 사이의 스틱의 방향정보를 반환
    float GetStickInfomation(unsigned int id, PadStick padStick) const;

    // 이번 프레임 마우스 위치
    POINT GetMousePosition() const
    {
        return currentMousePosition;
    }
    // 이번 프레임 마우스 변화량
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
    HWND _hwnd; // 메인 윈도우 핸들
    KeyInfomation _keyInfomations[static_cast<size_t>(Key::Last)]; // 키보드정보를 담은 배열

    KeyInfomation _padKeyInfomations[XUSER_MAX_COUNT][static_cast<size_t>(
        PadKey::Last)]; // 게임패드 키정보
    PadStickInfomation _padStickInfomations[XUSER_MAX_COUNT]; // 스틱관련정보 저장

    VibrationInfomation _padVibrationInfomations[XUSER_MAX_COUNT]; // 패드 진동 관련

    const int _matchVK[static_cast<size_t>(Key::Last)]; // KEY 값을 가상키값과 매칭시켜주는 배열
    const int _matchPadKey[static_cast<size_t>(
        PadKey::LeftTrigger)]; // PadKey 값을 가상키값과 매칭시키는 배열

    POINT currentMousePosition; // 현재 프레임 마우스 위치
    POINT prevMousePosition;    // 이전 프레임 마우스 위치
    POINT deltaMousePosition;   // 이번프레임 마우스 변화량

    bool _isActive; // 현재 키입력을 받는지
};
} // namespace ImpEngineModule
