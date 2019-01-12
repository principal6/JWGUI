#include "JWThickBorder.h"

using namespace JW_GUI;

JWThickBorder::JWThickBorder()
{
	m_bCanResizeWindow = false;
}

auto JWThickBorder::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->Error
{
	if (JW_FAILED(JWControl::Create(pDevice, pFont)))
		return Error::ControlNotCreated;

	m_ControlType = CONTROL_TYPE::ThickBorder;
	m_ControlState = CONTROL_STATE::Normal;

	SetDrawBorder(true);

	return Error::Ok;
}

void JWThickBorder::MakeThickBorder(D3DXVECTOR2 Size)
{
	m_Size = Size;

	// Create border line
	JWControl::MakeBorder();

	// Update control region
	SetRegion();
}

void JWThickBorder::Draw()
{
	// Draw border
	JWControl::DrawBorder();
}

void JWThickBorder::SetSize(D3DXVECTOR2 Size)
{
	m_Size.x = Size.x - 1;
	m_Size.y = Size.y - 1;

	// Set border size
	JWControl::UpdateBorder();

	// Update control region
	SetRegion();
}

void JWThickBorder::SetRegion()
{
	m_Region[0] = REGION(m_Position, D3DXVECTOR2(m_Size.x, 1.0f)); // top
	m_Region[1] = REGION(D3DXVECTOR2(m_Position.x + m_Size.x - 1.0f, m_Position.y), D3DXVECTOR2(1.0f, m_Size.y)); // right
	m_Region[2] = REGION(D3DXVECTOR2(m_Position.x, m_Position.y + m_Size.y - 1.0f), D3DXVECTOR2(m_Size.x, 1.0f)); // bottom
	m_Region[3] = REGION(m_Position, D3DXVECTOR2(1.0f, m_Size.y)); // left
	
	m_Region[4] = REGION(D3DXVECTOR2(m_Position.x + m_Size.x - 2.0f, m_Position.y), D3DXVECTOR2(2.0f, 2.0f)); // top-right
	m_Region[5] = REGION(D3DXVECTOR2(m_Position.x, m_Position.y), D3DXVECTOR2(2.0f, 2.0f)); // top-left
	m_Region[6] = REGION(D3DXVECTOR2(m_Position.x + m_Size.x - 2.0f, m_Position.y + m_Size.y - 2.0f),
		D3DXVECTOR2(2.0f, 2.0f)); // bottom-right
	m_Region[7] = REGION(D3DXVECTOR2(m_Position.x, m_Position.y + m_Size.y - 2.0f),
		D3DXVECTOR2(2.0f, 2.0f)); // bottom-left
}

auto JWThickBorder::IsMouseOnRegion(Int2 MousePosition)->bool
{
	bool Result = false;

	for (int i = 0; i < 8; i++)
	{
		m_RegionWithMouseOver[i] = false;

		if ((MousePosition.x >= m_Region[i].Left) && (MousePosition.x <= m_Region[i].Right) &&
			(MousePosition.y >= m_Region[i].Top) && (MousePosition.y <= m_Region[i].Bottom))
		{
			Result = true;
			m_RegionWithMouseOver[i] = true;
		}
	}
	
	return Result;
}

void JWThickBorder::SetCursorAndMoveID()
{
	m_MoveID = -1;
	for (int i = 0; i < 8; i++)
	{
		if (m_RegionWithMouseOver[i])
		{
			m_MoveID = i;
		}
	}
	
	if (m_RegionWithMouseOver[0] || m_RegionWithMouseOver[2])
	{
		// Top, bottom
		SetCursor(LoadCursor(nullptr, IDC_SIZENS));
	}
	else if (m_RegionWithMouseOver[1] || m_RegionWithMouseOver[3])
	{
		// Right, left
		SetCursor(LoadCursor(nullptr, IDC_SIZEWE));
	}
	if ((m_RegionWithMouseOver[4] || m_RegionWithMouseOver[7]))
	{
		// Top-right or Bottom-left
		SetCursor(LoadCursor(nullptr, IDC_SIZENESW));
	}
	if ((m_RegionWithMouseOver[5] || m_RegionWithMouseOver[6]))
	{
		// Top-left or Bottom-right
		SetCursor(LoadCursor(nullptr, IDC_SIZENWSE));
	}
}

