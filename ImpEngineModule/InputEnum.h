#pragma once

/// <summary>
/// 전역적으로 사용하는 enum class 이므로 namespace를 따로 정의하지 않음
/// </summary>

enum class KeyState
{
    Tap,  // 막 누른 시점
    Hold, // 누르고 있는
    Away, // 막 뗀 시점
    None, // 눌리지 않았고, 이전에도 눌리지 않은 상태
};

enum class Key
{
    Left,
    Right,
    Up,
    Down,

    Q,
    W,
    E,
    R,
    T,
    Y,
    U,
    O,
    P,
    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    L,
    Z,
    X,
    C,
    V,
    B,
    N,
    M,

    Alt,
    Ctrl,
    LShift,
    Space,
    Enter,
    ESC,
    Tab,

    LMouse,
    RMouse,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,

    Ins,
    Del,
    Home,
    End,
    PgUp,
    PgDn,

    Last, // enum의 끝
};

enum class PadKey
{
    DpadUp,
    DpadDown,
    DpadLeft,
    DpadRight,

    A,
    B,
    X,
    Y,

    Start,
    Back,

    LeftThumb,
    RightThumb,

    LeftShoulder,
    RightShoulder,

    LeftTrigger,
    RightTrigger,

    Last
};

enum class PadStick
{
    leftX,
    leftY,
    rightX,
    rightY
};

enum class VibrationMode
{
    Left,
    Right,
    Both
};
