#pragma once

#include "../Core/JWWinBase.h"
#include "JWButton.h"
#include "JWLabel.h"
#include "JWImage.h"

namespace JW_GUI
{
	class JWTitlebar final : public JWControl
	{
	private:
		static const float TITLEBAR_HEIGHT;
		static const float ICON_WIDTH;
		static const float ICON_PAD;
		static const float SYSBUTTON_WIDTH;

		UNIQUE_PTR<JWButton> m_SysExit;
		UNIQUE_PTR<JWButton> m_SysMin;
		UNIQUE_PTR<JWButton> m_SysMax;
		UNIQUE_PTR<JWLabel> m_Label;
		UNIQUE_PTR<JWShape> m_BG;
		UNIQUE_PTR<JWImage> m_Icon;

		bool m_bOnSystemMinimize;
		bool m_bOnSystemMaximize;
		bool m_bOnSystemExit;
		bool m_bOnWindowMove;

	public:
		JWTitlebar();
		~JWTitlebar() {};

		// Creation
		auto JWTitlebar::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->Error override;

		// Make
		void JWTitlebar::Make(Int2 WindowSize, STRING WindowName);

		// Draw
		void JWTitlebar::Draw();

		// Update
		void JWTitlebar::Update(JWWinBase* pBase);
		void JWTitlebar::UpdateSize(Int2 WindowSize);
		void JWTitlebar::UpdateControlStates(JWWinBase* pBase);

		// Recall
		auto JWTitlebar::OnSystemMinimize()->bool;
		auto JWTitlebar::OnSystemMaximize()->bool;
		auto JWTitlebar::OnSystemExit() const->bool;

		// Move or stop the window
		auto JWTitlebar::CanMoveWindow(POINT CapturedMousePositionClient)->bool;
		void JWTitlebar::StopWindow();
		void JWTitlebar::ToggleSysMaxButton();
		void JWTitlebar::DoubleClickMaximize(POINT MouePosition);
	};
};