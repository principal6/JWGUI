#include "JWControl.h"

using namespace JW_GUI;

JWControl::JWControl()
{
	m_bDrawBorder = false;
	m_ColorFont = JWCOLOR_FONT;
}

auto JWControl::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (nullptr == (m_pDevice = pDevice))
		return Error::NullDevice;

	m_Font = MAKE_UNIQUE(JWFont)();
	if (JW_FAILED(m_Font->Create(m_pDevice)))
		return Error::FontNotCreated;

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
	m_Font->Draw();
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

void JWControl::SetFontAlpha(BYTE Alpha)
{
	JW_GUI::SetAlpha(&m_ColorFont, Alpha);

	m_Font->SetAlpha(Alpha);
}

void JWControl::SetFontXRGB(DWORD XRGB)
{
	JW_GUI::SetXRGB(&m_ColorFont, XRGB);

	m_Font->SetXRGB(m_ColorFont);
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

void JWControl::UpdateState(Int2 MousePosition, Int2 MouseDownPosition, bool IsLeftButtonDown)
{
	m_MousePosition = MousePosition;
	m_MouseDownPosition = MouseDownPosition;

	// Currently the mouse pointer is in the region
	if (IsMouseOnRegion(m_MousePosition))
	{
		// When the left button is pressed
		if (IsLeftButtonDown)
		{
			// If the button was pressed in the region
			if (IsMouseOnRegion(m_MouseDownPosition))
			{
				SetControlState(JWControl::CONTROL_STATE::Pressed);
				return;
			}
		}
		else // When no button is pressed
		{
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