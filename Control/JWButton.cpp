#include "JWButton.h"

using namespace JW_GUI;

JWButton::JWButton()
{
	m_bDrawAlt = false;

	// Set to deafult colors
	m_ColorNormal = JWCOLOR_DARK;
	m_ColorHover = JWCOLOR_PLAIN;
	m_ColorPressed = JWCOLOR_HIGHLIGHT;
}

auto JWButton::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (JW_FAILED(JWControl::Create(pDevice)))
		return Error::ControlNotCreated;

	m_BG = MAKE_UNIQUE(JWShape)();
	if (JW_FAILED(m_BG->Create(pDevice)))
		return Error::ShapeNotCreated;

	m_ControlType = CONTROL_TYPE::Button;
	m_ControlState = CONTROL_STATE::Normal;

	return Error::Ok;
}

void JWButton::MakeCommandButton(WSTRING Text, D3DXVECTOR2 Size)
{
	m_Type = JWButton::BUTTON_TYPE::CommandButton;
	m_Text = Text;

	m_Font->MakeFont(FONT_NAME);
	m_Font->SetText(Text);

	// Not system button
	m_BG->MakeRectangle(Size, m_ColorNormal);
	SetSize(Size);

	// Create border line
	JWControl::MakeBorder();

	return;
}

void JWButton::MakeSystemButton(BUTTON_TYPE Type, D3DXVECTOR2 Size)
{
	m_Type = Type;

	if (!IsSystemButton())
		return;

	m_BG->MakeRectangle(Size, m_ColorNormal);
	SetSize(Size);

	m_SystemLine = MAKE_UNIQUE(JWLine)();
	if (JW_FAILED(m_SystemLine->Create(m_pDevice)))
		return;

	m_SystemLineAlt = MAKE_UNIQUE(JWLine)();
	if (JW_FAILED(m_SystemLineAlt->Create(m_pDevice)))
		return;
		
	D3DXVECTOR2 RatioSize;
	D3DXVECTOR2 NewSize;
	float tempX, tempY;

	switch (Type)
	{	
	case BUTTON_TYPE::SystemExit:
		// Maintain the ratio of the system mark
		RatioSize.x = min(Size.x, Size.y);
		RatioSize.y = RatioSize.x;
			
		NewSize.x = RatioSize.x * 0.3f;
		NewSize.y = RatioSize.y * 0.3f;

		tempX = (Size.x - NewSize.x) / 2;
		tempY = (Size.y - NewSize.y) / 2;

		m_SystemLine->AddLine(D3DXVECTOR2(tempX, tempY), D3DXVECTOR2(NewSize.x, NewSize.y), JWCOLOR_SYSTEM_MARK);
		m_SystemLine->AddLine(D3DXVECTOR2(tempX + 1.0f, tempY), D3DXVECTOR2(NewSize.x, NewSize.y), JWCOLOR_SYSTEM_MARK);
		m_SystemLine->AddLine(D3DXVECTOR2(tempX + NewSize.x, tempY),
			D3DXVECTOR2(-NewSize.x, NewSize.y), JWCOLOR_SYSTEM_MARK);
		m_SystemLine->AddLine(D3DXVECTOR2(tempX + NewSize.x + 1.0f, tempY),
			D3DXVECTOR2(-NewSize.x, NewSize.y), JWCOLOR_SYSTEM_MARK);
		m_SystemLine->AddEnd();
		break;
	case BUTTON_TYPE::SystemMinimize:
		// Maintain the ratio of the system mark
		RatioSize.x = min(Size.x, Size.y);
		RatioSize.y = RatioSize.x;

		NewSize.x = RatioSize.x * 0.3f;
		NewSize.y = RatioSize.y * 0.8f;

		tempX = (Size.x - NewSize.x) / 2;
		tempY = Size.y - (NewSize.y / 2);

		m_SystemLine->AddLine(D3DXVECTOR2(tempX, tempY), D3DXVECTOR2(NewSize.x, 0), JWCOLOR_SYSTEM_MARK);
		m_SystemLine->AddLine(D3DXVECTOR2(tempX, tempY + 1.0f), D3DXVECTOR2(NewSize.x, 0), JWCOLOR_SYSTEM_MARK);
		m_SystemLine->AddEnd();

		break;
	case BUTTON_TYPE::SystemMaximize:
		// Maintain the ratio of the system mark
		RatioSize.x = min(Size.x, Size.y);
		RatioSize.y = RatioSize.x;

		NewSize.x = RatioSize.x * 0.3f;
		NewSize.y = RatioSize.y * 0.3f;

		tempX = (Size.x - NewSize.x) / 2;
		tempY = (Size.y - NewSize.y) / 2;

		m_SystemLine->AddBox(D3DXVECTOR2(tempX, tempY), D3DXVECTOR2(NewSize.x, NewSize.y), JWCOLOR_SYSTEM_MARK);
		m_SystemLine->AddBox(D3DXVECTOR2(tempX + 1.0f, tempY + 1.0f),
			D3DXVECTOR2(NewSize.x - 2.0f, NewSize.y - 2.0f), JWCOLOR_SYSTEM_MARK);
		m_SystemLine->AddEnd();

		m_SystemLineAlt->AddBox(D3DXVECTOR2(tempX + 1.0f, tempY + 1.0f),
			D3DXVECTOR2(NewSize.x - 1.0f, NewSize.y - 1.0f), JWCOLOR_SYSTEM_MARK);
		m_SystemLineAlt->AddBox(D3DXVECTOR2(tempX + 2.0f, tempY + 2.0f),
			D3DXVECTOR2(NewSize.x - 3.0f, NewSize.y - 3.0f), JWCOLOR_SYSTEM_MARK);

		m_SystemLineAlt->AddLine(D3DXVECTOR2(tempX + 3.0f, tempY - 1.0f),
			D3DXVECTOR2(NewSize.x - 1.0f, 0), JWCOLOR_SYSTEM_MARK);
		m_SystemLineAlt->AddLine(D3DXVECTOR2(tempX + 2.0f + NewSize.x, tempY - 2.0f),
			D3DXVECTOR2(0, NewSize.y), JWCOLOR_SYSTEM_MARK);

		m_SystemLineAlt->AddEnd();
		break;
	default:
		break;
	}

	// Create border line
	JWControl::MakeBorder();
}

