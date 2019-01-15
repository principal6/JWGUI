#pragma once

#include "Core/JWCommon.h"
#include "Core/JWDXDevice.h"
#include "Control/JWLabel.h"
#include "Control/JWButton.h"
#include "Control/JWImage.h"
#include "Control/JWThickBorder.h"
#include "Control/JWTitleBar.h"

namespace JW_GUI
{
	class JWWindow
	{
	protected:
		enum class WindowState
		{
			Normal,
			Minimized,
			Maximized,
		};

		enum class WindowType
		{
			OutterWindow,
			InnerWindow,
		};

	protected:
		static bool ms_IsWindowCaptured;
		static bool ms_onMouseMove;
		static bool ms_onMouseDoubleCliked;
		static POINT ms_MousePosition;
		static POINT ms_CapturedMousePosition;

		UNIQUE_PTR<JWThickBorder> m_WindowBorder;
		UNIQUE_PTR<JWTitleBar> m_TitleBar;

		VECTOR<JWControl*> m_pControls;

		HINSTANCE m_hInstance;
		HWND m_hWnd;
		RECT m_Rect;

		LPDIRECT3DDEVICE9 m_pDevice;
		JWDXDevice* m_pDXDevice;

		WSTRING m_CaptionString;

		Int2 m_WindowSize;
		Int2 m_WindowSizePrev;
		Int2 m_CapturedWindowSize;
		Int2 m_WindowPosition;
		Int2 m_WindowPositionPrev;
		Int2 m_CapturedWindowPosition;
		DWORD m_BackColor;

		WindowState m_WindowState;

	protected:
		virtual void SetCapturedMousePosition(Int2 Value);

	public:
		JWWindow() {};
		virtual ~JWWindow() {};

		// Creation & destruction
		virtual auto Create(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error;
		virtual auto CreateGUI(JWDXDevice* pDXDevice)->Error;
		virtual void Destroy();

		// Control
		virtual void AddControl(JWControl::CONTROL_TYPE ControlType);
		virtual auto GetControlPointer(UINT Control_ID)->JWControl* const;
		virtual void UpdateControls();

		// Draw
		virtual void Draw() {};

		// Window state change
		virtual void ShowJWWindow();
		virtual void MinimizeWindow();
		virtual void MaximizeWindow();
		virtual void RestoreWindow();
		virtual void ResizeWindow() {};
		virtual void ShutdownWindow() {};

		// On
		virtual auto OnMouseLeftButtonDown() const->bool;
		virtual auto OnMouseMove() const->bool;
		virtual auto OnCapturedMouseMove() const->bool;
		virtual auto OnMouseDoubleClicked()->bool;

		// Setter
		virtual void SetWindowPosition(Int2 Value);
		virtual void SetWindowSize(Int2 Value);

		// Getter
		virtual auto GetWindowPosition() const->Int2;
		virtual auto GetWindowSize() const->Int2;

		// Getter only
		virtual auto GetMousePosition() const->Int2;
		virtual auto GetCapturedMousePosition() const->Int2;
		virtual auto GetCapturedWindowPosition() const->Int2;
		virtual auto GetCapturedWindowSize() const->Int2;
	};
};