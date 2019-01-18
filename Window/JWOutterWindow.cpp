#include "JWOutterWindow.h"

using namespace JW_GUI;

auto JWOutterWindow::Create(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error
{
	// @warning:
	// You can create only one outter window per process
	if (m_hWnd)
		return Error::OutterWindowAlreadyExists;

	JWWindow::Create(Name, Position, WindowSize, BackColor);

	m_bRunning = true;
	m_hInstance = GetModuleHandle(nullptr);

	WNDCLASSW r_WndClass;
	r_WndClass.cbClsExtra = 0;
	r_WndClass.cbWndExtra = 0;
	r_WndClass.hbrBackground = CreateSolidBrush(JW_GUI::GetXRGB(m_BackColor));
	r_WndClass.hCursor = nullptr; // LoadCursor(nullptr, IDC_ARROW);
	r_WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	r_WndClass.hInstance = m_hInstance;
	r_WndClass.lpfnWndProc = WindowProcedure;
	r_WndClass.lpszClassName = L"OUTTERWINDOW";
	r_WndClass.lpszMenuName = nullptr;
	r_WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClassW(&r_WndClass);

	if (nullptr == (m_hWnd = CreateWindowExW(0L, r_WndClass.lpszClassName, r_WndClass.lpszClassName, WS_POPUP,
		Position.x, Position.y, WindowSize.x, WindowSize.y, nullptr, (HMENU)nullptr, m_hInstance, nullptr)))
		return Error::WindowNotCreated;

	UnregisterClassW(r_WndClass.lpszClassName, m_hInstance);

	return Error::Ok;
}

auto JWOutterWindow::CreateGUI(JWDXDevice* pDXDevice)->Error
{
	if (JW_FAILED(JWWindow::CreateGUI(pDXDevice)))
		return Error::GUINotCreated;

	m_WindowBorder->MakeThickBorder(D3DXVECTOR2(static_cast<float>(m_WindowSize.x - 1),
		static_cast<float>(m_WindowSize.y - 1)));

	m_TitleBar->MakeOutter(m_WindowSize, m_CaptionString);

	return Error::Ok;
}

void JWOutterWindow::Draw()
{
	// Draw controls
	if (m_pControls.size())
	{
		for (JWControl* iterator : m_pControls)
		{
			iterator->Draw();
		}
	}
}

void JWOutterWindow::DrawUI()
{
	// Draw the title bar
	m_TitleBar->Draw();

	// Draw the thick border, but when the window is maximized, it's not drawn
	if (m_WindowState != WindowState::Maximized)
		m_WindowBorder->Draw();
}

void JWOutterWindow::CaptureWindow()
{
	JWWindow::CaptureWindow();

	// Capture the window (for the case when the mouse goes out of the window region)
	SetCapture(m_hWnd);
}

void JWOutterWindow::ReleaseWindow()
{
	JWWindow::ReleaseWindow();

	// Release the captured window
	ReleaseCapture();
}

void JWOutterWindow::OnMouseDoubleClick()
{
	m_TitleBar->DoubleClickMaximize(GetClientMousePosition());
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
	return m_pDXDevice->GetDevice();
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

void JWOutterWindow::MoveWindow()
{
	Int2 CapMousePos = GetClientCapturedMousePosition();
	Int2 MousePos = GetClientMousePosition();
	Int2 WinPos = GetCapturedWindowPosition();

	Int2 NewPos = WinPos + MousePos - CapMousePos;

	SetWindowPosition(NewPos);
}

void JWOutterWindow::UpdateWindowSize()
{
	GetWindowRect(m_hWnd, &m_Rect);

	m_WindowSize.x = m_Rect.right - m_Rect.left;
	m_WindowSize.y = m_Rect.bottom - m_Rect.top;

	JWWindow::UpdateWindowSize();

	m_pDXDevice->UpdateWindowSize();
}

auto JWOutterWindow::IsRunning()->B_RUNNING
{
	return m_bRunning;
}