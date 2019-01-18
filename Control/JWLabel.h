#pragma once

#include "JWControl.h"

namespace JW_GUI
{
	class JWLabel final : public JWControl
	{
	protected:
		UNIQUE_PTR<JWShape> m_BG;

	public:
		JWLabel() {};
		~JWLabel() {};

		// Creation
		auto JWLabel::Create(LPDIRECT3DDEVICE9 pDevice)->Error override;

		// MakeOutter
		void JWLabel::MakeLabel(WSTRING Text, D3DXVECTOR2 Size, DWORD ColorFont = JWCOLOR_FONT,
			DWORD ColorBG = JWCOLOR_LABEL) override;

		// Draw
		void JWLabel::Draw() override;

		// Setter only
		void JWLabel::SetSize(D3DXVECTOR2 Size) override;
		void JWLabel::SetControlPosition(D3DXVECTOR2 Position) override;

		// Setter
		void JWLabel::SetBackgroundAlpha(BYTE Alpha) override;
		void JWLabel::SetBackgroundXRGB(DWORD XRGB) override;
	};
};