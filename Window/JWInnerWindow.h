#pragma once

#include "JWWindow.h"

namespace JW_GUI
{
	class JWInnerWindow final : public JWWindow
	{
	private:
		UNIQUE_PTR<JWShape> m_BG;

	public:
		JWInnerWindow() {};
		~JWInnerWindow() {};

		auto Create(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error;
		auto CreateGUI(JWDXDevice* pDXDevice)->Error override;

		void UpdateOutterWindowInfo(Int2 OutterWindowPosition, Int2 OutterWindowSize) override;
		void UpdateControls() override;
		void UpdateWindow(bool FreezeWindow) override;

		void Draw() override;

		// Window state change
		void MinimizeWindow() override;
		void MaximizeWindow() override;
		void RestoreWindow() override;
		void MoveWindow() override;

		void OnMouseDoubleClick() override;
		void UpdateMaximizedSize() override;
	};
};