#include "JWControl.h"

using namespace JW_GUI;

JWControl::JWControl()
{
	m_bDrawBorder = false;
	m_FontColor = JWCOLOR_FONT;
}

auto JWControl::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->Error
{
	if (nullptr == (m_pDevice = pDevice))
		return Error::NullDevice;

	if (nullptr == (m_pFont = pFont))
		return Error::NullFont;

	return Error::Ok;
}

auto JWControl::MakeBorder()->Error
{
	m_Border = MAKE_UNIQUE(JWBorder)();
	if (JW_FAILED(m_Border->Create(m_pDevice)))
		return Error::BorderNotCreated;

	m_Border->MakeBorder(m_Size);
	return Error::Ok;
}

void JWControl::UpdateBorder()
{
	if (m_Border)
		m_Border->UpdateBorder(m_Size);
}

void JWControl::DrawBorder()
{
	if (m_Border && m_bDrawBorder)
		m_Border->Draw();
}

void JWControl::DrawString()
{
	SetRect(&m_RectFont, static_cast<int>(m_Position.x), static_cast<int>(m_Position.y),
		static_cast<int>(m_Position.x + m_Size.x), static_cast<int>(m_Position.y + m_Size.y));

	m_pFont->DrawTextA(nullptr, m_Text.c_str(), -1, &m_RectFont, DT_LEFT | DT_VCENTER | DT_NOCLIP, m_FontColor);
}

void JWControl::SetRegion()
{
	m_Region = REGION(m_Position, m_Size);
}

void JWControl::SetControlState(CONTROL_STATE State)
{
	m_ControlState = State;
}

void JWControl::SetAlignmentHorz(ALIGNMENT_HORZ Align)
{
	m_AlignmentHorz = Align;
}

void JWControl::SetAlignmentVert(ALIGNMENT_VERT Align)
{
	m_AlignmentVert = Align;
}

void JWControl::SetDrawBorder(bool Value)
{
	m_bDrawBorder = Value;
}

void JWControl::SetFontColor(DWORD Color)
{
	m_FontColor = Color;
}

auto JWControl::GetControlType() const->JWControl::CONTROL_TYPE
{
	return m_ControlType;
}

auto JWControl::GetPosition() const->D3DXVECTOR2
{
	return m_Position;
}

auto JWControl::GetRegion() const->JWControl::REGION
{
	return m_Region;
}

auto JWControl::GetControlState() const->JWControl::CONTROL_STATE
{
	return m_ControlState;
}

auto JWControl::GetAlignmentHorz() const->ALIGNMENT_HORZ
{
	return m_AlignmentHorz;
}

auto JWControl::GetAlignmentVert() const->ALIGNMENT_VERT
{
	return m_AlignmentVert;
}

auto JWControl::GetDrawBorder() const->bool
{
	return m_bDrawBorder;
}

auto JWControl::IsMouseOnRegion(Int2 MousePosition) const->bool
{
	if ((MousePosition.x >= m_Region.Left) && (MousePosition.x <= m_Region.Right) &&
		(MousePosition.y >= m_Region.Top) && (MousePosition.y <= m_Region.Bottom))
	{
		return true;
	}
	return false;
}

void JWControl::UpdateControlState(JWWinBase* pBase)
{
	if (IsMouseOnRegion(pBase->GetMousePositionClient()))
	{
		// Currently the mouse pointer is in the region
		if (pBase->IsMouseLeftButtonDown())
		{
			// When the left button is pressed
			if (IsMouseOnRegion(pBase->GetCapturedMousePositionClient()))
			{
				// When the button is pressed in the region
				SetControlState(JWControl::CONTROL_STATE::Pressed);
				return;
			}
		}
		else
		{
			// When no button is pressed
			if (GetControlState() == JWControl::CONTROL_STATE::Pressed)
			{
				SetControlState(JWControl::CONTROL_STATE::Clicked);
				return;
			}
			else
			{
				SetControlState(JWControl::CONTROL_STATE::Hover);
				return;
			}
		}
	}
	else
	{
		// Currently the mouse pointer is out of the region
		SetControlState(JWControl::CONTROL_STATE::Normal);
		return;
	}
}