#pragma once

#include "JWControl.h"

namespace JW_GUI
{
	class JWLabel final : public JWControl
	{
	protected:
		UNIQUE_PTR<JWShape> m_Shape;

	public:
		JWLabel() {};
		~JWLabel() {};

		// Creation
		auto JWLabel::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->Error override;

		// Make
		void JWLabel::MakeLabel(STRING Text, D3DXVECTOR2 Size, DWORD ColorFont = JWCOLOR_FONT,
			DWORD ColorBG = JWCOLOR_LABEL);

		// Draw
		void JWLabel::Draw();

		// Setter only
		void JWLabel::SetSize(D3DXVECTOR2 Size);
		void JWLabel::SetPosition(D3DXVECTOR2 Position);
	};
};