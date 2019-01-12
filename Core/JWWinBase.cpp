#include "JWWinBase.h"

using namespace JW_GUI;

// Static member variables declaration
bool JWWinBase::ms_onMouseMove = false;
bool JWWinBase::ms_onMouseDoubleCliked = false;
bool JWWinBase::ms_IsWindowCaptured = false;
bool JWWinBase::ms_onCaptureWindow = false;
bool JWWinBase::ms_onReleaseWindow = false;
bool JWWinBase::ms_onRestoreWindow = false;

LRESULT CALLBACK JW_GUI::WindowProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	WINDOWPLACEMENT windowPlacement;

	switch (Message)
	{
	case WM_LBUTTONDBLCLK:
		JWWinBase::ms_onMouseDoubleCliked = true;
		break;
	case WM_LBUTTONDOWN:
		if (!JWWinBase::ms_IsWindowCaptured)
		{
			JWWinBase::ms_IsWindowCaptured = true;
			JWWinBase::ms_onCaptureWindow = true;
		}
		break;
	case WM_LBUTTONUP:
		if (JWWinBase::ms_IsWindowCaptured)
		{
			JWWinBase::ms_IsWindowCaptured = false;
			JWWinBase::ms_onReleaseWindow = true;
		}
		break;
	case WM_MOUSEMOVE:
		JWWinBase::ms_onMouseMove = true;
		break;
	case WM_ACTIVATEAPP:
		if (wParam)
		{
			// Activating the window
			GetWindowPlacement(hWnd, &windowPlacement);
			if (windowPlacement.showCmd == SW_SHOWMINIMIZED)
			{
				JWWinBase::ms_onRestoreWindow = true;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

void JWWinBase::Update()
{
	if (ms_onCaptureWindow)
	{
		RECT capturedWindowRect;

		// Capture the mouse position
		GetCursorPos(&m_CapturedMousePosition);

		// Capture the window position
		GetWindowRect(m_hWnd, &capturedWindowRect);
		m_CapturedWindowPosition.x = capturedWindowRect.left;
		m_CapturedWindowPosition.y = capturedWindowRect.top;

		// Capture the window size
		m_CapturedWindowSize.x = capturedWindowRect.right - capturedWindowRect.left;
		m_CapturedWindowSize.y = capturedWindowRect.bottom - capturedWindowRect.top;

		// Capture the window
		SetCapture(m_hWnd);

		ms_onCaptureWindow = false;
	}

	if (ms_onReleaseWindow)
	{
		// Release the captured window
		ReleaseCapture();

		ms_onReleaseWindow = false;

	}

	if (ms_onRestoreWindow)
	{
		ShowWindow(m_hWnd, SW_RESTORE);
		UpdateWindowSize();
		ResetDevice();

		ms_onRestoreWindow = false;
	}

	if (ms_onMouseMove)
	{
		GetCursorPos(&m_MousePosition);
		ms_onMouseMove = false;
	}
}

auto JWWinBase::Create(STRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error
{
	m_WindowPosition = Position;
	m_WindowSize = WindowSize;
	m_ClearColor = BackColor;

	m_hInstance = GetModuleHandle(nullptr);

	WNDCLASSA r_WndClass;
	r_WndClass.cbClsExtra = 0;
	r_WndClass.cbWndExtra = 0;
	r_WndClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255)); // This color doesn't matter for DX will clear on it
	r_WndClass.hCursor = nullptr; // LoadCursor(nullptr, IDC_ARROW);
	r_WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	r_WndClass.hInstance = m_hInstance;
	r_WndClass.lpfnWndProc = WindowProcedure;
	r_WndClass.lpszClassName = Name.c_str();
	r_WndClass.lpszMenuName = nullptr;
	r_WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClassA(&r_WndClass);

	if (nullptr == (m_hWnd = CreateWindowA(Name.c_str(), Name.c_str(), WS_POPUP, Position.x, Position.y,
		WindowSize.x, WindowSize.y, nullptr, (HMENU)nullptr, m_hInstance, nullptr)))
		return Error::WindowNotCreated;

	UnregisterClassA(Name.c_str(), m_hInstance);

	if (nullptr == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return Error::DirectXNotCreated;

	D3DPRESENT_PARAMETERS D3DPP;
	ZeroMemory(&D3DPP, sizeof(D3DPP));
	D3DPP.Windowed = TRUE;
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPP, &m_pD3DDevice)))
	{
		return Error::DirectXNotCreated;
	}

	return Error::Ok;
}

void JWWinBase::Destroy()
{
	JW_RELEASE(m_pD3D);
	JW_RELEASE(m_pD3DDevice);
}

auto JWWinBase::BeginRender()->HRESULT
{
	if (m_pD3DDevice)
	{
		m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.0f, 0);
		return m_pD3DDevice->BeginScene();
	}
	return E_FAIL;
}

