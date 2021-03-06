#include "JWTitleBar.h"

using namespace JW_GUI;

// Static member variable declaration
const float JWTitleBar::TITLEBAR_SPARE_WIDTH = 100.0f;
const float JWTitleBar::TITLEBAR_HEIGHT = 24.0f;
const float JWTitleBar::ICON_WIDTH = 24.0f;
const float JWTitleBar::ICON_PAD = 4.0f;
const float JWTitleBar::SYSBUTTON_WIDTH = 28.0f;

inline auto JWTitleBar::GetLabelSizeX()->float
{
	if (m_bInnerWindow)
	{
		return static_cast<float>(m_WindowSize.x - SYSBUTTON_WIDTH * 2);
	}
	else
	{
		return static_cast<float>(m_WindowSize.x - SYSBUTTON_WIDTH * 3);
	}
}

JWTitleBar::JWTitleBar()
{
	m_bOnSystemMinimize = false;
	m_bOnSystemMaximize = false;
	m_bOnSystemExit = false;
	m_bInnerWindow = false;
}

auto JWTitleBar::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (JW_FAILED(JWControl::Create(pDevice)))
		return Error::ControlNotCreated;

	m_BG = MAKE_UNIQUE(JWShape)();
	if (JW_FAILED(m_BG->Create(pDevice)))
		return Error::ShapeNotCreated;

	m_Label = MAKE_UNIQUE(JWLabel)();
	if (JW_FAILED(m_Label->Create(pDevice)))
		return Error::LabelNotCreated;

	m_SysMin = MAKE_UNIQUE(JWButton)();
	if (JW_FAILED(m_SysMin->Create(pDevice)))
		return Error::SystemButtonNotCreated;

	m_SysMax = MAKE_UNIQUE(JWButton)();
	if (JW_FAILED(m_SysMax->Create(pDevice)))
		return Error::SystemButtonNotCreated;

	m_SysExit = MAKE_UNIQUE(JWButton)();
	if (JW_FAILED(m_SysExit->Create(pDevice)))
		return Error::SystemButtonNotCreated;

	m_Icon = MAKE_UNIQUE(JWImage)();
	if (JW_FAILED(m_Icon->Create(pDevice)))
		return Error::ImageNotCreated;

	m_ControlType = CONTROL_TYPE::TitleBar;
	m_ControlState = CONTROL_STATE::Normal;

	return Error::Ok;
}

void JWTitleBar::MakeOutter(Int2 WindowSize, WSTRING WindowName)
{
	m_BG->MakeRectangle(D3DXVECTOR2(0, 0), JWCOLOR_DARK);

	m_Label->MakeLabel(WindowName, D3DXVECTOR2(0, 0), JWCOLOR_FONT, JWCOLOR_TRANSPARENT);
	m_Label->SetAlignmentVert(ALIGNMENT_VERT::Center);

	m_SysMin->MakeSystemButton(JWButton::BUTTON_TYPE::SystemMinimize, D3DXVECTOR2(SYSBUTTON_WIDTH, TITLEBAR_HEIGHT));

	m_SysMax->MakeSystemButton(JWButton::BUTTON_TYPE::SystemMaximize, D3DXVECTOR2(SYSBUTTON_WIDTH, TITLEBAR_HEIGHT));

	m_SysExit->MakeSystemButton(JWButton::BUTTON_TYPE::SystemExit, D3DXVECTOR2(SYSBUTTON_WIDTH, TITLEBAR_HEIGHT));

	m_Icon->MakeImage(L"icon.png", D3DXVECTOR2(ICON_WIDTH - ICON_PAD * 2, ICON_WIDTH - ICON_PAD * 2));

	m_WindowSize = WindowSize;
	m_Position = D3DXVECTOR2(0, 0);

	UpdateSizeAndPosition();
}

