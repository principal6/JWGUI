#pragma once

#include "JWButton.h"
#include "JWLabel.h"
#include "JWImage.h"

namespace JW_GUI
{
	class JWTitleBar final : public JWControl
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
		JWTitleBar();
		~JWTitleBar() {};

		// Creation
		auto JWTitleBar::Create(LPDIRECT3DDEVICE9 pDevice)->Error override;

		// Make
		void JWTitleBar::Make(Int2 WindowSize, WSTRING WindowName);

		// Update
		void JWTitleBar::UpdateState(Int2 MousePosition, Int2 MouseDownPosition, bool IsLeftButtonDown) override;
		void JWTitleBar::UpdateSize(Int2 WindowSize);

		// Draw
		void JWTitleBar::Draw();

		// Recall
		auto JWTitleBar::OnSystemMinimize()->bool;
		auto JWTitleBar::OnSystemMaximize()->bool;
		auto JWTitleBar::OnSystemExit() const->bool;

		// Move or stop the window
		auto JWTitleBar::CanMoveWindow(POINT CapturedMousePositionClient)->bool;
		void JWTitleBar::StopWindow();
		void JWTitleBar::ToggleSysMaxButton();
		void JWTitleBar::DoubleClickMaximize(POINT MouePosition);
	};
};