#include "JWTitleBar.h"

using namespace JW_GUI;

// Static member variable declaration
const float JWTitlebar::TITLEBAR_HEIGHT = 24.0f;
const float JWTitlebar::ICON_WIDTH = 24.0f;
const float JWTitlebar::ICON_PAD = 4.0f;
const float JWTitlebar::SYSBUTTON_WIDTH = 28.0f;

JWTitlebar::JWTitlebar()
{
	m_bOnSystemMinimize = false;
	m_bOnSystemMaximize = false;
	m_bOnSystemExit = false;
}

auto JWTitlebar::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->Error
{
	if (JW_FAILED(JWControl::Create(pDevice, pFont)))
		return Error::ControlNotCreated;

	m_BG = MAKE_UNIQUE(JWShape)();
	if (JW_FAILED(m_BG->Create(pDevice)))
		return Error::ShapeNotCreated;

	m_Label = MAKE_UNIQUE(JWLabel)();
	if (JW_FAILED(m_Label->Create(pDevice, pFont)))
		return Error::LabelNotCreated;

	m_SysMin = MAKE_UNIQUE(JWButton)();
	if (JW_FAILED(m_SysMin->Create(pDevice, pFont)))
		return Error::SystemButtonNotCreated;

	m_SysMax = MAKE_UNIQUE(JWButton)();
	if (JW_FAILED(m_SysMax->Create(pDevice, pFont)))
		return Error::SystemButtonNotCreated;

	m_SysExit = MAKE_UNIQUE(JWButton)();
	if (JW_FAILED(m_SysExit->Create(pDevice, pFont)))
		return Error::SystemButtonNotCreated;

	m_Icon = MAKE_UNIQUE(JWImage)();
	if (JW_FAILED(m_Icon->Create(pDevice, pFont)))
		return Error::ImageNotCreated;

	m_ControlType = CONTROL_TYPE::TitleBar;
	m_ControlState = CONTROL_STATE::Normal;

	return Error::Ok;
}

void JWTitlebar::Make(Int2 WindowSize, STRING WindowName)
{
	m_BG->MakeRectangle(D3DXVECTOR2(0, 0), JWCOLOR_DARK);

	m_Label->MakeLabel(WindowName, D3DXVECTOR2(0, 0), JWCOLOR_FONT, JWCOLOR_TRANSPARENT);
	m_Label->SetPosition(D3DXVECTOR2(ICON_WIDTH + 2, 1));
	m_Label->SetAlignmentVert(ALIGNMENT_VERT::Center);

	m_SysMin->MakeSystemButton(JWButton::BUTTON_TYPE::SystemMinimize, D3DXVECTOR2(SYSBUTTON_WIDTH, TITLEBAR_HEIGHT));

	m_SysMax->MakeSystemButton(JWButton::BUTTON_TYPE::SystemMaximize, D3DXVECTOR2(SYSBUTTON_WIDTH, TITLEBAR_HEIGHT));

	m_SysExit->MakeSystemButton(JWButton::BUTTON_TYPE::SystemExit, D3DXVECTOR2(SYSBUTTON_WIDTH, TITLEBAR_HEIGHT));

	m_Icon->MakeImage("icon.png", D3DXVECTOR2(ICON_WIDTH - ICON_PAD * 2, ICON_WIDTH - ICON_PAD * 2));

	UpdateSize(WindowSize);
}

void JWTitlebar::Update(JWWinBase* pBase)
{
	UpdateSize(pBase->GetWindowSize());
	UpdateControlStates(pBase);
}

void JWTitlebar::UpdateSize(Int2 WindowSize)
{
	D3DXVECTOR2 NewWindowSize = D3DXVECTOR2(static_cast<float>(WindowSize.x), static_cast<float>(WindowSize.y));
	float LabelSizeX = NewWindowSize.x - SYSBUTTON_WIDTH * 3;

	// Set title bar size
	m_Size = D3DXVECTOR2(LabelSizeX, TITLEBAR_HEIGHT);

	m_BG->SetSize(D3DXVECTOR2(NewWindowSize.x, TITLEBAR_HEIGHT));

	m_Label->SetSize(D3DXVECTOR2(LabelSizeX, TITLEBAR_HEIGHT));

	m_SysMin->SetPosition(D3DXVECTOR2(LabelSizeX, 0));

	m_SysMax->SetPosition(D3DXVECTOR2(LabelSizeX + SYSBUTTON_WIDTH, 0));

	m_SysExit->SetPosition(D3DXVECTOR2(LabelSizeX + SYSBUTTON_WIDTH * 2, 0));

	m_Icon->SetPosition(D3DXVECTOR2(ICON_PAD, ICON_PAD));

	// Update border
	UpdateBorder();

	// For the thick border
	m_Position.x = 2.0f;
	m_Size.x = LabelSizeX - 4.0f;
	m_Position.y = 2.0f;
	m_Size.y = TITLEBAR_HEIGHT - 2.0f;

	// Update control region
	SetRegion();
}

void JWTitlebar::Draw()
{
	m_BG->Draw();
	m_Label->Draw();
	m_SysMin->Draw();
	m_SysMax->Draw();
	m_SysExit->Draw();
	m_Icon->Draw();
}

void JWTitlebar::UpdateControlStates(JWWinBase* pBase)
{
	// Check control states
	UpdateControlState(pBase);
	m_SysMin->UpdateControlState(pBase);
	m_SysMax->UpdateControlState(pBase);
	m_SysExit->UpdateControlState(pBase);
	m_Icon->UpdateControlState(pBase);

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

auto JWTitlebar::OnSystemMinimize()->bool
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

auto JWTitlebar::OnSystemMaximize()->bool
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

auto JWTitlebar::OnSystemExit() const->bool
{
	return m_bOnSystemExit;
}

void JWTitlebar::DoubleClickMaximize(POINT MouePosition)
{
	if (IsMouseOnRegion(MouePosition))
	{
		m_bOnSystemMaximize = true;
	}
}

auto JWTitlebar::CanMoveWindow(POINT CapturedMousePositionClient)->bool
{
	if (!m_bOnWindowMove)
	{
		if (IsMouseOnRegion(CapturedMousePositionClient))
		{
			// Title bar move
			m_bOnWindowMove = true;
		}
	}
	return m_bOnWindowMove;
}

void JWTitlebar::StopWindow()
{
	m_bOnWindowMove = false;
}

void JWTitlebar::ToggleSysMaxButton()
{
	m_SysMax->ToggleDrawAlt();
}