void JWTitleBar::MakeInner(Int2 WindowSize, WSTRING WindowName)
{
	m_bInnerWindow = true;

	m_BG->MakeRectangle(D3DXVECTOR2(0, 0), JWCOLOR_MIDDLE);

	m_Label->MakeLabel(WindowName, D3DXVECTOR2(0, 0), JWCOLOR_FONT, JWCOLOR_TRANSPARENT);
	m_Label->SetAlignmentVert(ALIGNMENT_VERT::Center);

	m_SysMax->MakeSystemButton(JWButton::BUTTON_TYPE::SystemMaximize, D3DXVECTOR2(SYSBUTTON_WIDTH, TITLEBAR_HEIGHT));
	m_SysMax->SetColorNormal(JWCOLOR_MIDDLE);

	m_SysExit->MakeSystemButton(JWButton::BUTTON_TYPE::SystemExit, D3DXVECTOR2(SYSBUTTON_WIDTH, TITLEBAR_HEIGHT));
	m_SysExit->SetColorNormal(JWCOLOR_MIDDLE);

	m_Icon->MakeImage(L"icon.png", D3DXVECTOR2(ICON_WIDTH - ICON_PAD * 2, ICON_WIDTH - ICON_PAD * 2));

	m_WindowSize = WindowSize;
	m_Position = D3DXVECTOR2(0, 0);
}

void JWTitleBar::UpdateSizeAndPosition()
{
	// Set title bar size
	m_Size = D3DXVECTOR2(GetLabelSizeX(), TITLEBAR_HEIGHT - 4.0f);
	m_Label->SetSize(D3DXVECTOR2(GetLabelSizeX(), TITLEBAR_HEIGHT));
	m_Label->SetControlPosition(D3DXVECTOR2(m_Position.x + ICON_WIDTH + 2, m_Position.y + 1));

	if (m_bInnerWindow)
	{
		m_BG->SetSize(D3DXVECTOR2(m_WindowSize.x, TITLEBAR_HEIGHT));

		m_SysMax->SetControlPosition(D3DXVECTOR2(m_Position.x + GetLabelSizeX(), m_Position.y));

		m_SysExit->SetControlPosition(D3DXVECTOR2(m_Position.x + GetLabelSizeX() + SYSBUTTON_WIDTH, m_Position.y));
	}
	else
	{
		m_BG->SetSize(D3DXVECTOR2(m_WindowSize.x + TITLEBAR_SPARE_WIDTH, TITLEBAR_HEIGHT));

		m_SysMin->SetControlPosition(D3DXVECTOR2(m_Position.x + GetLabelSizeX(), m_Position.y));

		m_SysMax->SetControlPosition(D3DXVECTOR2(m_Position.x + GetLabelSizeX() + SYSBUTTON_WIDTH, m_Position.y));

		m_SysExit->SetControlPosition(D3DXVECTOR2(m_Position.x + GetLabelSizeX() + SYSBUTTON_WIDTH * 2, m_Position.y));
	}

	m_Icon->SetControlPosition(D3DXVECTOR2(m_Position.x + ICON_PAD, m_Position.y + ICON_PAD));

	// Update border
	UpdateBorder();

	// Update control region (with padding for the thick border)
	SetRegion(D3DXVECTOR2(m_Position.x + 2.0f, m_Position.y + 2.0f));
}

void JWTitleBar::Draw()
{
	m_BG->Draw();
	m_Label->Draw();

	if (!m_bInnerWindow)
		m_SysMin->Draw();

	m_SysMax->Draw();
	m_SysExit->Draw();
	m_Icon->Draw();
}

