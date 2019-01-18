#pragma once

#include "JWButton.h"
#include "JWLabel.h"
#include "JWImage.h"

namespace JW_GUI
{
	class JWTitleBar final : public JWControl
	{
	private:
		static const float TITLEBAR_SPARE_WIDTH;
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

		D3DXVECTOR2 m_WindowSize;

		bool m_bInnerWindow;

	private:
		inline auto JWTitleBar::GetLabelSizeX()->float;
		void JWTitleBar::UpdateSizeAndPosition();

	public:
		JWTitleBar();
		~JWTitleBar() {};

		// Creation
		auto JWTitleBar::Create(LPDIRECT3DDEVICE9 pDevice)->Error override;

		// MakeOutter
		void JWTitleBar::MakeOutter(Int2 WindowSize, WSTRING WindowName);
		void JWTitleBar::MakeInner(Int2 WindowSize, WSTRING WindowName);

		// Update
		void JWTitleBar::UpdateState(Int2 MousePosition, Int2 MouseDownPosition, bool IsLeftButtonDown,
			bool WindowMaximized = false, bool StayPressed = false) override;

		// Draw
		void JWTitleBar::Draw();

		// Recall
		auto JWTitleBar::OnSystemMinimize()->bool;
		auto JWTitleBar::OnSystemMaximize()->bool;
		auto JWTitleBar::OnSystemExit() const->bool;

		// Move or stop the window
		auto JWTitleBar::CanMoveWindow(POINT CapturedMousePositionClient)->bool;
		auto JWTitleBar::IsMovingWindow()->bool;
		void JWTitleBar::StopWindow();
		void JWTitleBar::ToggleSysMaxButton();
		void JWTitleBar::DoubleClickMaximize(POINT MouePosition);

		// Setter
		void JWTitleBar::SetSize(D3DXVECTOR2 WindowSize) override;
		void JWTitleBar::SetControlPosition(D3DXVECTOR2 Position) override;
		void JWTitleBar::SetWindowOffset(D3DXVECTOR2 InnerWindowPosition, D3DXVECTOR2 OutterWindowPosition) override;

		// Getter
		auto JWTitleBar::GetSystemButtonsWidth()->int;
		auto JWTitleBar::GetTitleBarHeight()->int;
	};
};