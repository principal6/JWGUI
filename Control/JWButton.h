#pragma once

#include "JWControl.h"

namespace JW_GUI
{
	class JWButton final : public JWControl
	{
	public:
		enum class BUTTON_TYPE
		{
			SystemMinimize,
			SystemMaximize,
			SystemExit,
			CommandButton,
		};

	protected:
		BUTTON_TYPE m_Type;
		UNIQUE_PTR<JWShape> m_Shape;
		UNIQUE_PTR<JWLine> m_SystemLine;
		UNIQUE_PTR<JWLine> m_SystemLineAlt;
		bool m_bDrawAlt;

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

		// Make
		auto JWButton::MakeSystemButton(BUTTON_TYPE Type, D3DXVECTOR2 Size)->Error;

		// Draw
		void JWButton::Draw();

		// Setter only
		void JWButton::SetSize(D3DXVECTOR2 Size);
		void JWButton::SetPosition(D3DXVECTOR2 Position);

		// Toggle
		void JWButton::ToggleDrawAlt();
	};
};

