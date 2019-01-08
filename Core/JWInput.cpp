#include "JWInput.h"

JWInput::JWInput()
{
	m_hWnd = nullptr;
	m_hInstance = nullptr;

	m_DI8 = nullptr;
	m_DIDeviceKeyboard = nullptr;
	m_DIDeviceMouse = nullptr;

	memset(&m_BufferKeyState, false, sizeof(m_BufferKeyState));
	memset(&m_KeyDown, false, sizeof(m_KeyDown));
	memset(&m_KeyUp, false, sizeof(m_KeyUp));

	memset(m_MouseBtnDown, false, sizeof(m_MouseBtnDown));
	memset(m_MouseBtnUp, false, sizeof(m_MouseBtnUp));
	memset(m_MouseBtnIdle, true, sizeof(m_MouseBtnIdle));

	m_MouseX = 0;
	m_MouseY = 0;
}

auto JWInput::Create(HINSTANCE hInstance, HWND hWnd)->JWERROR
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	if (FAILED(DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_DI8, nullptr)))
		return JWERROR::DirectInputCreationFailed;

	if (FAILED(CreateMouseDevice(DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return JWERROR::DirectInputCreationFailed;

	if (FAILED(CreateKeyboardDevice(DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return JWERROR::DirectInputCreationFailed;

	return JWERROR::Ok;
}

auto JWInput::CreateMouseDevice(DWORD dwFlags)->bool
{
	if (FAILED(m_DI8->CreateDevice(GUID_SysMouse, &m_DIDeviceMouse, nullptr)))
		return false;

	if (FAILED(m_DIDeviceMouse->SetDataFormat(&c_dfDIMouse2)))
		return false;

	if (FAILED(m_DIDeviceMouse->SetCooperativeLevel(m_hWnd, dwFlags)))
		return false;

	if (FAILED(m_DIDeviceMouse->Acquire()))
		return false;

	return true;
}

auto JWInput::CreateKeyboardDevice(DWORD dwFlags)->bool
{
	if (FAILED(m_DI8->CreateDevice(GUID_SysKeyboard, &m_DIDeviceKeyboard, nullptr)))
		return false;

	if (FAILED(m_DIDeviceKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (FAILED(m_DIDeviceKeyboard->SetCooperativeLevel(m_hWnd, dwFlags)))
		return false;

	if (FAILED(m_DIDeviceKeyboard->Acquire()))
		return false;

	return true;
}

void JWInput::Destroy()
{
	if (m_DIDeviceMouse)
	{
		m_DIDeviceMouse->Unacquire();
		JW_RELEASE(m_DIDeviceMouse);
	}
	if (m_DIDeviceKeyboard)
	{
		m_DIDeviceKeyboard->Unacquire();
		JW_RELEASE(m_DIDeviceKeyboard);
	}

	JW_RELEASE(m_DI8);
}

auto JWInput::OnKeyDown(DWORD DIK_KeyCode)->bool
{
	HRESULT hr;

	if (FAILED(hr = m_DIDeviceKeyboard->GetDeviceState(sizeof(m_BufferKeyState), (LPVOID)&m_BufferKeyState)))
	{
		if (hr == DIERR_INPUTLOST)
			m_DIDeviceKeyboard->Acquire();
	}

	if (m_BufferKeyState[DIK_KeyCode] & 0x80) // When the key is pressed
	{
		m_KeyUp[DIK_KeyCode] = false;
		m_KeyDown[DIK_KeyCode] = true;
		return true;
	}
	else // not pressed
	{
		if (m_KeyDown[DIK_KeyCode] == true)
		{
			m_KeyUp[DIK_KeyCode] = true;
		}
		else
		{
			m_KeyUp[DIK_KeyCode] = false;
		}

		m_KeyDown[DIK_KeyCode] = false;
		return false;
	}
}

auto JWInput::OnKeyUp(DWORD DIK_KeyCode)->bool
{
	if (m_KeyUp[DIK_KeyCode])
		return true;

	return false;
}

auto JWInput::OnMouseMove()->bool
{
	HRESULT hr;
	memset(&m_MouseState, 0, sizeof(m_MouseState));
	if (FAILED(hr = m_DIDeviceMouse->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState)))
	{
		if (hr == DIERR_INPUTLOST)
			m_DIDeviceMouse->Acquire();
	}

	if (m_MouseState.lX || m_MouseState.lY || m_MouseState.lZ)
		return true;

	return false;
}

auto JWInput::GetMouseState()->DIMOUSESTATE2
{
	return m_MouseState;
}

auto JWInput::CheckMouseButton(int button)->bool
{
	if (m_MouseState.rgbButtons[button] & 0x80) // Button pressed
	{
		if (!m_MouseBtnDown[button] && m_MouseBtnIdle[button])
		{
			m_MouseBtnUp[button] = false;
			m_MouseBtnDown[button] = true;
		}
	}
	else if (!m_MouseState.rgbButtons[button])
	{
		if (!m_MouseBtnDown[button] && !m_MouseBtnIdle[button])
		{
			m_MouseBtnUp[button] = true;
			m_MouseBtnIdle[button] = true;
			m_MouseBtnDown[button] = false;
		}
		else if (m_MouseBtnDown[button] && m_MouseBtnIdle[button])
		{
			m_MouseBtnUp[button] = true;
			m_MouseBtnDown[button] = false;
		}
	}

	return true;
}

auto JWInput::OnMouseButtonDown(int button)->bool
{
	CheckMouseButton(button);

	if (m_MouseBtnDown[button])
	{
		m_MouseBtnDown[button] = false;
		m_MouseBtnIdle[button] = false;
		return true;
	}

	return false;
}

auto JWInput::OnMouseButtonUp(int button)->bool
{
	CheckMouseButton(button);

	if (m_MouseBtnUp[button])
	{
		m_MouseBtnUp[button] = false;
		m_MouseBtnIdle[button] = true;
		return true;
	}

	return false;
}