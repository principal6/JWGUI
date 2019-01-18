#pragma once

#include "../Core/JWCommon.h"
#include "../Control/JWLabel.h"
#include "../Control/JWButton.h"
#include "../Control/JWImage.h"
#include "../Control/JWThickBorder.h"
#include "../Control/JWTitleBar.h"
#include "JWDXDevice.h"

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

	protected:
		static const int UNMAXIMIZE_PADDING = 2;
		static POINT ms_MousePosition;
		static POINT ms_CapturedMousePosition;

		UNIQUE_PTR<JWThickBorder> m_WindowBorder;
		UNIQUE_PTR<JWTitleBar> m_TitleBar;

		VECTOR<JWControl*> m_pControls;

		HINSTANCE m_hInstance;
		HWND m_hWnd;
		RECT m_Rect;

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
		WindowState m_WindowStatePrev;

		B_RUNNING m_bRunning;
		bool m_bCaptured;
		bool m_bCapturedMouseMoved;

		Int2 m_OutterWindowPosition;
		Int2 m_OutterWindowSize;
		bool m_bResized;
		bool m_bResizing;
		bool m_bMoving;

	protected:
		friend LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		virtual void SetCapturedMousePosition(Int2 Value);

	public:
		JWWindow();
		virtual ~JWWindow() {};

		// Creation & destruction
		virtual auto Create(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error;
		virtual auto CreateGUI(JWDXDevice* pDXDevice)->Error;
		virtual void Destroy();

		// Control
		virtual auto AddControl(JWControl::CONTROL_TYPE ControlType)->JWControl*;
		virtual auto GetControlPointer(UINT Control_ID)->JWControl* const;

		// Update
		virtual void UpdateOutterWindowInfo(Int2 OutterWindowPosition, Int2 OutterWindowSize) {};
		virtual void UpdateControls();
		virtual void UpdateWindow(bool FreezeWindow = false);

		// Draw
		virtual void Draw() {};
		virtual void DrawUI() {};

		// Window state change
		virtual void ShowJWWindow();
		virtual void MinimizeWindow();
		virtual void MaximizeWindow();
		virtual void RestoreWindow();
		virtual void ResizeWindow();
		virtual void MoveWindow() {};
		virtual void ShutdownWindow();
		virtual void UpdateWindowSize();
		virtual void UpdateMaximizedSize() {};

		virtual void CaptureWindow();
		virtual void ReleaseWindow();
		virtual void OnMouseDoubleClick() {};
		virtual void OnMouseMove();

		// Setter
		virtual void SetWindowPosition(Int2 Value);
		virtual void SetWindowSize(Int2 Value);

		// Getter
		virtual auto GetWindowPosition() const->Int2;
		virtual auto GetWindowSize() const->Int2;

		// Getter only
		virtual auto GetCapturedWindowPosition() const->Int2;
		virtual auto GetCapturedWindowSize() const->Int2;

		virtual auto GetMousePosition() const->Int2;
		virtual auto GetCapturedMousePosition() const->Int2;
		virtual auto GetClientMousePosition() const->Int2;
		virtual auto GetClientCapturedMousePosition() const->Int2;

		virtual auto IsResized()->bool;
		virtual auto IsMoving()->bool;
		virtual auto IsResizing()->bool;
		virtual auto IsRunning()->B_RUNNING { return false; };
		virtual auto GethWnd() const->HWND { return nullptr; };
		virtual auto GethInstance() const->HINSTANCE { return nullptr; };
		virtual auto GetDevice() const->LPDIRECT3DDEVICE9 { return nullptr; };
	};
};