#include "JWLabel.h"

using namespace JW_GUI;

auto JWLabel::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (JW_FAILED(JWControl::Create(pDevice)))
		return Error::ControlNotCreated;

	m_BG = MAKE_UNIQUE(JWShape)();
	if (JW_FAILED(m_BG->Create(pDevice)))
		return Error::ShapeNotCreated;

	m_ControlType = CONTROL_TYPE::Label;
	m_ControlState = CONTROL_STATE::Normal;

	return Error::Ok;
}

void JWLabel::MakeLabel(WSTRING Text, D3DXVECTOR2 Size, DWORD ColorFont, DWORD ColorBG)
{
	m_Size = Size;
	m_Text = Text;

	m_Font->MakeFont(FONT_NAME);
	m_Font->SetText(Text);

	m_ColorBackground = ColorBG;
	m_ColorFont = ColorFont;

	// Create background
	m_BG->MakeRectangle(Size, ColorBG);

	// Create border line
	JWControl::MakeBorder();

	// Update base position
	SetControlPosition(m_Position);
}

void JWLabel::Draw()
{
	// Draw background
	m_BG->Draw();

	// Draw text
	JWControl::DrawString();

	// Draw border
	JWControl::DrawBorder();
}

void JWLabel::SetSize(D3DXVECTOR2 Size)
{
	m_Size = Size;

	// Set background size
	m_BG->SetSize(m_Size);

	// Set border size
	JWControl::UpdateBorder();

	// Update control region
	SetRegion(m_Position);
}

void JWLabel::SetControlPosition(D3DXVECTOR2 Position)
{
	// Set control position
	JWControl::SetControlPosition(Position);

	// Set border position
	JWControl::UpdateBorder();

	// Set background position
	m_BG->SetPosition(Position + m_InnerWindowPosition);

	// Set text position
	m_Font->SetPosition(Position + m_InnerWindowPosition);

	// Set control region
	SetRegion(m_Position);
}

void JWLabel::SetBackgroundAlpha(BYTE Alpha)
{
	JW_GUI::SetAlpha(&m_ColorBackground, Alpha);

	m_BG->SetAlpha(Alpha);
}

void JWLabel::SetBackgroundXRGB(DWORD XRGB)
{
	JW_GUI::SetXRGB(&m_ColorBackground, XRGB);

	m_BG->SetXRGB(XRGB);
}