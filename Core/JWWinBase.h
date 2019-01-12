#pragma once

#include "JWCommon.h"

namespace JW_GUI
{
	class JWWinBase final
	{
	private:
		static bool ms_onMouseMove;
		static bool ms_onMouseDoubleCliked;
		static bool ms_bMouseLBTNDown;
		static bool ms_IsWindowCaptured;
		static bool ms_onCaptureWindow;
		static bool ms_onReleaseWindow;
		static bool ms_onRestoreWindow;

		HINSTANCE m_hInstance;
		HWND m_hWnd;
		RECT m_Rect;

		LPDIRECT3D9 m_pD3D;
		LPDIRECT3DDEVICE9 m_pD3DDevice;
		D3DPRESENT_PARAMETERS m_D3DPP;

		HRESULT m_HResult;

		POINT m_MousePosition;
		POINT m_CapturedMousePosition;
		Int2 m_ScreenSize;
		Int2 m_WindowSize;
		Int2 m_WindowSizePrev;
		Int2 m_CapturedWindowSize;
		Int2 m_WindowPosition;
		Int2 m_WindowPositionPrev;
		Int2 m_CapturedWindowPosition;
		DWORD m_ClearColor;
		bool m_bMaximized;

	private:
		friend LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		void JWWinBase::UpdateWindowSize();
		auto JWWinBase::GetWindowShowState()->UINT;

	public:
		JWWinBase() {};
		~JWWinBase() {};

		// Creation & destruction
		auto JWWinBase::Create(WSTRING Name, Int2 Position, Int2 WindowSize,
			DWORD BackColor)->Error;
		void JWWinBase::Destroy();

		// Renderer
		auto JWWinBase::BeginRender()->HRESULT;
		auto JWWinBase::EndRender()->HRESULT;

		// Device checker
		auto JWWinBase::CheckDevice()->HRESULT;
		auto JWWinBase::ResetDevice()->HRESULT;

		// Recall
		auto JWWinBase::IsMouseLeftButtonDown() const->bool;
		auto JWWinBase::IsMouseLeftButtonUp() const->bool;
		auto JWWinBase::OnMouseMove() const->bool;
		auto JWWinBase::OnMouseDoubleClicked()->bool;

		//
		void JWWinBase::MinimizeWindow();
		void JWWinBase::MaximizeWindow();
		void JWWinBase::Update();

		// Setter
		void JWWinBase::SetWindowPosition(Int2 Value);
		void JWWinBase::SetWindowSize(Int2 Value, bool bResetDevice = false);

		// Getter
		auto JWWinBase::GetWindowPosition() const->Int2;
		auto JWWinBase::GetWindowSize() const->Int2;

		// Getter only
		auto JWWinBase::GethWnd() const->HWND;
		auto JWWinBase::GethInstance() const->HINSTANCE;
		auto JWWinBase::GetDevice() const->LPDIRECT3DDEVICE9;
		auto JWWinBase::GetCapturedWindowPosition() const->Int2;
		auto JWWinBase::GetCapturedWindowSize() const->Int2;
		auto JWWinBase::GetMousePositionScreen() const->Int2;
		auto JWWinBase::GetMousePositionClient() const->Int2;
		auto JWWinBase::GetCapturedMousePositionScreen() const->Int2;
		auto JWWinBase::GetCapturedMousePositionClient() const->Int2;
		auto JWWinBase::IsWindowMaximized()->bool;
	};
};