void JWButton::Draw()
{
	CheckState();

	m_BG->Draw();

	if (IsSystemButton())
	{
		// DrawControls system mark only when it's system button
		if (m_bDrawAlt)
		{
			m_SystemLineAlt->Draw();
		}
		else
		{
			m_SystemLine->Draw();
		}
	}
	else
	{
		// Draw text only when it's not system button
		JWControl::DrawString();
	}

	// Draw border
	JWControl::DrawBorder();
}

void JWButton::SetSize(D3DXVECTOR2 Size)
{
	m_Size = Size;

	// Set ui size
	m_BG->SetSize(Size);

	// Set border size
	JWControl::UpdateBorder();

	// System buttons have fixed size, so no update for them

	// Update control region
	SetRegion(m_Position);
}

void JWButton::SetControlPosition(D3DXVECTOR2 Position)
{
	// Set control position
	JWControl::SetControlPosition(Position);

	// Set border position
	JWControl::UpdateBorder();

	// Set background position
	m_BG->SetPosition(Position + m_InnerWindowPosition);

	// Set text position
	m_Font->SetPosition(Position + m_InnerWindowPosition);

	// Set system line position if this is a system button
	if (m_SystemLine)
		m_SystemLine->SetPosition(Position + m_InnerWindowPosition);

	if (m_SystemLineAlt)
		m_SystemLineAlt->SetPosition(Position + m_InnerWindowPosition);

	if (m_Type == JWButton::BUTTON_TYPE::SystemExit)
	{
		D3DXVECTOR2 orgPos = m_Position;
		D3DXVECTOR2 orgSize = m_Size;

		// For border line offset
		m_Position.y += 2.0f;
		m_Size.x -= 3.0f;
		m_Size.y -= 2.0f;

		// Update control region
		SetRegion(m_Position);

		m_Position = orgPos;
		m_Size = orgSize;
	}
	else
	{
		// Update control region
		SetRegion(m_Position);
	}
}

void JWButton::SetColorNormal(DWORD Color)
{
	m_ColorNormal = Color;
}

void JWButton::SetColorHover(DWORD Color)
{
	m_ColorHover = Color;
}

void JWButton::SetColorPressed(DWORD Color)
{
	m_ColorPressed = Color;
}

auto JWButton::IsSystemButton()->bool
{
	if (m_Type == BUTTON_TYPE::SystemMinimize ||
		m_Type == BUTTON_TYPE::SystemMaximize ||
		m_Type == BUTTON_TYPE::SystemExit)
		return true;
	return false;
}

void JWButton::CheckState()
{
	switch (m_ControlState)
	{
	case JWButton::CONTROL_STATE::Normal:
		m_BG->SetXRGB(m_ColorNormal);
		break;
	case JWButton::CONTROL_STATE::Hover:
		m_BG->SetXRGB(m_ColorHover);
		break;
	case JWButton::CONTROL_STATE::Pressed:
		m_BG->SetXRGB(m_ColorPressed);
		break;
	default:
		break;
	}
}

void JWButton::ToggleDrawAlt()
{
	m_bDrawAlt = !m_bDrawAlt;
}