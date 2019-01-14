#include "JWOutterWindow.h"

using namespace JW_GUI;

// Static member variables declaration
bool JWOutterWindow::ms_onCaptureWindow = false;
bool JWOutterWindow::ms_onReleaseWindow = false;
bool JWOutterWindow::ms_onRestoreWindow = false;

inline auto GetWindowShowState(HWND hWnd)->UINT
{
	WINDOWPLACEMENT windowPlacement;
	GetWindowPlacement(hWnd, &windowPlacement);

	return windowPlacement.showCmd;
}

LRESULT CALLBACK JW_GUI::WindowProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_LBUTTONDBLCLK:
		JWOutterWindow::ms_onMouseDoubleCliked = true;
		break;
	case WM_LBUTTONDOWN:
		if (!JWOutterWindow::ms_IsWindowCaptured)
		{
			JWOutterWindow::ms_IsWindowCaptured = true;
			JWOutterWindow::ms_onCaptureWindow = true;
		}
		break;
	case WM_LBUTTONUP:
		if (JWOutterWindow::ms_IsWindowCaptured)
		{
			JWOutterWindow::ms_IsWindowCaptured = false;
			JWOutterWindow::ms_onReleaseWindow = true;
		}
		break;
	case WM_MOUSEMOVE:
		JWOutterWindow::ms_onMouseMove = true;
		break;
	case WM_ACTIVATEAPP:
		if (wParam)
		{
			// Activating the window
			if (GetWindowShowState(hWnd) == SW_SHOWMINIMIZED)
			{
				JWOutterWindow::ms_onRestoreWindow = true;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

auto JWOutterWindow::Create(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error
{
	JWWindow::Create(Name, Position, WindowSize, BackColor);

	m_bRunning = true;
	m_hInstance = GetModuleHandle(nullptr);

	WNDCLASSW r_WndClass;
	r_WndClass.cbClsExtra = 0;
	r_WndClass.cbWndExtra = 0;
	r_WndClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255)); // This color doesn't matter for DX will clear on it
	r_WndClass.hCursor = nullptr; // LoadCursor(nullptr, IDC_ARROW);
	r_WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	r_WndClass.hInstance = m_hInstance;
	r_WndClass.lpfnWndProc = WindowProcedure;
	r_WndClass.lpszClassName = L"OUTTERWINDOW";
	r_WndClass.lpszMenuName = nullptr;
	r_WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClassW(&r_WndClass);

	if (nullptr == (m_hWnd = CreateWindowW(r_WndClass.lpszClassName, r_WndClass.lpszClassName, WS_POPUP,
		Position.x, Position.y, WindowSize.x, WindowSize.y, nullptr, (HMENU)nullptr, m_hInstance, nullptr)))
		return Error::WindowNotCreated;

	UnregisterClassW(r_WndClass.lpszClassName, m_hInstance);

	return Error::Ok;
}

auto JWOutterWindow::CreateGUI(JWDXDevice* pDXDevice)->Error
{
	return JWWindow::CreateGUI(pDXDevice);
}

void JWOutterWindow::Draw()
{
	// Always draw the title bar
	m_TitleBar->Draw();

	// When the window is maximized, the border is not drawn
	if (m_WindowState != WindowState::Maximized)
		m_WindowBorder->Draw();

	if (m_pControls.size())
	{
		for (JWControl* iterator : m_pControls)
		{
			iterator->Draw();
		}	
	}
}

auto JWOutterWindow::Update()->bool
{
	if (m_bRunning == false)
		return m_bRunning;

	// !!
	JWWindow::UpdateControls();

	if (ms_onCaptureWindow)
	{
		RECT capturedWindowRect;

		// Capture the mouse position
		GetCursorPos(&ms_MouseDownPosition);

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
		RestoreWindow();
		
		ms_onRestoreWindow = false;
	}

	if (ms_onMouseMove)
	{
		GetCursorPos(&ms_MousePosition);

		ms_onMouseMove = false;
	}

	m_TitleBar->UpdateSize(GetWindowSize());

	if (OnMouseDoubleClicked())
	{
		m_TitleBar->DoubleClickMaximize(GetMousePositionClient());
	}

	if (m_TitleBar->OnSystemMinimize())
	{
		MinimizeWindow();
	}

	if (m_TitleBar->OnSystemMaximize())
	{
		MaximizeWindow();
	}

	if (m_TitleBar->OnSystemExit())
	{
		ShutdownWindow();
		return m_bRunning;
	}

	if (IsMouseLeftButtonDown())
	{
		m_bCanResize = m_WindowBorder->CanResizeWindow();

		if (m_WindowState == WindowState::Maximized)
			m_bCanResize = false;

		if (!m_bValuesLocked)
		{
			// Get these values only the first time the mouse button is pressed
			m_bCanMove = m_TitleBar->CanMoveWindow(GetMouseDownPositionClient());

			if (m_bCanResize)
				m_bCanMove = false;

			// Lock the values
			m_bValuesLocked = true;
		}

		
		if (m_bCanResize) // Resize window
		{
			ResizeWindow();
		}
		else if (m_bCanMove) // Move window
		{
			if (m_WindowState == WindowState::Maximized)
			{
				// If the maximized window gets moved, restore the size
				m_TitleBar->ToggleSysMaxButton();
				MaximizeWindow();
			}
			else
			{
				Int2 NewPos = GetCapturedWindowPosition() + GetMousePositionScreen() - GetCapturedMousePositionScreen();

				SetWindowPosition(NewPos);
			}
		}
	}
	else
	{
		// End moving and resizing window
		m_TitleBar->StopWindow();
		m_WindowBorder->StopResizeWindow();

		// Unlock the values
		m_bValuesLocked = false;
	}

	return true;
}

void JWOutterWindow::SetWindowPosition(Int2 Value)
{
	JWWindow::SetWindowPosition(Value);
	SetWindowPos(m_hWnd, nullptr, Value.x, Value.y, 0, 0, SWP_NOSIZE);
}

void JWOutterWindow::SetWindowSize(Int2 Value)
{
	JWWindow::SetWindowSize(Value);
	SetWindowPos(m_hWnd, nullptr, 0, 0, Value.x, Value.y, SWP_NOMOVE);
	GetWindowRect(m_hWnd, &m_Rect);
}

auto JWOutterWindow::GethWnd() const->HWND
{
	return m_hWnd;
}

auto JWOutterWindow::GethInstance() const->HINSTANCE
{
	return m_hInstance;
}

auto JWOutterWindow::GetDevice() const->LPDIRECT3DDEVICE9
{
	return m_pDevice;
}

auto JWOutterWindow::GetMousePositionScreen() const->Int2
{
	return ms_MousePosition;
}

auto JWOutterWindow::GetCapturedMousePositionScreen() const->Int2
{
	return ms_MouseDownPosition;
}

void JWOutterWindow::ShowJWWindow()
{
	JWWindow::ShowJWWindow();

	ShowWindow(m_hWnd, SW_SHOW);
}

void JWOutterWindow::MinimizeWindow()
{
	JWWindow::MinimizeWindow();

	ShowWindow(m_hWnd, SW_MINIMIZE);

	UpdateWindowSize();
}

void JWOutterWindow::MaximizeWindow()
{
	JWWindow::MaximizeWindow();

	if (m_WindowState == WindowState::Normal)
	{
		SetWindowSize(m_WindowSizePrev);
		SetWindowPosition(m_WindowPositionPrev);
	}
	else
	{
		m_WindowSizePrev = m_WindowSize;
		m_WindowPositionPrev = m_WindowPosition;

		RECT tempRect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&tempRect, 0);
		int W = GetSystemMetrics(SM_CXSCREEN);
		int H = tempRect.bottom - tempRect.top;

		SetWindowSize(Int2(W, H));
		SetWindowPosition(Int2(0, 0));
	}

	UpdateWindowSize();
}

