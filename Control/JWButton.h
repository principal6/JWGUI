#pragma once

#include "JWControl.h"

namespace JW_GUI
{
	class JWButton final : public JWControl
	{
	protected:
		BUTTON_TYPE m_Type;
		UNIQUE_PTR<JWShape> m_BG;
		UNIQUE_PTR<JWLine> m_SystemLine;
		UNIQUE_PTR<JWLine> m_SystemLineAlt;
		bool m_bDrawAlt;

		DWORD m_ColorNormal;
		DWORD m_ColorHover;
		DWORD m_ColorPressed;

		// TODO: Caption
		// TODO: COMMAND: [focused, unfocused = dot line] / (hover), pressed

	private:
		auto JWButton::IsSystemButton()->bool;
		void JWButton::CheckState();

	public:
		JWButton();
		~JWButton() {};

		// Creation
		auto JWButton::Create(LPDIRECT3DDEVICE9 pDevice)->Error override;

		// MakeOutter
		void JWButton::MakeCommandButton(WSTRING Text, D3DXVECTOR2 Size) override;
		void JWButton::MakeSystemButton(BUTTON_TYPE Type, D3DXVECTOR2 Size) override;

		// Draw
		void JWButton::Draw() override;

		// Setter only
		void JWButton::SetSize(D3DXVECTOR2 Size) override;
		void JWButton::SetControlPosition(D3DXVECTOR2 Position) override;
		void JWButton::SetColorNormal(DWORD Color);
		void JWButton::SetColorHover(DWORD Color);
		void JWButton::SetColorPressed(DWORD Color);

		// Toggle
		void JWButton::ToggleDrawAlt();
	};
};

