#include "JWCommon.h"

class JWFont
{
private:
	struct FontInstance
	{
		FontID ID;
		LPD3DXFONT pFont;
		WSTRING FontName;
		int FontSize;
		UINT Weight;

		FontInstance() : pFont(nullptr) {};
		FontInstance(FontID _ID, LPD3DXFONT _pFont, WSTRING _FontName, int _FontSize, UINT _Weight) :
			ID(_ID), pFont(_pFont), FontName(_FontName), FontSize(_FontSize), Weight(_Weight) {};
	};

private:
	LPDIRECT3DDEVICE9 m_pDevice;
	VECTOR<FontInstance> m_Fonts;
	int m_CurrFontInstanceID;
	DWORD m_FontColor;
	RECT m_RectFont;

public:
	JWFont();
	virtual ~JWFont() {};

	// Creation & destruction
	auto JWFont::Create(LPDIRECT3DDEVICE9 pDevice)->JWERROR;
	void JWFont::Destroy();

	// Make
	auto JWFont::MakeFont(FontID ID, WSTRING FontName, int FontSize, bool IsBold)->JWERROR;

	// Draw
	auto JWFont::Draw(D3DXVECTOR2 Position, D3DXVECTOR2 Size, WSTRING String,
		ALIGNMENT_HORZ AlignHorz, ALIGNMENT_VERT AlignVert)->JWFont*;

	// Device loss handler
	void JWFont::OnLostDevice();
	void JWFont::OnResetDevice();

	// Setter
	auto JWFont::SetFont(FontID ID)->JWFont*;
	auto JWFont::SetFontColor(DWORD Color)->JWFont*;
};