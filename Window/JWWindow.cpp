#include "JWWindow.h"

using namespace JW_GUI;

// Static member variables declaration
POINT JWWindow::ms_MousePosition;
POINT JWWindow::ms_CapturedMousePosition;

JWWindow::JWWindow()
{
	m_bCaptured = false;
	m_bCapturedMouseMoved = false;

	m_bResizing = false;
	m_bMoving = false;
}

auto JWWindow::Create(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error
{
	m_WindowState = WindowState::Normal;

	m_CaptionString = Name;

	m_WindowPosition = Position;
	m_WindowSize = WindowSize;
	m_BackColor = BackColor;

	return Error::Ok;
}

auto JWWindow::CreateGUI(JWDXDevice* pDXDevice)->Error
{
	if (nullptr == (m_pDXDevice = pDXDevice))
		return Error::NullDXDevice;

	if (nullptr == (pDXDevice->GetDevice()))
		return Error::NullDevice;

	// Craete window border
	m_WindowBorder = MAKE_UNIQUE(JWThickBorder)();
	if (JW_FAILED(m_WindowBorder->Create(m_pDXDevice->GetDevice())))
		return Error::BorderNotCreated;

	// Create main titlebar
	m_TitleBar = MAKE_UNIQUE(JWTitleBar)();
	if (JW_FAILED(m_TitleBar->Create(m_pDXDevice->GetDevice())))
		return Error::TitlebarNotcreated;

	return Error::Ok;
}

void JWWindow::Destroy()
{
	if (m_pControls.size())
	{
		for (JWControl* iterator : m_pControls)
		{
			delete iterator;
			iterator = nullptr;
		}
	}
}

auto JWWindow::AddControl(JWControl::CONTROL_TYPE ControlType)->JWControl*
{
	JWControl* pNewControl = nullptr;

	switch (ControlType)
	{
	case JW_GUI::JWControl::CONTROL_TYPE::Label:
		pNewControl = new JWLabel();
		break;
	case JW_GUI::JWControl::CONTROL_TYPE::Button:
		pNewControl = new JWButton();
		break;
	case JW_GUI::JWControl::CONTROL_TYPE::ThickBorder:
		pNewControl = new JWThickBorder();
		break;
	case JW_GUI::JWControl::CONTROL_TYPE::TitleBar:
		pNewControl = new JWTitleBar();
		break;
	case JW_GUI::JWControl::CONTROL_TYPE::Image:
		pNewControl = new JWImage();
		break;
	default:
		break;
	}

	pNewControl->Create(m_pDXDevice->GetDevice());

	if (pNewControl)
	{
		m_pControls.push_back(pNewControl);
		return pNewControl;
	}

	return nullptr;
}

auto JWWindow::GetControlPointer(UINT Control_ID)->JWControl* const
{
	if (Control_ID < m_pControls.size())
	{
		return m_pControls[Control_ID];
	}

	return nullptr;
}

void JWWindow::UpdateControls()
{
	if (m_pControls.size())
	{
		for (JWControl* iterator : m_pControls)
		{
			iterator->UpdateState(GetClientMousePosition(), GetClientCapturedMousePosition(), m_bCaptured);
		}
	}

	Int2 MousePos = GetClientMousePosition();
	Int2 CapMousePos = GetClientCapturedMousePosition();

	if (m_WindowState == WindowState::Maximized)
	{
		m_WindowBorder->UpdateState(MousePos, CapMousePos, m_bCaptured, true);
	}
	else
	{
		m_WindowBorder->UpdateState(MousePos, CapMousePos, m_bCaptured);
	}
	
	m_TitleBar->UpdateState(MousePos, CapMousePos, m_bCaptured);
}

void JWWindow::UpdateWindow(bool FreezeWindow)
{
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
		return;
	}

	if (m_bCapturedMouseMoved && !FreezeWindow)
	{
		m_bResizing = m_WindowBorder->IsResizingWindow();
		m_bMoving = m_TitleBar->IsMovingWindow();

		if (m_bResizing)
			m_bMoving = false;

		// @warning: If the window is maximized, it cannot be resized
		if (m_WindowState != WindowState::Maximized)
		{
			/**
			* Resize the window
			*/
			if (m_bResizing)
			{
				ResizeWindow();
			}
			else if (!m_bMoving)
			{
				// Check once!
				if (m_WindowBorder->CanResizeWindow(GetClientCapturedMousePosition()))
				{
					ResizeWindow();
				}
			}

			/**
			* Move the window (if it's not being resized)
			*/
			if (m_bMoving)
			{
				MoveWindow();
			}
			else if (!m_bResizing)
			{
				// Check once!
				if (m_TitleBar->CanMoveWindow(GetClientCapturedMousePosition()))
				{
					MoveWindow();
				}
			}
		}
		else
		{
			// Move window when maximized
			if (!m_bResizing)
			{
				// Check once!
				if (m_TitleBar->CanMoveWindow(GetClientCapturedMousePosition()))
				{
					// @warning:
					// GetClientCapturedMousePosition() should be called before MaximizeWindow()
					// becasue MaximizeWindow() changes the window size and its rect, and so
					// the captured mouse position's ScreenToClient() changes
					Int2 NewCapturedMousePos = GetCapturedMousePosition();

					// Unmaximize the window
					m_TitleBar->ToggleSysMaxButton();
					MaximizeWindow();

					// When maximized, the window's position is (0, 0)
					Int2 NewPos;

					// @warning:
					// without UNMAXIMEZ_PADDING, the pointer touches the minimize button,
					// so it stops moving the window
					if (NewCapturedMousePos.x > m_OutterWindowPosition.x + m_WindowSize.x - m_TitleBar->GetSystemButtonsWidth() - UNMAXIMIZE_PADDING)
					{
						NewCapturedMousePos.x = m_OutterWindowPosition.x + m_WindowSize.x - m_TitleBar->GetSystemButtonsWidth() - UNMAXIMIZE_PADDING;
						SetCapturedMousePosition(NewCapturedMousePos);

						NewPos = GetCapturedWindowPosition() + GetClientMousePosition() - GetClientCapturedMousePosition();
					}

					SetWindowPosition(NewPos);
				}
			}
		}
	}
	else
	{
		// Stop moving and resizing the window
		m_TitleBar->StopWindow();
		m_WindowBorder->StopResizeWindow();
	}
}

