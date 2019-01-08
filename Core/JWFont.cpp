#include "JWFont.h"

JWFont::JWFont()
{
	m_CurrFontInstanceID = 0; // Set current font instance ID to zero
	m_FontColor = 0xFF000000; // Set defualt font color to black
}

auto JWFont::Create(LPDIRECT3DDEVICE9 pDevice)->JWERROR
{
	if (pDevice == nullptr)
		return JWERROR::NullDevice;

	m_pDevice = pDevice;
	return JWERROR::Ok;
}

void JWFont::Destroy()
{
	m_pDevice = nullptr;

	for (FontInstance& iterator : m_Fonts)
	{
		if (iterator.pFont)
		{
			iterator.pFont->Release();
			iterator.pFont = nullptr;
		}
	}
}

auto JWFont::MakeFont(FontID ID, WSTRING FontName, int FontSize, bool IsBold)->JWERROR
{
	UINT Weight = FW_NORMAL;
	if (IsBold)
		Weight = FW_BOLD;

	LPD3DXFONT tpFont;
	D3DXCreateFontW(m_pDevice, FontSize, 0, Weight, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, FontName.c_str(), &tpFont);

	if (!tpFont)
		return JWERROR::FontNotCreated;

	m_Fonts.push_back(FontInstance(ID, tpFont, FontName, FontSize, Weight));
	//m_Fonts.emplace_back(ID, tpFont, FontName, FontSize, Weight);

	return JWERROR::Ok;
}

void JWFont::OnLostDevice()
{
	for (FontInstance iterator : m_Fonts)
	{
		iterator.pFont->OnLostDevice();
	}
}

void JWFont::OnResetDevice()
{
	for (FontInstance iterator : m_Fonts)
	{
		iterator.pFont->OnResetDevice();
	}
}

auto JWFont::SetFont(FontID ID)->JWFont*
{
	int iterator_n = 0;
	for (FontInstance& iterator : m_Fonts)
	{
		if (iterator.ID == ID)
		{
			m_CurrFontInstanceID = iterator_n;
			return this;
		}
		iterator_n++;
	}

	return nullptr;
}

auto JWFont::SetFontColor(DWORD Color)->JWFont*
{
	m_FontColor = Color;
	return this;
}

auto JWFont::Draw(D3DXVECTOR2 Position, D3DXVECTOR2 Size, WSTRING String,
	ALIGNMENT_HORZ AlignHorz, ALIGNMENT_VERT AlignVert)->JWFont*
{
	SetRect(&m_RectFont, static_cast<int>(Position.x), static_cast<int>(Position.y),
		static_cast<int>(Position.x + Size.x), static_cast<int>(Position.y + Size.y));

	DWORD FontAlignFlag = 0;
	switch (AlignHorz)
	{
	case ALIGNMENT_HORZ::Left:
		FontAlignFlag = DT_LEFT;
		break;
	case ALIGNMENT_HORZ::Center:
		FontAlignFlag = DT_CENTER;
		break;
	case ALIGNMENT_HORZ::Right:
		FontAlignFlag = DT_RIGHT;
		break;
	default:
		break;
	}

	switch (AlignVert)
	{
	case ALIGNMENT_VERT::Top:
		FontAlignFlag |= DT_TOP;
		break;
	case ALIGNMENT_VERT::Center:
		FontAlignFlag |= DT_VCENTER;
		break;
	case ALIGNMENT_VERT::Bottom:
		FontAlignFlag |= DT_BOTTOM;
		break;
	default:
		break;
	}

	if (m_Fonts[m_CurrFontInstanceID].pFont)
	{
		m_Fonts[m_CurrFontInstanceID].pFont->DrawText(nullptr, String.c_str(), -1, &m_RectFont,
			FontAlignFlag | DT_NOCLIP, m_FontColor);
	}

	return this;
}