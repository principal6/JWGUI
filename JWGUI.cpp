#include "JWGUI.h"

using namespace JW_GUI;

// Static member variable
JWWindowManager JWGUI::ms_WindowManager;

LRESULT CALLBACK JW_GUI::WindowProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return JWGUI::ms_WindowManager.MessageHandler(hWnd, Message, wParam, lParam);
}

auto JWGUI::Create(WSTRING Name)->Error
{
	// @warning: Console window is for debugging
	// Close the console window	
	FreeConsole();

	// Create the window manager
	ms_WindowManager.CreateOutterWindow(Name, Int2(0, 0), Int2(600, 400), JWCOLOR_PLAIN);
	ms_WindowManager.CreateInnerWindow(L"INNER01", Int2(50, 100), Int2(150, 200), D3DCOLOR_XRGB(0, 150, 80));

	// Create DirectX Input device
	m_Input = MAKE_UNIQUE(JWInput)();
	if (JW_FAILED(m_Input->Create(ms_WindowManager.GetInstanceHandle(), ms_WindowManager.GetWindowHandle())))
		return Error::DirectInputNotCreated;

	// Add controls
	ms_WindowManager.GetOutterWindow()->AddControl(JWControl::CONTROL_TYPE::Label);
	ms_WindowManager.GetOutterWindow()->GetControlPointer(0)->MakeLabel(L"AA", D3DXVECTOR2(100, 20));
	ms_WindowManager.GetOutterWindow()->GetControlPointer(0)->SetControlPosition(D3DXVECTOR2(0, 50));
	ms_WindowManager.GetOutterWindow()->GetControlPointer(0)->SetBackgroundAlpha(100);
	ms_WindowManager.GetOutterWindow()->GetControlPointer(0)->SetBackgroundXRGB(D3DCOLOR_XRGB(255, 255, 0));
	ms_WindowManager.GetOutterWindow()->GetControlPointer(0)->SetFontXRGB(D3DCOLOR_XRGB(0, 0, 80));

	ms_WindowManager.GetInnerWindow()->AddControl(JWControl::CONTROL_TYPE::Button);
	ms_WindowManager.GetInnerWindow()->GetControlPointer(0)->MakeCommandButton(L"ABC", D3DXVECTOR2(100, 50));
	ms_WindowManager.GetInnerWindow()->GetControlPointer(0)->SetControlPosition(D3DXVECTOR2(0, 100));
	ms_WindowManager.GetInnerWindow()->GetControlPointer(0)->SetDrawBorder(true);

	return Error::Ok;
}

void JWGUI::Run()
{
	m_bRunning = true;

	// Show the created window on the screen when Run() is executed
	ms_WindowManager.ShowWindows();

	// Enter the main loop
	while (m_bRunning)
	{
		if (PeekMessage(&m_MSG, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);
		}
		else
		{
			DoEvents();
		}
	}

	// Shutdown the GUI
	Destroy();
}

void JWGUI::Destroy()
{
	m_Input->Destroy();
	ms_WindowManager.Destroy();
}

void JWGUI::DoEvents()
{
	m_bRunning = ms_WindowManager.UpdateWindows();

	if (SUCCEEDED(ms_WindowManager.GetDXDevice()->BeginRender()))
	{
		// @warning: Render only when the device is not lost
		ms_WindowManager.DrawWindows();
	}

	if (FAILED(ms_WindowManager.GetDXDevice()->EndRender()))
	{
		// If EndRender() failes, the device is lost
		if (SUCCEEDED(ms_WindowManager.GetDXDevice()->ResetDevice()))
		{

		}
	}
}