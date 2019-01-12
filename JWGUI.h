#pragma once

#include "Core/JWWinBase.h"
#include "Core/JWInput.h"
#include "Core/JWFont.h"
#include "Control/JWLabel.h"
#include "Control/JWButton.h"
#include "Control/JWThickBorder.h"
#include "Control/JWTitleBar.h"
#include "JWControlManager.h"

namespace JW_GUI
{
	class JWGUI final
	{
	private:
		MSG m_MSG;
		DIMOUSESTATE2 m_MouseState;
		STRING m_GUIName;

		UNIQUE_PTR<JWWinBase> m_WinBase;
		UNIQUE_PTR<JWThickBorder> m_WindowBorder;
		UNIQUE_PTR<JWInput> m_Input;
		UNIQUE_PTR<JWControlManager> m_ControlManager;
		UNIQUE_PTR<JWTitlebar> m_TitleBar;
		LPD3DXFONT m_pFont;
		UNIQUE_PTR<JWFont> m_FontTest;


		bool m_bRunning;
		bool m_bValuesLocked;
		bool m_bCanResize;
		bool m_bCanMove;

	private:
		void JWGUI::Destroy();
		void JWGUI::DoEvents();

	public:
		JWGUI();
		~JWGUI() {};

		auto JWGUI::Create(STRING Name)->Error;

		void JWGUI::Run();
	};
};