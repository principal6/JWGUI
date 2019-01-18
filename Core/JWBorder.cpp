#include "JWBorder.h"

using namespace JW_GUI;

auto JWBorder::Create(LPDIRECT3DDEVICE9 pDevice) -> Error
{
	if (nullptr == (m_pDevice = pDevice))
		return Error::NullDevice;

	// Create border line
	m_Line = MAKE_UNIQUE(JWLine)();
	if (JW_FAILED(m_Line->Create(m_pDevice)))
		return Error::LineNotCreated;

	return Error::Ok;
}

void JWBorder::MakeBorder(D3DXVECTOR2 Size)
{
	m_Size = Size;

	m_Line->AddBox(m_Position, D3DXVECTOR2(m_Size.x, m_Size.y), JWCOLOR_HIGHLIGHT);
	m_Line->AddEnd();
}

void JWBorder::UpdateBorder()
{
	m_Line->UpdateLine(0, m_Position, D3DXVECTOR2(m_Size.x, 0), JWCOLOR_HIGHLIGHT);
	m_Line->UpdateLine(1, m_Position, D3DXVECTOR2(0, m_Size.y), JWCOLOR_HIGHLIGHT);
	m_Line->UpdateLine(2, D3DXVECTOR2(m_Position.x + m_Size.x, m_Position.y), D3DXVECTOR2(0, m_Size.y),
		JWCOLOR_HIGHLIGHT);
	m_Line->UpdateLine(3, D3DXVECTOR2(m_Position.x, m_Position.y + m_Size.y), D3DXVECTOR2(m_Size.x, 0),
		JWCOLOR_HIGHLIGHT);
}

void JWBorder::Draw()
{
	m_Line->Draw();
}

void JWBorder::SetSize(D3DXVECTOR2 Size)
{
	m_Size = Size;
}

void JWBorder::SetPosition(D3DXVECTOR2 Position)
{
	m_Position = Position;
}