#include "JWGUI.h"

using namespace JW_GUI;

JWGUI::JWGUI()
{
	m_bValuesLocked = false;
	m_bCanResize = false;
	m_bCanMove = false;
}

auto JWGUI::Create(STRING Name)->Error
{
	// @warning: Console window is for debugging
	// Close the console window	
	FreeConsole();

	// Create base window and initialize DirectX
	m_WinBase = MAKE_UNIQUE(JWWinBase)();
	if (JW_FAILED(m_WinBase->Create("JWGUI", Int2(200, 200), Int2(600, 400), JWCOLOR_PLAIN)))
		return Error::WinBaseNotCreated;

	// Create DirectX Input device
	m_Input = MAKE_UNIQUE(JWInput)();
	if (JW_FAILED(m_Input->Create(m_WinBase->GethInstance(), m_WinBase->GethWnd())))
		return Error::DirectInputNotCreated;

	// Create font
	D3DXCreateFontA(m_WinBase->GetDevice(), 18, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "¸¼Àº °íµñ", &m_pFont);

	m_FontTest = MAKE_UNIQUE(JWFont)();
	if (JW_FAILED(m_FontTest->Create(m_WinBase->GetDevice())))
		return Error::FontNotCreated;
	m_FontTest->MakeFont("minimal.fnt");

	// Craete window border
	m_WindowBorder = MAKE_UNIQUE(JWThickBorder)();
	if (JW_FAILED(m_WindowBorder->Create(m_WinBase->GetDevice(), m_pFont)))
		return Error::BorderNotCreated;
	m_WindowBorder->MakeThickBorder(D3DXVECTOR2(static_cast<float>(m_WinBase->GetWindowSize().x - 1),
		static_cast<float>(m_WinBase->GetWindowSize().y - 1)));

	// Create control manager (with font)
	// 1. Mangage the static font object in JWControl-inherited-classes
	m_ControlManager = MAKE_UNIQUE(JWControlManager)();
	if (JW_FAILED(m_ControlManager->Create(m_WinBase->GetDevice(), m_pFont)))
		return Error::ControlManagerNotCreated;

	// Create main titlebar
	m_TitleBar = MAKE_UNIQUE(JWTitlebar)();
	if (JW_FAILED(m_TitleBar->Create(m_WinBase->GetDevice(), m_pFont)))
		return Error::TitlebarNotcreated;
	m_TitleBar->Make(m_WinBase->GetWindowSize(), Name);
	
	return Error::Ok;
}

void JWGUI::Run()
{
	m_bRunning = true;

	// Now show the created window on the screen
	ShowWindow(m_WinBase->GethWnd(), SW_SHOW);

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
	m_WinBase->Destroy();
}

void JWGUI::Destroy()
{
	m_ControlManager->Destroy();
	m_Input->Destroy();
	m_WinBase->Destroy();
}

void JWGUI::DoEvents()
{
	// Set defualt cursor
	SetCursor(LoadCursor(nullptr, IDC_ARROW));

	m_WinBase->Update();

	m_TitleBar->Update(m_WinBase.get());
	m_ControlManager->UpdateControlStates(m_WinBase.get());

	// When the window is maximized, you can not resize it
	if (!m_WinBase->IsWindowMaximized())
		m_WindowBorder->UpdateControlState(m_WinBase.get());

	if (m_WinBase->OnMouseDoubleClicked())
	{
		m_TitleBar->DoubleClickMaximize(m_WinBase->GetMousePositionClient());
	}

	if (m_TitleBar->OnSystemMinimize())
	{
		m_WinBase->MinimizeWindow();
	}

	if (m_TitleBar->OnSystemMaximize())
	{
		m_WinBase->MaximizeWindow();
	}

	if (m_TitleBar->OnSystemExit())
	{
		m_bRunning = false;
		return;
	}

	if (m_WinBase->IsMouseLeftButtonDown())
	{
		if (!m_bValuesLocked)
		{
			// Get these values only the first time the mouse button is pressed
			m_bCanResize = m_WindowBorder->CanResizeWindow(m_WinBase->GetCapturedMousePositionClient());
			m_bCanMove = m_TitleBar->CanMoveWindow(m_WinBase->GetCapturedMousePositionClient());

			// When the window is maximized, you can not resize it
			if (m_WinBase->IsWindowMaximized())
				m_bCanResize = false;

			if (m_bCanResize)
				m_bCanMove = false;

			// Lock the values
			m_bValuesLocked = true;
		}
		
		// Resize window
		if (m_bCanResize)
		{
			m_WindowBorder->ResizeWindow(m_WinBase.get());
		}
		else
		{
			// Move window
			if (m_bCanMove)
			{
				if (m_WinBase->IsWindowMaximized())
				{
					// If the maximized window gets moved, restore the size
					m_TitleBar->ToggleSysMaxButton();
					m_WinBase->MaximizeWindow();
				}
				else
				{
					Int2 NewPos = m_WinBase->GetCapturedWindowPosition() +
						m_WinBase->GetMousePositionScreen() - m_WinBase->GetCapturedMousePositionScreen();

					m_WinBase->SetWindowPosition(NewPos);
				}
			}
		}
	}
	else
	{
		// End moving and resizing window
		m_TitleBar->StopWindow();
		m_WindowBorder->StopResizeWindow();
		
		// Unlock the values
		m_bValuesLocked = false;
	}
	
	if (SUCCEEDED(m_WinBase->BeginRender()))
	{
		// Render only when the device is not lost
		m_ControlManager->DrawControls();

		// Title bar and border is drawn over any other things
		m_TitleBar->Draw();

		// When the window is maximized, the border is not drawn
		if (!m_WinBase->IsWindowMaximized())
			m_WindowBorder->Draw();
	}

	if (FAILED(m_WinBase->EndRender()))
	{
		// If EndRender() failes, the device is lost
		m_pFont->OnLostDevice();

		if (SUCCEEDED(m_WinBase->ResetDevice()))
		{
			m_pFont->OnResetDevice();
			m_WindowBorder->SetSize(m_WinBase->GetWindowSize());
		}
	}
}