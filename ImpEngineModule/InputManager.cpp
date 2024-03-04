#include "EngineModulePCH.h"
#include "InputManager.h"


ImpEngineModule::InputManager::InputManager()
	:_matchVK{
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		'Q','W','E','R','T','Y','U','O','P',
		'A','S','D','F','G','H','J','K','L',
		'Z','X','C','V','B','N','M',

		VK_MENU, // ALT,
		VK_CONTROL, // CTRL,
		VK_LSHIFT, // LSHIFT,
		VK_SPACE, // SPACE,
		VK_RETURN, // ENTER,
		VK_ESCAPE, // ESC,
		VK_TAB, //TAB

		VK_LBUTTON, // LBTN
		VK_RBUTTON, // RBTN

		VK_F1, // F1
		VK_F2, // F2
		VK_F3, // F3
		VK_F4, // F4
		VK_F5, // F5
        VK_F6, // F6
        VK_F7, // F7
        VK_F8, // F8

       VK_INSERT, //        Ins,
       VK_DELETE,//Del,
       VK_HOME,//Home,
      VK_END,   //End,
      VK_PRIOR,//PgUp,
       VK_NEXT,//PgDn,

	}
	, _matchPadKey{
        0x0001, //DpadUp,
		0x0002, //DpadDown,
		0x0004, //DpadLeft,
		0x0008, //DpadRight,
		0x1000, //A,
		0x2000, //B,
		0x4000, //X,
		0x8000, //Y,
		0x0010, //Start,
		0x0020, //Back,
		0x0040, //LeftThumb,
		0x0080, //RightThumb,
		0x0100, //LeftShoulder,
		0x0200, //RightShoulder,
	}
	, _keyInfomations{}
	, _hwnd(nullptr)
	, currentMousePosition{}
	, prevMousePosition{}
	, deltaMousePosition{} 
	,_isActive(true)
    ,_padKeyInfomations{}
    ,_padStickInfomations{}
{
}

ImpEngineModule::InputManager::~InputManager()
{
}

void ImpEngineModule::InputManager::Initalize(HWND main)
{
    _hwnd = main;
}

void ImpEngineModule::InputManager::Update(float dt)
{

    HWND hWnd = GetFocus();

    // 현재 포커스중일때
    if (nullptr != hWnd && _isActive)
    {
        for (int i = 0; i < static_cast<int>(Key::Last); ++i)
        {
            if (GetAsyncKeyState(_matchVK[i]) & 0x8000)
            {
                if (_keyInfomations[i].prevPush)
                {
                    _keyInfomations[i].state = KeyState::Hold;
                }
                else
                {
                    _keyInfomations[i].state = KeyState::Tap;
                }
                _keyInfomations[i].prevPush = true;
            }
            else
            {
                if (_keyInfomations[i].prevPush)
                {
                    _keyInfomations[i].state = KeyState::Away;
                }
                else
                {
                    _keyInfomations[i].state = KeyState::None;
                }
                _keyInfomations[i].prevPush = false;
            }
        }

        prevMousePosition = currentMousePosition;

        // Mouse 위치 계산
        GetCursorPos(&currentMousePosition);

        // 스크린 좌표계의 마우스 위치
        ScreenToClient(_hwnd, &currentMousePosition);

        // 마우스가 이번프레임에 얼마나 움직였는지
        deltaMousePosition = { currentMousePosition.x - prevMousePosition.x,
                               currentMousePosition.y - prevMousePosition.y };
    }
    // 포커스해제되어 예외처리 진행
    else
    {
        for (int i = 0; i < static_cast<int>(Key::Last); ++i)
        {
            _keyInfomations[i].prevPush = false;
            if (KeyState::Tap == _keyInfomations[i].state ||
                KeyState::Hold == _keyInfomations[i].state)
            {
                _keyInfomations[i].state = KeyState::Away;
            }
            else if (KeyState::Away == _keyInfomations[i].state)
            {
                _keyInfomations[i].state = KeyState::None;
            }
        }
    }

    // GamePad 관련 업데이트
    PadUpdate(dt);
}

void ImpEngineModule::InputManager::OffAllPadVibration()
{
    for (int i = 0; i < 4; ++i)
    {
        SetVibration(i, VibrationMode::Both, 0, 0.f);
    }
}

