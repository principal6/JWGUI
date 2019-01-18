#include "JWInnerWindow.h"

using namespace JW_GUI;

auto JWInnerWindow::Create(WSTRING Name, Int2 Position, Int2 WindowSize, DWORD BackColor)->Error
{
	JWWindow::Create(Name, Position, WindowSize, BackColor);

	return Error::Ok;
}

auto JWInnerWindow::CreateGUI(JWDXDevice* pDXDevice)->Error
{
	if (JW_FAILED(JWWindow::CreateGUI(pDXDevice)))
		return Error::GUINotCreated;

	m_WindowBorder->MakeThickBorder(D3DXVECTOR2(static_cast<float>(m_WindowSize.x - 1),
		static_cast<float>(m_WindowSize.y - 1)));

	m_TitleBar->MakeInner(m_WindowSize, m_CaptionString);

	m_BG = MAKE_UNIQUE(JWShape)();
	if (JW_FAILED(m_BG->Create(pDXDevice->GetDevice())))
		return Error::ShapeNotCreated;

	m_BG->MakeRectangle(m_WindowSize, m_BackColor);

	return Error::Ok;
}

void JWInnerWindow::Draw()
{
	// Draw background
	m_BG->Draw();

	// Draw controls
	if (m_pControls.size())
	{
		for (JWControl* iterator : m_pControls)
		{
			iterator->Draw();
		}
	}

	// Draw the title bar
	m_TitleBar->Draw();

	// Draw the thick border, but when the window is maximized, it's not drawn
	if (m_WindowState != WindowState::Maximized)
		m_WindowBorder->Draw();
}

void JWInnerWindow::UpdateControls()
{
	JWWindow::UpdateControls();

	if (m_pControls.size())
	{
		for (JWControl* iterator : m_pControls)
		{
			iterator->SetWindowOffset(m_WindowPosition, m_OutterWindowPosition);
		}
	}

	m_WindowBorder->SetWindowOffset(m_WindowPosition, m_OutterWindowPosition);
	m_TitleBar->SetWindowOffset(m_WindowPosition, m_OutterWindowPosition);
}

void JWInnerWindow::UpdateOutterWindowInfo(Int2 OutterWindowPosition, Int2 OutterWindowSize)
{
	m_OutterWindowPosition = OutterWindowPosition;
	m_OutterWindowSize = OutterWindowSize;
}

void JWInnerWindow::UpdateWindow(bool FreezeWindow)
{
	JWWindow::UpdateWindow(FreezeWindow);

	m_BG->SetSize(m_WindowSize);
	m_BG->SetPosition(m_WindowPosition);
}

void JWInnerWindow::MinimizeWindow()
{
	JWWindow::MinimizeWindow();

	UpdateWindowSize();
}

void JWInnerWindow::MaximizeWindow()
{
	JWWindow::MaximizeWindow();

	if (m_WindowState == WindowState::Normal)
	{
		// Unmaximizing
		SetWindowSize(m_WindowSizePrev);
		SetWindowPosition(m_WindowPositionPrev);
	}
	else
	{
		// Maximizing
		m_WindowSizePrev = m_WindowSize;
		m_WindowPositionPrev = m_WindowPosition;

		int W = m_OutterWindowSize.x;
		int H = m_OutterWindowSize.y - m_TitleBar->GetTitleBarHeight();

		SetWindowSize(Int2(W, H));
		SetWindowPosition(Int2(0, m_TitleBar->GetTitleBarHeight()));
	}

	UpdateWindowSize();
}

void JWInnerWindow::UpdateMaximizedSize()
{
	if (m_WindowState == WindowState::Maximized)
	{
		int W = m_OutterWindowSize.x;
		int H = m_OutterWindowSize.y - m_TitleBar->GetTitleBarHeight();

		SetWindowSize(Int2(W, H));
		//SetWindowPosition(Int2(0, m_TitleBar->GetTitleBarHeight()));

		UpdateWindowSize();
	}
}

void JWInnerWindow::RestoreWindow()
{
	JWWindow::RestoreWindow();

	UpdateWindowSize();
}

void JWInnerWindow::MoveWindow()
{
	Int2 CapMousePos = GetClientCapturedMousePosition();
	Int2 MousePos = GetClientMousePosition();
	Int2 WinPos = GetCapturedWindowPosition();

	Int2 NewPos = WinPos + MousePos - CapMousePos;

	SetWindowPosition(NewPos);
}

void JWInnerWindow::OnMouseDoubleClick()
{
	m_TitleBar->DoubleClickMaximize(GetClientMousePosition());
}