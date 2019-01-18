#pragma once

#include "Core/JWInput.h"
#include "Window/JWWindowManager.h"

namespace JW_GUI
{
	class JWGUI final
	{
	private:
		MSG m_MSG;
		DIMOUSESTATE2 m_MouseState;
		WSTRING m_GUIName;

		static JWWindowManager ms_WindowManager;

		UNIQUE_PTR<JWInput> m_Input;

		B_RUNNING m_bRunning;

	private:
		friend LRESULT CALLBACK JW_GUI::WindowProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

		void JWGUI::Destroy();
		void JWGUI::DoEvents();

	public:
		JWGUI() {};
		~JWGUI() {};

		auto JWGUI::Create(WSTRING Name)->Error;

		void JWGUI::Run();
	};
};