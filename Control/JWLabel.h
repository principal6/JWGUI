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
		auto JWLabel::Create(LPDIRECT3DDEVICE9 pDevice)->Error override;

		// Make
		void JWLabel::MakeLabel(WSTRING Text, D3DXVECTOR2 Size, DWORD ColorFont = JWCOLOR_FONT,
			DWORD ColorBG = JWCOLOR_LABEL) override;

		// Draw
		void JWLabel::Draw() override;

		// Setter only
		void JWLabel::SetSize(D3DXVECTOR2 Size) override;
		void JWLabel::SetPosition(D3DXVECTOR2 Position) override;
	};
};