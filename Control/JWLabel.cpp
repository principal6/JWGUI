#include "JWLabel.h"

auto JWLabel::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->JWERROR
{
	if (JW_FAILED(JWControl::Create(pDevice, pFont)))
		return JWERROR::ControlNotCreated;

	m_Shape = UNIQUE_PTR<JWShape>(new JWShape);
	if (JW_FAILED(m_Shape->Create(pDevice)))
		return JWERROR::ShapeNotCreated;

	m_ControlType = CONTROL_TYPE::Label;
	m_ControlState = CONTROL_STATE::Normal;

	return JWERROR::Ok;
}

void JWLabel::MakeLabel(WSTRING Text, D3DXVECTOR2 Size, DWORD ColorFont, DWORD ColorBG)
{
	m_Size = Size;
	m_Text = Text;
	m_ColorBackground = ColorBG;
	m_ColorFont = ColorFont;

	// Create background
	m_Shape->MakeRectangle(Size, ColorBG);

	// Create border line
	JWControl::MakeBorder();

	// Update control region
	SetRegion();
}

void JWLabel::Draw()
{
	// Draw background
	m_Shape->Draw();

	// Draw text
	JWControl::DrawString();

	// Draw border
	JWControl::DrawBorder();
}

void JWLabel::SetSize(D3DXVECTOR2 Size)
{
	m_Size = Size;

	// Set background size
	m_Shape->SetSize(m_Size);

	// Set border size
	JWControl::UpdateBorder();

	// Update control region
	SetRegion();
}

void JWLabel::SetPosition(D3DXVECTOR2 Position)
{
	m_Position = Position;

	// Set background position
	m_Shape->SetPosition(Position);

	// Set border position
	JWControl::UpdateBorder();

	// Set control region
	SetRegion();
}