#pragma once

#include "JWWindow.h"

namespace JW_GUI
{
	class JWOutterWindow final : public JWWindow
	{
	private:
		static const int UNMAXIMIZE_PADDING = 2;
		static bool ms_onCaptureWindow;
		static bool ms_onReleaseWindow;
		static bool ms_onRestoreWindow;
		static bool ms_bRunning;

	private:
		friend LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		void UpdateWindowSize();

	public:
		auto Create(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error override;
		auto CreateGUI(JWDXDevice* pDXDevice)->Error override;

		void Draw() override;

		auto Update()->B_RUNNING;

		void SetWindowPosition(Int2 Value) override;
		void SetWindowSize(Int2 Value) override;

		// Outter-window-only functions
		auto GethWnd() const->HWND;
		auto GethInstance() const->HINSTANCE;
		auto GetDevice() const->LPDIRECT3DDEVICE9;

		// Window state change
		void ShowJWWindow() override;
		void MinimizeWindow() override;
		void MaximizeWindow() override;
		void RestoreWindow() override;
		void ResizeWindow() override;
		void ShutdownWindow() override;
	};
};