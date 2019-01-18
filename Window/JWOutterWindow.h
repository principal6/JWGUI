#pragma once

#include "JWWindow.h"

namespace JW_GUI
{
	class JWOutterWindow final : public JWWindow
	{
	public:
		auto Create(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error override;
		auto CreateGUI(JWDXDevice* pDXDevice)->Error override;

		void Draw() override;
		void DrawUI() override;

		void SetWindowPosition(Int2 Value) override;
		void SetWindowSize(Int2 Value) override;

		// Outter-window-only functions
		auto GethWnd() const->HWND override;
		auto GethInstance() const->HINSTANCE override;
		auto GetDevice() const->LPDIRECT3DDEVICE9 override;

		// Window state change
		void ShowJWWindow() override;
		void MinimizeWindow() override;
		void MaximizeWindow() override;
		void RestoreWindow() override;
		void MoveWindow() override;

		void UpdateWindowSize() override;
		void CaptureWindow() override;
		void ReleaseWindow() override;
		void OnMouseDoubleClick() override;
		auto IsRunning()->B_RUNNING;
	};
};