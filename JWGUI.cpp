#include "JWGUI.h"

using namespace JW_GUI;

auto JWGUI::Create(WSTRING Name)->Error
{
	// @warning: Console window is for debugging
	// Close the console window	
	FreeConsole();

	// Create base window and initialize DirectX
	m_OutterWindow = MAKE_UNIQUE(JWOutterWindow)();
	if (JW_FAILED(m_OutterWindow->Create(Name, Int2(200, 200), Int2(600, 400), JWCOLOR_PLAIN)))
		return Error::OutterWindowNotCreated;

	// Initialize DirectX
	m_DXDevice = MAKE_SHARED(JWDXDevice)();
	if (JW_FAILED(m_DXDevice->Create(m_OutterWindow->GethWnd(), JWCOLOR_PLAIN)))
		return Error::DXDeviceNotCreated;

	// @warning: This process is essential!
	// Create Window GUI
	if (JW_FAILED(m_OutterWindow->CreateGUI(m_DXDevice.get())))
		return Error::GUINotCreated;

	// Create DirectX Input device
	m_Input = MAKE_UNIQUE(JWInput)();
	if (JW_FAILED(m_Input->Create(m_OutterWindow->GethInstance(), m_OutterWindow->GethWnd())))
		return Error::DirectInputNotCreated;

	// Add controls
	m_OutterWindow->AddControl(JWControl::CONTROL_TYPE::Label);
	m_OutterWindow->GetControlPointer(0)->MakeLabel(L"AA", D3DXVECTOR2(100, 20));
	m_OutterWindow->GetControlPointer(0)->SetPosition(D3DXVECTOR2(0, 50));
	m_OutterWindow->GetControlPointer(0)->SetBackgroundAlpha(100);
	m_OutterWindow->GetControlPointer(0)->SetBackgroundXRGB(D3DCOLOR_XRGB(255, 255, 0));
	m_OutterWindow->GetControlPointer(0)->SetFontXRGB(D3DCOLOR_XRGB(0, 0, 80));

	return Error::Ok;
}

void JWGUI::Run()
{
	m_bRunning = true;

	// Show the created window on the screen when Run() is executed
	m_OutterWindow->ShowJWWindow();

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
	m_DXDevice->Destroy();
	m_OutterWindow->Destroy();
}

void JWGUI::DoEvents()
{
	// Set defualt cursor
	SetCursor(LoadCursor(nullptr, DEFAULT_CURSOR));

	m_bRunning = m_OutterWindow->Update();

	if (SUCCEEDED(m_DXDevice->BeginRender()))
	{
		// @warning: Render only when the device is not lost

		m_OutterWindow->Draw();
	}

	if (FAILED(m_DXDevice->EndRender()))
	{
		// If EndRender() failes, the device is lost
		if (SUCCEEDED(m_DXDevice->ResetDevice()))
		{
			
		}
	}
}