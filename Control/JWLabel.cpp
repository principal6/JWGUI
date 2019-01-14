#include "JWLabel.h"

using namespace JW_GUI;

auto JWLabel::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (JW_FAILED(JWControl::Create(pDevice)))
		return Error::ControlNotCreated;

	m_Shape = MAKE_UNIQUE(JWShape)();
	if (JW_FAILED(m_Shape->Create(pDevice)))
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
	m_Shape->MakeRectangle(Size, ColorBG);

	// Create border line
	JWControl::MakeBorder();

	// Update base position
	SetPosition(m_Position);
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

	// Set text position
	m_Font->SetPosition(Position);

	// Set border position
	JWControl::UpdateBorder();

	// Set control region
	SetRegion();
}

void JWLabel::SetBackgroundAlpha(BYTE Alpha)
{
	JW_GUI::SetAlpha(&m_ColorBackground, Alpha);

	m_Shape->SetAlpha(Alpha);
}

void JWLabel::SetBackgroundXRGB(DWORD XRGB)
{
	JW_GUI::SetXRGB(&m_ColorBackground, XRGB);

	m_Shape->SetXRGB(XRGB);
}