void ImpEngineModule::InputManager::Finalize()
{
}


KeyState ImpEngineModule::InputManager::GetKeyState(Key key) const
{
    return _keyInfomations[static_cast<int>(key)].state;
}

bool ImpEngineModule::InputManager::IsKeyState(const Key& _key,
                                               const KeyState& _state) const
{
    if (_state == GetKeyState(_key))
    {
        return true;
    }

    return false;
}


KeyState ImpEngineModule::InputManager::GetPadKeyState(unsigned int id, PadKey key) const
{
    assert(id <= 3);

    return _padKeyInfomations[id][static_cast<size_t>(key)].state;
}


bool ImpEngineModule::InputManager::IsPadKeyState(unsigned int id,
                                                  PadKey key,
                                                  KeyState keyState) const
{
    if (keyState == GetPadKeyState(id, key))
    {
        return true;
    }

    return false;
}

void ImpEngineModule::InputManager::SetVibration(unsigned int id,
                                                 VibrationMode mode,
                                                 unsigned short intensity,
                                                 float duration)
{
    assert(id <= 3);

    if (mode == VibrationMode::Left)
    {
        _padVibrationInfomations[id].leftIntensity = intensity;
        _padVibrationInfomations[id].leftDuration  = duration;
    }
    else if (mode == VibrationMode::Right)
    {
        _padVibrationInfomations[id].rightIntensity = intensity;
        _padVibrationInfomations[id].rightDuration  = duration;
    }
    else if (mode == VibrationMode::Both)
    {
        _padVibrationInfomations[id].leftIntensity = intensity;
        _padVibrationInfomations[id].leftDuration  = duration;

        _padVibrationInfomations[id].rightIntensity = intensity;
        _padVibrationInfomations[id].rightDuration  = duration;
    }

    // Pad 진동 관련
    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
    vibration.wLeftMotorSpeed = _padVibrationInfomations[id].leftIntensity; // 왼쪽 진동 모터의 세기0
    vibration.wRightMotorSpeed = _padVibrationInfomations[id].rightIntensity; // 오른쪽 진동 모터의 세기
    XInputSetState(id, &vibration);
}

float ImpEngineModule::InputManager::GetStickInfomation(unsigned int id,
                                                        PadStick padStick) const
{
    assert(id <= 3);

    float value = 0.f;
    switch (padStick)
    {
        case PadStick::leftX:
        {
            value = _padStickInfomations[id].leftX;
        }
        break;
        case PadStick::leftY:
        {
            value = _padStickInfomations[id].leftY;
        }
        break;
        case PadStick::rightX:
        {
            value = _padStickInfomations[id].rightX;
        }
        break;
        case PadStick::rightY:
        {
            value = _padStickInfomations[id].rightY;
        }
        break;
        default:
            break;
    }

    return value;
}


RECT ImpEngineModule::InputManager::GetScreenRect() const
{
    RECT rect;

    GetWindowRect(_hwnd, &rect);

    return rect;
}