void JWWindow::ShowJWWindow()
{
	m_WindowState = WindowState::Normal;
}

void JWWindow::MinimizeWindow()
{
	m_WindowStatePrev = m_WindowState;
	m_WindowState = WindowState::Minimized;
}

void JWWindow::MaximizeWindow()
{
	m_bResized = true;

	if (m_WindowState == WindowState::Normal)
	{
		m_WindowState = WindowState::Maximized;
		return;
	}
	else if (m_WindowState == WindowState::Maximized)
	{
		m_WindowState = WindowState::Normal;
		return;
	}
}

void JWWindow::RestoreWindow()
{
	m_bResized = true;

	if (m_WindowState == WindowState::Minimized)
	{
		// It could be normal size or maximized size
		m_WindowState = m_WindowStatePrev;
	}
}

void JWWindow::ResizeWindow()
{
	m_bResized = true;

	SetCursor(LoadCursor(nullptr, m_WindowBorder->GetCapturedCursorID()));

	Int2 MovedPos = GetClientMousePosition() - GetClientCapturedMousePosition();
	Int2 NewSize = GetCapturedWindowSize();
	Int2 NewPos;

	switch (m_WindowBorder->GetCapturedMoveID())
	{
	case 0:
		// top NS
		NewSize.y -= MovedPos.y;

		MovedPos.x = 0;
		break;
	case 1:
		// right EW
		NewSize.x += MovedPos.x;

		MovedPos = Int2(0, 0);
		break;
	case 2:
		// bottom NS
		NewSize.y += MovedPos.y;

		MovedPos = Int2(0, 0);
		break;
	case 3:
		// left EW
		NewSize.x -= MovedPos.x;

		MovedPos.y = 0;
		break;
	case 4:
		// top-right NESW
		NewSize.x += MovedPos.x;
		NewSize.y -= MovedPos.y;

		MovedPos.x = 0;
		break;
	case 5:
		// top-left NESW
		NewSize.x -= MovedPos.x;
		NewSize.y -= MovedPos.y;

		break;
	case 6:
		// bottom-right NWSE
		NewSize.x += MovedPos.x;
		NewSize.y += MovedPos.y;

		MovedPos = Int2(0, 0);
		break;
	case 7:
		// bottom-left NWSE
		NewSize.x -= MovedPos.x;
		NewSize.y += MovedPos.y;

		MovedPos.y = 0;
		break;
	default:
		break;
	}

	NewPos = GetCapturedWindowPosition() + MovedPos;

	if (NewSize.x < MIN_WINDOW_WIDTH)
		NewSize.x = MIN_WINDOW_WIDTH;

	if (NewSize.y < MIN_WINDOW_HEIGHT)
		NewSize.y = MIN_WINDOW_HEIGHT;

	SetWindowPosition(NewPos);
	SetWindowSize(NewSize);

	UpdateWindowSize();
}

