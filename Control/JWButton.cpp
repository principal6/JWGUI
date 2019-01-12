#include "JWButton.h"

using namespace JW_GUI;

JWButton::JWButton()
{
	m_bDrawAlt = false;
}

auto JWButton::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (JW_FAILED(JWControl::Create(pDevice)))
		return Error::ControlNotCreated;

	m_Shape = MAKE_UNIQUE(JWShape)();
	if (JW_FAILED(m_Shape->Create(pDevice)))
		return Error::ShapeNotCreated;

	m_ControlType = CONTROL_TYPE::Button;
	m_ControlState = CONTROL_STATE::Normal;

	return Error::Ok;
}

auto JWButton::MakeSystemButton(BUTTON_TYPE Type, D3DXVECTOR2 Size)->Error
{
	m_Type = Type;

	if (IsSystemButton())
	{
		m_Size = Size;
		m_ColorBackground = JWCOLOR_DARK;
		m_Shape->MakeRectangle(Size, JWCOLOR_DARK);

		SetRegion();

		m_SystemLine = MAKE_UNIQUE(JWLine)();
		if (JW_FAILED(m_SystemLine->Create(m_pDevice)))
			return Error::LineNotCreated;

		m_SystemLineAlt = MAKE_UNIQUE(JWLine)();
		if (JW_FAILED(m_SystemLineAlt->Create(m_pDevice)))
			return Error::LineNotCreated;
		
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

		return Error::Ok;
	}

	return Error::InvalidType;
}

void JWButton::Draw()
{
	CheckState();

	m_Shape->Draw();

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
	m_Shape->SetSize(Size);

	// Set border size
	JWControl::UpdateBorder();

	// System buttons have fixed size, so no update for them

	// Update control region
	SetRegion();
}

void JWButton::SetPosition(D3DXVECTOR2 Position)
{
	m_Position = Position;

	// Set ui position
	m_Shape->SetPosition(m_Position);

	// Set border position
	JWControl::UpdateBorder();

	// Set system line if this is a system button
	if (m_SystemLine)
		m_SystemLine->SetPosition(m_Position);

	if (m_SystemLineAlt)
		m_SystemLineAlt->SetPosition(m_Position);

	if (m_Type == JWButton::BUTTON_TYPE::SystemExit)
	{
		D3DXVECTOR2 orgPos = m_Position;
		D3DXVECTOR2 orgSize = m_Size;

		// For border line offset
		m_Position.y += 2.0f;
		m_Size.x -= 3.0f;
		m_Size.y -= 2.0f;


		SetRegion();

		m_Position = orgPos;
		m_Size = orgSize;
	}
	else
	{
		// Update control region
		SetRegion();
	}
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
		m_Shape->SetColor(JWCOLOR_DARK);
		break;
	case JWButton::CONTROL_STATE::Hover:
		m_Shape->SetColor(JWCOLOR_PLAIN);
		break;
	case JWButton::CONTROL_STATE::Pressed:
		m_Shape->SetColor(JWCOLOR_HIGHLIGHT);
		break;
	default:
		break;
	}
}

void JWButton::ToggleDrawAlt()
{
	m_bDrawAlt = !m_bDrawAlt;
}