auto JWWinBase::EndRender()-> HRESULT
{
	if (m_pD3DDevice)
	{
		m_pD3DDevice->EndScene();
		return m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
	}
	return E_FAIL;
}

auto JWWinBase::CheckDevice()->HRESULT
{
	return m_pD3DDevice->TestCooperativeLevel();
}

auto JWWinBase::ResetDevice()->HRESULT
{
	ZeroMemory(&m_D3DPP, sizeof(m_D3DPP));
	m_D3DPP.Windowed = TRUE;
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	m_D3DPP.hDeviceWindow = m_hWnd;

	return m_HResult = m_pD3DDevice->Reset(&m_D3DPP);
}

auto JWWinBase::IsMouseLeftButtonDown() const->bool
{
	return ms_IsWindowCaptured;
}

auto JWWinBase::IsMouseLeftButtonUp() const->bool
{
	return !ms_IsWindowCaptured;
}

auto JWWinBase::OnMouseMove() const->bool
{
	return ms_onMouseMove;
}

auto JWWinBase::OnMouseDoubleClicked()->bool
{
	if (ms_onMouseDoubleCliked)
	{
		ms_onMouseDoubleCliked = false;
		return true;
	}
	else
	{
		return false;
	}
}

void JWWinBase::MinimizeWindow()
{
	if (GetWindowShowState() == SW_SHOWNORMAL || GetWindowShowState() == SW_SHOWMAXIMIZED)
	{
		ShowWindow(m_hWnd, SW_MINIMIZE);
	}
}

void JWWinBase::MaximizeWindow()
{
	if (m_bMaximized)
	{
		SetWindowSize(m_WindowSizePrev);
		SetWindowPosition(m_WindowPositionPrev);

		m_bMaximized = false;
	}
	else
	{
		m_WindowSizePrev = m_WindowSize;
		m_WindowPositionPrev = m_WindowPosition;

		RECT rect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rect, 0);
		int W = GetSystemMetrics(SM_CXSCREEN);
		int H = rect.bottom - rect.top;
		
		SetWindowSize(Int2(W, H));
		SetWindowPosition(Int2(0, 0));

		m_bMaximized = true;
	}
}

void JWWinBase::UpdateWindowSize()
{
	RECT tempRect;
	GetWindowRect(m_hWnd, &tempRect);

	m_WindowSize.x = tempRect.right - tempRect.left;
	m_WindowSize.y = tempRect.bottom - tempRect.top;
}

void JWWinBase::SetWindowPosition(Int2 Value)
{
	m_WindowPosition = Value;
	SetWindowPos(m_hWnd, nullptr, Value.x, Value.y, 0, 0, SWP_NOSIZE);
}

void JWWinBase::SetWindowSize(Int2 Value, bool bResetDevice)
{
	m_WindowSize = Value;
	SetWindowPos(m_hWnd, nullptr, 0, 0, Value.x, Value.y, SWP_NOMOVE);

	if (bResetDevice)
		ResetDevice();
}

auto JWWinBase::GetWindowPosition() const->Int2
{
	return m_WindowPosition;
}

auto JWWinBase::GetCapturedWindowPosition() const->Int2
{
	return m_CapturedWindowPosition;
}

auto JWWinBase::GetCapturedWindowSize() const->Int2
{
	return m_CapturedWindowSize;
}

auto JWWinBase::GetWindowSize() const->Int2
{
	return m_WindowSize;
}

auto JWWinBase::GethWnd() const->HWND
{
	return m_hWnd;
}

auto JWWinBase::GethInstance() const->HINSTANCE
{
	return m_hInstance;
}

auto JWWinBase::GetDevice() const->LPDIRECT3DDEVICE9
{
	return m_pD3DDevice;
}

auto JWWinBase::GetMousePositionScreen() const->Int2
{
	return m_MousePosition;
}

auto JWWinBase::GetMousePositionClient() const->Int2
{
	POINT Result = m_MousePosition;
	ScreenToClient(m_hWnd, &Result);
	return Result;
}

auto JWWinBase::GetCapturedMousePositionScreen() const->Int2
{
	return m_CapturedMousePosition;
}

auto JWWinBase::GetCapturedMousePositionClient() const->Int2
{
	POINT Result = m_CapturedMousePosition;
	ScreenToClient(m_hWnd, &Result);
	return Result;
}

auto JWWinBase::GetWindowShowState()->UINT
{
	WINDOWPLACEMENT windowPlacement;
	GetWindowPlacement(m_hWnd, &windowPlacement);

	return windowPlacement.showCmd;
}

auto JWWinBase::IsWindowMaximized()->bool
{
	return m_bMaximized;
}