void JWWindow::CaptureWindow()
{
	m_bCaptured = true;

	// Capture the mouse position
	GetCursorPos(&ms_CapturedMousePosition);

	// Capture the window position
	m_CapturedWindowPosition = m_WindowPosition;

	// Capture the window size
	m_CapturedWindowSize = m_WindowSize;
}

void JWWindow::ReleaseWindow()
{
	m_bCaptured = false;
	m_bCapturedMouseMoved = false;
}

void JWWindow::ShutdownWindow()
{
	m_bRunning = false;
}

void JWWindow::UpdateWindowSize()
{
	m_WindowBorder->SetSize(m_WindowSize);
	m_TitleBar->SetSize(m_WindowSize);
}

void JWWindow::OnMouseMove()
{
	GetCursorPos(&ms_MousePosition);

	if (m_bCaptured)
	{
		if ((ms_CapturedMousePosition.x != ms_MousePosition.x) || (ms_CapturedMousePosition.y != ms_MousePosition.y))
		{
			// Captured mouse moved
			m_bCapturedMouseMoved = true;
		}
	}
}

void JWWindow::SetWindowPosition(Int2 Value)
{
	m_WindowPosition = Value;
}

void JWWindow::SetCapturedMousePosition(Int2 Value)
{
	ms_CapturedMousePosition = Value;
}

void JWWindow::SetWindowSize(Int2 Value)
{
	m_WindowSize = Value;
}

auto JWWindow::GetWindowPosition() const->Int2
{
	return m_WindowPosition;
}

auto JWWindow::GetWindowSize() const->Int2
{
	return m_WindowSize;
}

auto JWWindow::GetCapturedWindowPosition() const->Int2
{
	return m_CapturedWindowPosition;
}

auto JWWindow::GetCapturedWindowSize() const->Int2
{
	return m_CapturedWindowSize;
}

auto JWWindow::GetMousePosition() const->Int2
{
	Int2 Result = ms_MousePosition;
	return Result;
}

auto JWWindow::GetCapturedMousePosition() const->Int2
{
	Int2 Result = ms_CapturedMousePosition;
	return Result;
}

auto JWWindow::GetClientMousePosition() const->Int2
{
	Int2 Result = ms_MousePosition;
	Result -= m_WindowPosition;
	return Result;
}

auto JWWindow::GetClientCapturedMousePosition() const->Int2
{
	Int2 Result = ms_CapturedMousePosition;
	Result -= m_WindowPosition;
	return Result;
}

auto JWWindow::IsResized()->bool
{
	if (m_bResized)
	{
		m_bResized = false;
		return true;
	}
		
	return false;
}

auto JWWindow::IsMoving()->bool
{
	return m_bMoving;
}

auto JWWindow::IsResizing()->bool
{
	return m_bResizing;
}