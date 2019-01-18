#pragma once

#include "JWInnerWindow.h"
#include "JWOutterWindow.h"

namespace JW_GUI
{
	class JWWindowManager final
	{
	private:
		UNIQUE_PTR<JWWindow> m_OutterWindow;
		UNIQUE_PTR<JWDXDevice> m_DXDevice;
		UNIQUE_PTR<JWWindow> m_InnerWindow;

		HINSTANCE m_hInstance;
		HWND m_hWnd;

		bool m_bWindowsCaptured;

	public:
		auto CreateOutterWindow(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error;
		auto CreateInnerWindow(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error;
		void Destroy();
		void ShowWindows();

		auto MessageHandler(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)->LRESULT;
		auto UpdateWindows()->B_RUNNING;
		void DrawWindows();

		auto GetInstanceHandle()->HINSTANCE;
		auto GetWindowHandle()->HWND;
		auto GetOutterWindow()->JWWindow*;
		auto GetInnerWindow()->JWWindow*;
		auto GetDXDevice()->JWDXDevice*;
	};
};