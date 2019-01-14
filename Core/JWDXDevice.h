#pragma once

#include "JWCommon.h"

namespace JW_GUI
{
	class JWDXDevice final
	{
	private:
		static LPDIRECT3D9 m_pD3D;

		Int2 m_ScreenSize;

		HWND m_hWnd;
		RECT m_WindowRect;
		RECT m_RenderRect;

		LPDIRECT3DDEVICE9 m_pD3DDevice;
		D3DPRESENT_PARAMETERS m_D3DPP;
		DWORD m_ClearColor;

	public:
		JWDXDevice() {};
		~JWDXDevice() {};

		auto Create(HWND hWnd, DWORD ClearColor)->Error;
		void Destroy();

		void UpdateWindowSize();

		// Device checker
		auto CheckDevice()->HRESULT;
		auto ResetDevice()->HRESULT;

		auto GetDevice()->LPDIRECT3DDEVICE9;

		// Renderer
		auto BeginRender()->HRESULT;
		auto EndRender()->HRESULT;

	};
};