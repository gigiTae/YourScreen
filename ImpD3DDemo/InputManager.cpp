#include "pch.h"
#include "InputManager.h"
void InputManager::Init(HWND hwnd)
{
	m_hwnd = hwnd;
	for (int i = 0; i < 255; i++)
	{
		m_key[i] = KeyState::None;
	}
}

void InputManager::Update()
{
	m_isKeyDown = false;
	m_isKey = false;
	m_isKeyUp = false;

	for (int i = 0; i < 255; i++)
	{
		/// �̹� �����ӿ� Ű ����
		// ���� �����ӿ� None or Up �̾��µ� ���ȴ�. -> Down
		if ((m_key[i] == KeyState::None || m_key[i] == KeyState::Up)
			&& GetAsyncKeyState(i) & 0x8000)
		{
			m_key[i] = KeyState::Down;
			m_isKeyDown = true;
		}
		// ���� �����ӿ� Down �̾��µ� ���ȴ�. -> Press
		else if (m_key[i] == KeyState::Down && GetAsyncKeyState(i) & 0x8000)
		{
			m_key[i] = KeyState::Press;
			m_isKey = true;
		}

		/// �̹� �����ӿ� Ű �� ����
		// ���� �����ӿ� Down, Press �̾��µ� �ȴ�����. -> Up
		else if ((m_key[i] == KeyState::Down || m_key[i] == KeyState::Press) 
			&& !(GetAsyncKeyState(i) & 0x8000))
		{
			m_key[i] = KeyState::Up;
			m_isKeyUp = true;
		}
		// ���� �����ӿ� Up �̾��µ� �ȴ�����.
		else if (m_key[i] == KeyState::Up && !(GetAsyncKeyState(i) & 0x8000))
		{
			m_key[i] = KeyState::None;
		}
	}

	_preMousePosition = _mousePosition;
	_mousePosition = GetMousePosition();
}

POINT InputManager::GetMousePosition()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(m_hwnd, &mousePos);

	return mousePos;
}

POINT InputManager::GetDeltaPosition()
{
	return { _mousePosition.x - _preMousePosition.x,  _mousePosition.y - _preMousePosition.y };
}
