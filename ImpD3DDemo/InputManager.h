#pragma once
/// ���콺 ó���� ���� HWND�� ������ include �� �ؾ��Ѵ�.
#include <windows.h>

#include "Singleton.h"

/// <summary>
/// Ű���� ����
/// </summary>
enum class KeyState
{
	None,	// ���� ������x, ���� ������ x
	Down,	// ���� ������x, ���� ������ o
	Press,	// ���� ������o, ���� ������ o
	Up		// ���� ������o, ���� ������ x
};

/// <summary>
/// ���콺, Ű������ �Է� ó���� ����Ѵ�.
/// 
/// �̱������� �����. (�̱��� ���ø��� ��ӹ޴´�.)
/// 2023.07.24 ������
/// </summary>
class InputManager : public Singleton<InputManager>
{
public:
	void Init(HWND hwnd);
	void Update();

	// Ű����, ���콺 �Ѵ� ó���� �����ϴ�. ���� Ű�� �̹� Mouse�� ���ԵǾ� �ִ�.
	bool IsGetKeyDown(int key) const { return m_key[key] == KeyState::Down; }
	bool IsGetKey(int key) const { return m_key[key] == KeyState::Press; }
	bool IsGetKeyUp(int key) const { return m_key[key] == KeyState::Up; }

	bool IsAnyKeyDown() { return m_isKeyDown; }
	bool IsAnyKey() { return m_isKey; }
	bool IsAnyKeyUp() { return m_isKeyUp; }

	// ī�޶� �ϴ� ���� ���ϰ� �����. ���� ī�޶� �߰��Ǹ� ���� �ʿ��Ҽ��� ����!
	// �ϴ� hwnd�� �ϳ���� �����ϰ� �����. ���� �þ�ٸ� �����ؾ� ��.
	POINT GetMousePosition();
	POINT GetDeltaPosition();

	void SetWheelPower(int power) { m_wheelPower = power; }
	int GetWheelPower() { return m_wheelPower; }

private:
	// �� ���� Ŭ������� ���� �����Ϸ��� �����ڸ� ���ܾ� �Ѵ�.
	InputManager() : m_hwnd(0), m_key{}, m_wheelPower(0), m_isKeyDown(false), m_isKey(0), m_isKeyUp(0), _preMousePosition(), _mousePosition() {}
	~InputManager() {}

private:
	HWND m_hwnd;
	int m_wheelPower;

	bool m_isKeyDown;
	bool m_isKey;
	bool m_isKeyUp;

	POINT _preMousePosition;
	POINT _mousePosition;

	KeyState m_key[0xFE /*VK_OEM_CLEAR*/ + 1];	// 255, https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes ��� ���� Ű�� ������ �� �ְ� ũ�⸦ ���Ѵ�.

	friend class Singleton<InputManager>;
};