void ImpEngineModule::InputManager::PadUpdate(float dt)
{
    DWORD dwResult = ERROR_SUCCESS;
    HWND hWnd      = GetFocus();

    if (nullptr != hWnd && _isActive)
    {
        for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
        {
            // 컨트롤러 입력 상태 초기화
            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            // 컨트롤러 입력 상태 받아오기
            dwResult = XInputGetState(i, &state);

            if (dwResult == ERROR_SUCCESS)
            {
                for (size_t j = 0; j < static_cast<size_t>(PadKey::Last); ++j)
                {
                    bool isPressed = false;
                    if (j <= static_cast<size_t>(PadKey::RightShoulder))
                    {
                        isPressed = state.Gamepad.wButtons & _matchPadKey[j];
                    }
                    else if (j == static_cast<size_t>(PadKey::LeftTrigger))
                    {
                        isPressed = state.Gamepad.bLeftTrigger;
                    }
                    else if (j == static_cast<size_t>(PadKey::RightTrigger))
                    {
                        isPressed = state.Gamepad.bRightTrigger;
                    }

                    auto& padKeyInfo = _padKeyInfomations[i][j];

                    if (isPressed && !padKeyInfo.prevPush)
                    {
                        padKeyInfo.state = KeyState::Tap;
                    }
                    else if (isPressed && padKeyInfo.prevPush)
                    {
                        padKeyInfo.state = KeyState::Hold;
                    }
                    else if (!isPressed && padKeyInfo.prevPush)
                    {
                        padKeyInfo.state = KeyState::Away;
                    }
                    else
                    {
                        padKeyInfo.state = KeyState::None;
                    }

                    padKeyInfo.prevPush = isPressed;
                }
            }

            // Stick 관련정보 저장

            _padStickInfomations[i].leftX = AdjustStickBias(state.Gamepad.sThumbLX);
            _padStickInfomations[i].leftY = AdjustStickBias(state.Gamepad.sThumbLY);
            _padStickInfomations[i].rightX = AdjustStickBias(state.Gamepad.sThumbRX);
            _padStickInfomations[i].rightY = AdjustStickBias(state.Gamepad.sThumbRY);


            // 진동 관련 정보

            auto& vibrationInfo = _padVibrationInfomations[i];

            if (vibrationInfo.leftDuration != VibrationEnd)
            {
                vibrationInfo.leftDuration -= dt;

                // 왼쪽 진동 종료
                if (vibrationInfo.leftDuration <= 0.f)
                {
                    vibrationInfo.leftDuration = VibrationEnd;

                    XINPUT_VIBRATION vibration;
                    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
                    vibration.wLeftMotorSpeed = 0; // 왼쪽 진동 모터의 세기

                    if (vibrationInfo.rightDuration == VibrationEnd)
                    {
                        vibration.wRightMotorSpeed = 0; // 오른쪽 진동 모터의 세기
                    }
                    else
                    {
                        vibration.wRightMotorSpeed = _padVibrationInfomations[i].rightIntensity;
                    }

                    XInputSetState(i, &vibration);
                }
            }
            if (vibrationInfo.rightDuration != VibrationEnd)
            {
                vibrationInfo.rightDuration -= dt;

                // 오른쪽 진동 종료
                if (vibrationInfo.rightDuration <= 0.f)
                {
                    vibrationInfo.rightDuration = VibrationEnd;

                    XINPUT_VIBRATION vibration;
                    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
                    vibration.wRightMotorSpeed = 0; // 오른쪽 진동 모터의 세기

                    if (vibrationInfo.leftDuration == VibrationEnd)
                    {
                        vibration.wLeftMotorSpeed = 0; // 왼쪽 진동 모터의 세기
                    }
                    else
                    {
                        vibration.wLeftMotorSpeed = _padVibrationInfomations[i].leftIntensity;
                    }

                    XInputSetState(i, &vibration);
                }
            }
        }
    }
    else
    {
        for (size_t j = 0; j < static_cast<size_t>(PadKey::Last); ++j)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (_padKeyInfomations[i][j].prevPush)
                {
                    _padKeyInfomations[i][j].state = KeyState::Away;
                }
                else
                {
                    _padKeyInfomations[i][j].state = KeyState::None;
                }
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            _padStickInfomations[i].leftX  = 0.f;
            _padStickInfomations[i].leftY  = 0.f;
            _padStickInfomations[i].rightX = 0.f;
            _padStickInfomations[i].rightY = 0.f;
        }
    }
}

float ImpEngineModule::InputManager::AdjustStickBias(short value)
{
    constexpr short minStickBias   = 10000;
    constexpr short maxStickBias   = 30000;
    constexpr float maxValue       = maxStickBias - minStickBias;
    constexpr float inversMaxValue = 1.f / maxValue;

    if (-minStickBias <= value && value <= minStickBias)
    {
        return 0.f;
    }
    else if (maxStickBias <= value)
    {
        return 1.f;
    }
    else if (value <= -maxStickBias)
    {
        return -1.f;
    }

    float adjustValue = 0.f;

    if (value <= 0)
    {
        adjustValue = (static_cast<float>(value) + minStickBias) * inversMaxValue;
    }
    else
    {
        adjustValue = (static_cast<float>(value) - minStickBias) * inversMaxValue;
    }


    return adjustValue;
}
