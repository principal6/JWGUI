#pragma once

#include "JWControl.h"

namespace JW_GUI
{
	class JWThickBorder : public JWControl
	{
	private:
		REGION m_Region[8];

		mutable bool m_RegionWithMouseOver[8];
		FLAG m_MoveID;
		bool m_bCanResizeWindow;
		FLAG m_CapturedMoveID;
		LPWSTR m_CursorID;
		LPWSTR m_CapturedCursorID;

	private:
		virtual void JWThickBorder::SetRegion() override;
		virtual auto JWThickBorder::IsMouseOnRegion(Int2 MousePosition) const->bool override;
		void JWThickBorder::SetCursorAndMoveID();

	public:
		JWThickBorder();
		virtual ~JWThickBorder() {};

		virtual auto JWThickBorder::Create(LPDIRECT3DDEVICE9 pDevice)->Error override;

		virtual void JWThickBorder::MakeThickBorder(D3DXVECTOR2 Size);

		virtual void JWThickBorder::UpdateState(Int2 MousePosition, Int2 MouseDownPosition, bool IsLeftButtonDown,
			bool StayPressed = true) override;

		virtual void JWThickBorder::Draw();

		virtual void JWThickBorder::SetSize(D3DXVECTOR2 Size);

		virtual auto JWThickBorder::GetCapturedMoveID()->FLAG;
		virtual auto JWThickBorder::GetCapturedCursorID()->LPCWSTR;

		virtual auto JWThickBorder::CanResizeWindow()->bool;
		virtual void JWThickBorder::StopResizeWindow();
	};
};