#include "JWWindow.h"

using namespace JW_GUI;

// Static member variables declaration
bool JWWindow::ms_IsWindowCaptured = false;
bool JWWindow::ms_onMouseMove = false;
bool JWWindow::ms_onMouseDoubleCliked = false;
POINT JWWindow::ms_MousePosition;
POINT JWWindow::ms_CapturedMousePosition;

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

	if (nullptr == (m_pDevice = pDXDevice->GetDevice()))
		return Error::NullDevice;

	// Craete window border
	m_WindowBorder = MAKE_UNIQUE(JWThickBorder)();
	if (JW_FAILED(m_WindowBorder->Create(m_pDevice)))
		return Error::BorderNotCreated;
	m_WindowBorder->MakeThickBorder(D3DXVECTOR2(static_cast<float>(m_WindowSize.x - 1),
		static_cast<float>(m_WindowSize.y - 1)));

	// Create main titlebar
	m_TitleBar = MAKE_UNIQUE(JWTitleBar)();
	if (JW_FAILED(m_TitleBar->Create(m_pDevice)))
		return Error::TitlebarNotcreated;
	m_TitleBar->Make(m_WindowSize, m_CaptionString);


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

void JWWindow::AddControl(JWControl::CONTROL_TYPE ControlType)
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
	}
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
	m_WindowBorder->UpdateState(GetMousePosition(), GetCapturedMousePosition(), OnMouseLeftButtonDown());
	m_TitleBar->UpdateState(GetMousePosition(), GetCapturedMousePosition(), OnMouseLeftButtonDown());

	if (m_pControls.size())
	{
		for (JWControl* iterator : m_pControls)
		{
			iterator->UpdateState(GetMousePosition(), GetCapturedMousePosition(), OnMouseLeftButtonDown());
		}
	}
}

void JWWindow::ShowJWWindow()
{
	m_WindowState = WindowState::Normal;
}

void JWWindow::MinimizeWindow()
{
	m_WindowState = WindowState::Minimized;
}

void JWWindow::MaximizeWindow()
{
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
	if (m_WindowState == WindowState::Minimized)
	{
		m_WindowState = WindowState::Normal;
	}
}

auto JWWindow::OnMouseLeftButtonDown() const->bool
{
	return ms_IsWindowCaptured;
}

auto JWWindow::OnMouseMove() const->bool
{
	return ms_onMouseMove;
}

auto JWWindow::OnCapturedMouseMove() const->bool
{
	if (JWWindow::ms_IsWindowCaptured)
	{
		if ((ms_CapturedMousePosition.x != ms_MousePosition.x) || (ms_CapturedMousePosition.y != ms_MousePosition.y))
		{
			return true;
		}
	}
	return false;
}

auto JWWindow::OnMouseDoubleClicked()->bool
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
	POINT Result = ms_MousePosition;
	ScreenToClient(m_hWnd, &Result);
	return Result;
}

auto JWWindow::GetCapturedMousePosition() const->Int2
{
	POINT Result = ms_CapturedMousePosition;
	ScreenToClient(m_hWnd, &Result);
	return Result;
}