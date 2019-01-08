#pragma once

#include "JWControl.h"

class JWLabel final : public JWControl
{
protected:
	UNIQUE_PTR<JWShape> m_Shape;

public:
	JWLabel() {};
	~JWLabel() {};

	// Creation
	auto JWLabel::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->JWERROR override;

	// Make
	void JWLabel::MakeLabel(WSTRING Text, D3DXVECTOR2 Size, DWORD ColorFont = s_DefualtFontColor,
		DWORD ColorBG = s_DefualtLabelColor);

	// Draw
	void JWLabel::Draw();

	// Setter
	void JWLabel::SetSize(D3DXVECTOR2 Size);

	// Setter only
	void JWLabel::SetPosition(D3DXVECTOR2 Position);
};