void JWThickBorder::UpdateControlState(JWWinBase* pBase)
{
	if (IsMouseOnRegion(pBase->GetMousePositionClient()))
	{
		SetCursorAndMoveID();
	}

	if (IsMouseOnRegion(pBase->GetCapturedMousePositionClient()))
	{
		SetCursorAndMoveID();

		if (pBase->IsMouseLeftButtonDown())
		{
			// When the button is pressed in the region
			SetControlState(JWControl::CONTROL_STATE::Pressed);
		}
		if (pBase->IsMouseLeftButtonUp())
		{
			SetControlState(JWControl::CONTROL_STATE::Normal);
		}
	}
	else
	{
		SetControlState(JWControl::CONTROL_STATE::Normal);
	}
}

auto JWThickBorder::GetCapturedMoveID()->FLAG
{
	return m_CapturedMoveID;
}

auto JWThickBorder::CanResizeWindow(POINT ClientCapturedPosition)->bool
{
	if (!m_bCanResizeWindow)
	{
		if (IsMouseOnRegion(ClientCapturedPosition))
		{
			// Title bar move
			m_bCanResizeWindow = true;
			m_CapturedMoveID = m_MoveID;
		}
	}
	return m_bCanResizeWindow;
}

void JWThickBorder::StopResizeWindow()
{
	m_bCanResizeWindow = false;
}

void JWThickBorder::ResizeWindow(JWWinBase* pBase)
{
	Int2 MovedPos = pBase->GetMousePositionScreen() - pBase->GetCapturedMousePositionScreen();
	Int2 NewSize = pBase->GetCapturedWindowSize();
	Int2 NewPos;

	switch (GetCapturedMoveID())
	{
	case 0:
		// top NS
		NewSize.y -= MovedPos.y;

		NewPos.x = pBase->GetCapturedWindowPosition().x;
		NewPos.y = pBase->GetCapturedWindowPosition().y + MovedPos.y;
		break;
	case 1:
		// right EW
		NewSize.x += MovedPos.x;

		NewPos.x = pBase->GetCapturedWindowPosition().x;
		NewPos.y = pBase->GetCapturedWindowPosition().y;
		break;
	case 2:
		// bottom NS
		NewSize.y += MovedPos.y;

		NewPos.x = pBase->GetCapturedWindowPosition().x;
		NewPos.y = pBase->GetCapturedWindowPosition().y;
		break;
	case 3:
		// left EW
		NewSize.x -= MovedPos.x;

		NewPos.x = pBase->GetCapturedWindowPosition().x + MovedPos.x;
		NewPos.y = pBase->GetCapturedWindowPosition().y;
		break;
	case 4:
		// top-right NESW
		NewSize.x += MovedPos.x;
		NewSize.y -= MovedPos.y;

		NewPos.x = pBase->GetCapturedWindowPosition().x;
		NewPos.y = pBase->GetCapturedWindowPosition().y + MovedPos.y;
		break;
	case 5:
		// top-left NESW
		NewSize.x -= MovedPos.x;
		NewSize.y -= MovedPos.y;

		NewPos.x = pBase->GetCapturedWindowPosition().x + MovedPos.x;
		NewPos.y = pBase->GetCapturedWindowPosition().y + MovedPos.y;
		break;
	case 6:
		// bottom-right NWSE
		NewSize.x += MovedPos.x;
		NewSize.y += MovedPos.y;

		NewPos.x = pBase->GetCapturedWindowPosition().x;
		NewPos.y = pBase->GetCapturedWindowPosition().y;
		break;
	case 7:
		// bottom-left NWSE
		NewSize.x -= MovedPos.x;
		NewSize.y += MovedPos.y;

		NewPos.x = pBase->GetCapturedWindowPosition().x + MovedPos.x;
		NewPos.y = pBase->GetCapturedWindowPosition().y;
		break;
	default:
		return;
		break;
	}

	if (NewSize.x < MIN_WINDOW_WIDTH)
		NewSize.x = MIN_WINDOW_WIDTH;

	if (NewSize.y < MIN_WINDOW_HEIGHT)
		NewSize.y = MIN_WINDOW_HEIGHT;

	pBase->SetWindowPosition(NewPos);
	pBase->SetWindowSize(NewSize);
}