#include "JWBorder.h"

JWERROR JWBorder::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == (m_pDevice = pDevice))
		return JWERROR::NullDevice;

	// Create border line
	m_Line = MAKE_UNIQUE(JWLine);
	if (JW_FAILED(m_Line->Create(m_pDevice)))
		return JWERROR::LineNotCreated;

	return JWERROR::Ok;
}

void JWBorder::MakeBorder(D3DXVECTOR2 Size)
{
	m_Size = Size;

	m_Line->AddBox(m_Position, D3DXVECTOR2(m_Size.x, m_Size.y), s_DefaultHighlightColor);
	m_Line->AddEnd();
}

void JWBorder::UpdateBorder(D3DXVECTOR2 Size)
{
	m_Size = Size;

	m_Line->UpdateLine(0, m_Position, D3DXVECTOR2(m_Size.x, 0), s_DefaultHighlightColor);
	m_Line->UpdateLine(1, m_Position, D3DXVECTOR2(0, m_Size.y), s_DefaultHighlightColor);
	m_Line->UpdateLine(2, D3DXVECTOR2(m_Position.x + m_Size.x, m_Position.y), D3DXVECTOR2(0, m_Size.y),
		s_DefaultHighlightColor);
	m_Line->UpdateLine(3, D3DXVECTOR2(m_Position.x, m_Position.y + m_Size.y), D3DXVECTOR2(m_Size.x, 0),
		s_DefaultHighlightColor);
	
}

void JWBorder::Draw()
{
	m_Line->Draw();
}