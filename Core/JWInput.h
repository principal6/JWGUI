#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include "JWCommon.h"

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

class JWInput final
{
private:
	static const int DIK_NUM_KEYS = 256; // @warning: This value must be 256

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPDIRECTINPUT8 m_DI8;
	LPDIRECTINPUTDEVICE8 m_DIDeviceKeyboard;
	LPDIRECTINPUTDEVICE8 m_DIDeviceMouse;

	char m_BufferKeyState[DIK_NUM_KEYS];
	bool m_KeyDown[DIK_NUM_KEYS];
	bool m_KeyUp[DIK_NUM_KEYS];
	DIMOUSESTATE2 m_MouseState;

	int m_MouseX, m_MouseY;
	bool m_MouseBtnDown[3];
	bool m_MouseBtnUp[3];
	bool m_MouseBtnIdle[3];

private:
	auto JWInput::CreateMouseDevice(DWORD dwFlags)->bool;
	auto JWInput::CreateKeyboardDevice(DWORD dwFlags)->bool;

	auto JWInput::CheckMouseButton(int button)->bool;

public:
	JWInput();
	~JWInput() {};

	// Creation & destruction
	auto JWInput::Create(HINSTANCE hInstance, HWND hWnd)->JWERROR;
	void JWInput::Destroy();

	// Recall
	auto JWInput::OnKeyDown(DWORD DIK_KeyCode)->bool;
	auto JWInput::OnKeyUp(DWORD DIK_KeyCode)->bool;
	auto JWInput::OnMouseMove()->bool;
	auto JWInput::OnMouseButtonDown(int button)->bool;
	auto JWInput::OnMouseButtonUp(int button)->bool;

	// Getter only
	auto JWInput::GetMouseState()->DIMOUSESTATE2;
};