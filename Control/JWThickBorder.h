#pragma once

#include "JWControl.h"

class JWThickBorder : public JWControl
{
private:
	REGION m_Region[8];
	bool m_RegionWithMouseOver[8];
	FLAG m_MoveID;
	bool m_bCanResizeWindow;
	FLAG m_CapturedMoveID;

private:
	virtual void JWThickBorder::SetRegion() override;
	auto JWThickBorder::IsMouseOnRegion(JW_INT2 MousePosition)->bool;
	void JWThickBorder::SetCursorAndMoveID();

public:
	JWThickBorder();
	virtual ~JWThickBorder() {};

	virtual auto JWThickBorder::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->JWERROR override;

	virtual void JWThickBorder::MakeThickBorder(D3DXVECTOR2 Size);

	virtual void JWThickBorder::Draw();

	virtual void JWThickBorder::SetSize(D3DXVECTOR2 Size);

	virtual void JWThickBorder::UpdateControlState(JWWinBase* pBase);
	
	virtual auto JWThickBorder::GetCapturedMoveID()->FLAG;

	virtual auto JWThickBorder::CanResizeWindow(POINT ClientCapturedPosition)->bool;
	void JWThickBorder::StopResizeWindow();
	void JWThickBorder::ResizeWindow(JWWinBase* pBase);
};