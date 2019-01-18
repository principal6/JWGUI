#include "JWThickBorder.h"

using namespace JW_GUI;

JWThickBorder::JWThickBorder()
{
	m_bCanResizeWindow = false;
}

auto JWThickBorder::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (JW_FAILED(JWControl::Create(pDevice)))
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
	SetControlPosition(m_Position);
	SetRegion(m_Position);
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

	// Update the border
	JWControl::UpdateBorder();

	// Update control region
	SetRegion(m_Position);
}

void JWThickBorder::SetControlPosition(D3DXVECTOR2 Position)
{
	// Set control position
	JWControl::SetControlPosition(Position);

	// Update the border
	JWControl::UpdateBorder();

	// Update control region
	SetRegion(Position);
}

void JWThickBorder::SetRegion(D3DXVECTOR2 Position)
{
	m_Region[0] = REGION(Position, D3DXVECTOR2(m_Size.x, 1.0f)); // top
	m_Region[1] = REGION(D3DXVECTOR2(Position.x + m_Size.x - 1.0f, Position.y), D3DXVECTOR2(1.0f, m_Size.y)); // right
	m_Region[2] = REGION(D3DXVECTOR2(Position.x, Position.y + m_Size.y - 1.0f), D3DXVECTOR2(m_Size.x, 1.0f)); // bottom
	m_Region[3] = REGION(Position, D3DXVECTOR2(1.0f, m_Size.y)); // left
	
	m_Region[4] = REGION(D3DXVECTOR2(Position.x + m_Size.x - 2.0f, Position.y), D3DXVECTOR2(2.0f, 2.0f)); // top-right
	m_Region[5] = REGION(D3DXVECTOR2(Position.x, Position.y), D3DXVECTOR2(2.0f, 2.0f)); // top-left
	m_Region[6] = REGION(D3DXVECTOR2(Position.x + m_Size.x - 2.0f, Position.y + m_Size.y - 2.0f),
		D3DXVECTOR2(2.0f, 2.0f)); // bottom-right
	m_Region[7] = REGION(D3DXVECTOR2(Position.x, Position.y + m_Size.y - 2.0f),
		D3DXVECTOR2(2.0f, 2.0f)); // bottom-left
}

auto JWThickBorder::IsMouseOnRegion(Int2 MousePosition) const->bool
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
		m_CursorID = IDC_SIZENS;
	}
	if (m_RegionWithMouseOver[1] || m_RegionWithMouseOver[3])
	{
		// Right, left
		m_CursorID = IDC_SIZEWE;
	}
	if ((m_RegionWithMouseOver[4] || m_RegionWithMouseOver[7]))
	{
		// Top-right or Bottom-left
		m_CursorID = IDC_SIZENESW;
	}
	if ((m_RegionWithMouseOver[5] || m_RegionWithMouseOver[6]))
	{
		// Top-left or Bottom-right
		m_CursorID = IDC_SIZENWSE;
	}
}

void JWThickBorder::UpdateState(Int2 MousePosition, Int2 MouseDownPosition, bool IsLeftButtonDown, bool WindowMaximized,
	bool StayPressed)
{
	JWControl::UpdateState(MousePosition, MouseDownPosition, IsLeftButtonDown, WindowMaximized, StayPressed);

	// Mouse cursor chagnes when hovering the border
	SetCursorAndMoveID();

	if ((m_ControlState == JWControl::CONTROL_STATE::Hover) || (m_ControlState == JWControl::CONTROL_STATE::Pressed))
	{
		if (!WindowMaximized)
		{
			// @warnging:
			// Do not change cursor when the window is maximized (because it will not be resized!)
			SetCursor(LoadCursor(nullptr, m_CursorID));
		}
	}
}

auto JWThickBorder::GetCapturedMoveID()->FLAG
{
	return m_CapturedMoveID;
}

auto JWThickBorder::GetCapturedCursorID()->LPCWSTR
{
	return m_CapturedCursorID;
}

auto JWThickBorder::CanResizeWindow(POINT CapturedMousePositionClient)->bool
{
	if (IsMouseOnRegion(CapturedMousePositionClient))
	{
		SetCursorAndMoveID();

		m_CapturedMoveID = m_MoveID;
		m_CapturedCursorID = m_CursorID;

		SetCursor(LoadCursor(nullptr, m_CapturedCursorID));

		m_bCanResizeWindow = true;
	}
	else
	{
		SetCursorAndMoveID();

		m_bCanResizeWindow = false;
	}

	return m_bCanResizeWindow;
}

auto JWThickBorder::IsResizingWindow()->bool
{
	return m_bCanResizeWindow;
}


void JWThickBorder::StopResizeWindow()
{
	m_bCanResizeWindow = false;
}