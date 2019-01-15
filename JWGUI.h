#pragma once

#include "Core/JWInput.h"
#include "Core/JWDXDevice.h"
#include "JWOutterWindow.h"

namespace JW_GUI
{
	class JWGUI final
	{
	private:
		MSG m_MSG;
		DIMOUSESTATE2 m_MouseState;
		WSTRING m_GUIName;

		SHARED_PTR<JWDXDevice> m_DXDevice;
		UNIQUE_PTR<JWOutterWindow> m_OutterWindow;
		UNIQUE_PTR<JWInput> m_Input;

		B_RUNNING m_bRunning;

	private:
		void JWGUI::Destroy();
		void JWGUI::DoEvents();

	public:
		JWGUI() {};
		~JWGUI() {};

		auto JWGUI::Create(WSTRING Name)->Error;

		void JWGUI::Run();
	};
};