void JWOutterWindow::RestoreWindow()
{
	JWWindow::RestoreWindow();

	ShowWindow(m_hWnd, SW_RESTORE);
	UpdateWindowSize();
}

void JWOutterWindow::ResizeWindow()
{
	JWWindow::ResizeWindow();

	Int2 MovedPos = GetMousePositionClient() - GetMouseDownPositionClient();
	Int2 NewSize = GetCapturedWindowSize();
	Int2 NewPos;

	switch (m_WindowBorder->GetCapturedMoveID())
	{
	case 0:
		// top NS
		NewSize.y -= MovedPos.y;

		NewPos.x = GetCapturedWindowPosition().x;
		NewPos.y = GetCapturedWindowPosition().y + MovedPos.y;
		break;
	case 1:
		// right EW
		NewSize.x += MovedPos.x;

		NewPos.x = GetCapturedWindowPosition().x;
		NewPos.y = GetCapturedWindowPosition().y;
		break;
	case 2:
		// bottom NS
		NewSize.y += MovedPos.y;

		NewPos.x = GetCapturedWindowPosition().x;
		NewPos.y = GetCapturedWindowPosition().y;
		break;
	case 3:
		// left EW
		NewSize.x -= MovedPos.x;

		NewPos.x = GetCapturedWindowPosition().x + MovedPos.x;
		NewPos.y = GetCapturedWindowPosition().y;
		break;
	case 4:
		// top-right NESW
		NewSize.x += MovedPos.x;
		NewSize.y -= MovedPos.y;

		NewPos.x = GetCapturedWindowPosition().x;
		NewPos.y = GetCapturedWindowPosition().y + MovedPos.y;
		break;
	case 5:
		// top-left NESW
		NewSize.x -= MovedPos.x;
		NewSize.y -= MovedPos.y;

		NewPos.x = GetCapturedWindowPosition().x + MovedPos.x;
		NewPos.y = GetCapturedWindowPosition().y + MovedPos.y;
		break;
	case 6:
		// bottom-right NWSE
		NewSize.x += MovedPos.x;
		NewSize.y += MovedPos.y;

		NewPos.x = GetCapturedWindowPosition().x;
		NewPos.y = GetCapturedWindowPosition().y;
		break;
	case 7:
		// bottom-left NWSE
		NewSize.x -= MovedPos.x;
		NewSize.y += MovedPos.y;

		NewPos.x = GetCapturedWindowPosition().x + MovedPos.x;
		NewPos.y = GetCapturedWindowPosition().y;
		break;
	default:
		return;
		break;
	}

	if (NewSize.x < MIN_WINDOW_WIDTH)
		NewSize.x = MIN_WINDOW_WIDTH;

	if (NewSize.y < MIN_WINDOW_HEIGHT)
		NewSize.y = MIN_WINDOW_HEIGHT;

	SetWindowPosition(NewPos);
	SetWindowSize(NewSize);

	m_WindowBorder->SetSize(GetWindowSize());

	UpdateWindowSize();
}

void JWOutterWindow::ShutdownWindow()
{
	JWWindow::ShutdownWindow();

	m_bRunning = false;
}

void JWOutterWindow::UpdateWindowSize()
{
	GetWindowRect(m_hWnd, &m_Rect);

	m_WindowSize.x = m_Rect.right - m_Rect.left;
	m_WindowSize.y = m_Rect.bottom - m_Rect.top;

	m_pDXDevice->UpdateWindowSize();
}