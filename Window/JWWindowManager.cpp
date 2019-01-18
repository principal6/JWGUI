#include "JWWindowManager.h"

using namespace JW_GUI;

inline auto GetWindowShowState(HWND hWnd)->UINT
{
	WINDOWPLACEMENT windowPlacement;
	GetWindowPlacement(hWnd, &windowPlacement);
	return windowPlacement.showCmd;
}

auto JWWindowManager::CreateOutterWindow(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error
{
	// Create the outter window
	m_OutterWindow = MAKE_UNIQUE(JWOutterWindow)();
	if (JW_FAILED(m_OutterWindow->Create(Name, Position, WindowSize, BackColor)))
		return Error::OutterWindowNotCreated;

	// Initialize DirectX
	m_DXDevice = MAKE_UNIQUE(JWDXDevice)();
	if (JW_FAILED(m_DXDevice->Create(m_OutterWindow->GethWnd(), BackColor)))
		return Error::DXDeviceNotCreated;

	// Create window GUI
	if (JW_FAILED(m_OutterWindow->CreateGUI(m_DXDevice.get())))
		return Error::GUINotCreated;

	m_hInstance = m_OutterWindow->GethInstance();
	m_hWnd = m_OutterWindow->GethWnd();

	return Error::Ok;
}

auto JWWindowManager::CreateInnerWindow(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error
{
	if (m_OutterWindow == nullptr)
		return Error::OutterWindowNotCreated;

	// Create an inner window
	m_InnerWindow = MAKE_UNIQUE(JWInnerWindow)();
	if (JW_FAILED(m_InnerWindow->Create(Name, Position, WindowSize, BackColor)))
		return Error::InnerWindowNotCreated;

	// Create window GUI
	if (JW_FAILED(m_InnerWindow->CreateGUI(m_DXDevice.get())))
		return Error::GUINotCreated;

	return Error::Ok;
}

void JWWindowManager::Destroy()
{
	if (m_DXDevice)
		m_DXDevice->Destroy();
	
	if (m_OutterWindow)
		m_OutterWindow->Destroy();

	if (m_InnerWindow)
		m_InnerWindow->Destroy();
}

void JWWindowManager::ShowWindows()
{
	if (m_OutterWindow)
		m_OutterWindow->ShowJWWindow();

	if (m_InnerWindow)
		m_InnerWindow->ShowJWWindow();
}

auto JWWindowManager::MessageHandler(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT
{
	switch (Message)
	{
	case WM_LBUTTONDBLCLK:
		m_OutterWindow->OnMouseDoubleClick();

		if (m_InnerWindow)
			m_InnerWindow->OnMouseDoubleClick();

		break;
	case WM_LBUTTONDOWN:
		if (!m_bWindowsCaptured)
		{
			m_bWindowsCaptured = true;

			m_OutterWindow->CaptureWindow();
			
			if (m_InnerWindow)
				m_InnerWindow->CaptureWindow();
		}
		break;
	case WM_LBUTTONUP:
		if (m_bWindowsCaptured)
		{
			m_bWindowsCaptured = false;

			m_OutterWindow->ReleaseWindow();

			if (m_InnerWindow)
				m_InnerWindow->ReleaseWindow();
		}
		break;
	case WM_MOUSEMOVE:
		m_OutterWindow->OnMouseMove();
		if (m_InnerWindow)
			m_InnerWindow->OnMouseMove();
		break;
	case WM_ACTIVATEAPP:
		if (wParam)
		{
			// Activating the window
			if (GetWindowShowState(hWnd) == SW_SHOWMINIMIZED)
			{
				m_OutterWindow->RestoreWindow();
			}
		}
		break;
	case WM_DESTROY:
		m_OutterWindow->ShutdownWindow();
		return 0;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

auto JWWindowManager::UpdateWindows()->B_RUNNING
{
	B_RUNNING Result = false;

	// Set defualt cursor
	SetCursor(LoadCursor(nullptr, DEFAULT_CURSOR));

	if (m_OutterWindow)
	{
		Result = m_OutterWindow->IsRunning();
		if (Result)
		{
			m_OutterWindow->UpdateWindow();
			m_OutterWindow->UpdateControls();

			if (m_InnerWindow)
			{
				m_InnerWindow->UpdateOutterWindowInfo(m_OutterWindow->GetWindowPosition(), m_OutterWindow->GetWindowSize());

				if (m_OutterWindow->IsResizing() || m_OutterWindow->IsMoving())
				{
					m_InnerWindow->UpdateWindow(true);
				}
				else
				{
					m_InnerWindow->UpdateWindow();
				}

				if (m_OutterWindow->IsResized())
				{
					m_InnerWindow->UpdateMaximizedSize();
				}

				m_InnerWindow->UpdateControls();
			}
		}
	}

	return Result;
}

void JWWindowManager::DrawWindows()
{
	if (m_OutterWindow)
	{
		m_OutterWindow->Draw();

		if (m_InnerWindow)
			m_InnerWindow->Draw();

		m_OutterWindow->DrawUI();
	}
}

auto JWWindowManager::GetInstanceHandle()->HINSTANCE
{
	return m_hInstance;
}

auto JWWindowManager::GetWindowHandle()->HWND
{
	return m_hWnd;
}

auto JWWindowManager::GetOutterWindow()->JWWindow*
{
	return m_OutterWindow.get();
}

auto JWWindowManager::GetInnerWindow()->JWWindow*
{
	return m_InnerWindow.get();
}

auto JWWindowManager::GetDXDevice()->JWDXDevice*
{
	return m_DXDevice.get();
}