void JWTitleBar::UpdateState(Int2 MousePosition, Int2 MouseDownPosition, bool IsLeftButtonDown, bool WindowMaximized,
	bool StayPressed)
{
	// Check control states
	JWControl::UpdateState(MousePosition, MouseDownPosition, IsLeftButtonDown, WindowMaximized, StayPressed);

	m_SysMin->UpdateState(MousePosition, MouseDownPosition, IsLeftButtonDown, WindowMaximized, StayPressed);
	m_SysMax->UpdateState(MousePosition, MouseDownPosition, IsLeftButtonDown, WindowMaximized, StayPressed);
	m_SysExit->UpdateState(MousePosition, MouseDownPosition, IsLeftButtonDown, WindowMaximized, StayPressed);
	m_Icon->UpdateState(MousePosition, MouseDownPosition, IsLeftButtonDown, WindowMaximized, StayPressed);

	if (m_SysMin->GetControlState() == JWControl::CONTROL_STATE::Clicked)
	{
		m_bOnSystemMinimize = true;
		return;
	}

	if (m_SysMax->GetControlState() == JWControl::CONTROL_STATE::Clicked)
	{
		m_bOnSystemMaximize = true;
		return;
	}

	if (m_SysExit->GetControlState() == JWControl::CONTROL_STATE::Clicked)
	{
		m_bOnSystemExit = true;
		return;
	}
}

auto JWTitleBar::OnSystemMinimize()->bool
{
	if (m_bOnSystemMinimize)
	{
		m_bOnSystemMinimize = false;
		return true;
	}
	else
	{
		return false;
	}
}

auto JWTitleBar::OnSystemMaximize()->bool
{
	if (m_bOnSystemMaximize)
	{
		m_bOnSystemMaximize = false;
		m_SysMax->ToggleDrawAlt();
		return true;
	}
	else
	{
		return false;
	}
}

auto JWTitleBar::OnSystemExit() const->bool
{
	return m_bOnSystemExit;
}

void JWTitleBar::DoubleClickMaximize(POINT MouePosition)
{
	if (IsMouseOnRegion(MouePosition))
	{
		m_bOnSystemMaximize = true;
	}
}

auto JWTitleBar::CanMoveWindow(POINT CapturedMousePositionClient)->bool
{
	if (IsMouseOnRegion(CapturedMousePositionClient))
	{
		m_bOnWindowMove = true;
	}
	else
	{
		m_bOnWindowMove = false;
	}

	return m_bOnWindowMove;
}

auto JWTitleBar::IsMovingWindow()->bool
{
	return m_bOnWindowMove;
}

void JWTitleBar::StopWindow()
{
	m_bOnWindowMove = false;
}

void JWTitleBar::ToggleSysMaxButton()
{
	m_SysMax->ToggleDrawAlt();
}

void JWTitleBar::SetSize(D3DXVECTOR2 WindowSize)
{
	m_WindowSize = WindowSize;

	UpdateSizeAndPosition();
}

void JWTitleBar::SetControlPosition(D3DXVECTOR2 Position)
{
	JWControl::SetControlPosition(Position);

	UpdateSizeAndPosition();
}

void JWTitleBar::SetWindowOffset(D3DXVECTOR2 InnerWindowPosition, D3DXVECTOR2 OutterWindowPosition)
{
	JWControl::SetWindowOffset(InnerWindowPosition, OutterWindowPosition);

	// Inner window's title bar
	m_BG->SetPosition(InnerWindowPosition);

	m_Label->SetWindowOffset(InnerWindowPosition, OutterWindowPosition);
	m_SysMin->SetWindowOffset(InnerWindowPosition, OutterWindowPosition);
	m_SysMax->SetWindowOffset(InnerWindowPosition, OutterWindowPosition);
	m_SysExit->SetWindowOffset(InnerWindowPosition, OutterWindowPosition);
	m_Icon->SetWindowOffset(InnerWindowPosition, OutterWindowPosition);
}

auto JWTitleBar::GetSystemButtonsWidth()->int
{
	if (m_bInnerWindow)
	{
		return static_cast<int>(SYSBUTTON_WIDTH * 2);
	}
	else
	{
		return static_cast<int>(SYSBUTTON_WIDTH * 3);
	}
}

auto JWTitleBar::GetTitleBarHeight()->int
{
	return static_cast<int>(TITLEBAR_HEIGHT);
}