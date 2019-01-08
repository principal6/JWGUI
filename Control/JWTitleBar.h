#pragma once

#include "../Core/JWWinBase.h"
#include "../Control/JWButton.h"
#include "../Control/JWLabel.h"

class JWTitlebar final : public JWControl
{
private:
	static const float TITLEBAR_HEIGHT;
	static const float ICON_WIDTH;
	static const float SYSBUTTON_WIDTH;

	UNIQUE_PTR<JWButton> m_SysExit;
	UNIQUE_PTR<JWButton> m_SysMin;
	UNIQUE_PTR<JWButton> m_SysMax;
	UNIQUE_PTR<JWLabel> m_Label;
	UNIQUE_PTR<JWShape> m_BG;

	bool m_bOnSystemMinimize;
	bool m_bOnSystemMaximize;
	bool m_bOnSystemExit;
	bool m_bOnWindowMove;

public:
	JWTitlebar();
	~JWTitlebar() {};

	// Creation
	auto JWTitlebar::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->JWERROR override;

	// Make
	void JWTitlebar::Make(JW_INT2 WindowSize, WSTRING WindowName);

	// Draw
	void JWTitlebar::Draw();

	// Update
	void JWTitlebar::Update(JWWinBase* pBase);
	void JWTitlebar::UpdateSize(JW_INT2 WindowSize);
	void JWTitlebar::UpdateControlStates(JWWinBase* pBase);

	// Recall
	auto JWTitlebar::OnSystemMinimize()->bool;
	auto JWTitlebar::OnSystemMaximize()->bool;
	auto JWTitlebar::OnSystemExit() const->bool;

	// Move or stop the window
	auto JWTitlebar::CanMoveWindow(POINT CapturedMousePositionClient)->bool;
	void JWTitlebar::StopWindow();
	void JWTitlebar::ToggleSysMaxButton();
	void JWTitlebar::DoubleClickMaximize(POINT MouePosition);
};