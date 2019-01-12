#pragma once

#include "../Core/JWWinBase.h"
#include "../Core/JWCommon.h"
#include "../Core/JWShape.h"
#include "../Core/JWLine.h"
#include "../Core/JWBorder.h"
#include "../Core/JWFont.h"

// Virtual class for every control
namespace JW_GUI
{
	class JWControl
	{
	public:
		struct REGION
		{
			int Left;
			int Right;
			int Top;
			int Bottom;

			REGION() : Left(0), Right(0), Top(0), Bottom(0) {};
			REGION(int L, int R, int T, int B) : Left(L), Right(R), Top(T), Bottom(B) {};
			REGION(D3DXVECTOR2 Position, D3DXVECTOR2 Size) :
				Left(static_cast<int>(Position.x)),
				Right(static_cast<int>(Position.x + Size.x)),
				Top(static_cast<int>(Position.y)),
				Bottom(static_cast<int>(Position.y + Size.y)) {};
		};

		enum class CONTROL_TYPE
		{
			Label,
			Button,
			ThickBorder,
			TitleBar,
			Image,
		};

		enum class CONTROL_STATE
		{
			Normal,
			Hover,
			Pressed,
			Clicked,
		};

	protected:
		LPDIRECT3DDEVICE9 m_pDevice;
		DWORD m_FontColor;

		UNIQUE_PTR<JWBorder> m_Border;
		UNIQUE_PTR<JWFont> m_Font;
		bool m_bDrawBorder;

		REGION m_Region;
		D3DXVECTOR2 m_Position;
		D3DXVECTOR2 m_Size;
		WSTRING m_Text;
		ALIGNMENT_HORZ m_AlignmentHorz;
		ALIGNMENT_VERT m_AlignmentVert;

		DWORD m_ColorBackground;
		DWORD m_ColorFont;
		CONTROL_TYPE m_ControlType;
		CONTROL_STATE m_ControlState;

	protected:
		virtual auto JWControl::MakeBorder()->Error;
		virtual void JWControl::UpdateBorder();
		virtual auto JWControl::IsMouseOnRegion(Int2 MousePosition) const->bool;

	public:
		JWControl();
		virtual ~JWControl() {};

		// Creation
		virtual auto JWControl::Create(LPDIRECT3DDEVICE9 pDevice)->Error;

		// Draw
		virtual void JWControl::DrawBorder();
		virtual void JWControl::DrawString();

		// Setter
		virtual void JWControl::SetRegion();
		virtual void JWControl::SetControlState(CONTROL_STATE State);
		virtual void JWControl::SetAlignmentHorz(ALIGNMENT_HORZ Align);
		virtual void JWControl::SetAlignmentVert(ALIGNMENT_VERT Align);
		virtual void JWControl::SetDrawBorder(bool Value);
		virtual void JWControl::SetFontColor(DWORD Color);

		// Getter only
		// This function has no setter, becuase control type is decided when the control is created
		virtual auto JWControl::GetControlType() const->CONTROL_TYPE;
		// This functions has its correspondent setter in sub-classes
		virtual auto JWControl::GetPosition() const->D3DXVECTOR2;

		// Getter
		virtual auto JWControl::GetRegion() const->REGION;
		virtual auto JWControl::GetControlState() const->CONTROL_STATE;
		virtual auto JWControl::GetAlignmentHorz() const->ALIGNMENT_HORZ;
		virtual auto JWControl::GetAlignmentVert() const->ALIGNMENT_VERT;
		virtual auto JWControl::GetDrawBorder() const->bool;

		// Update control state
		void JWControl::UpdateControlState(JWWinBase* pBase);
	};
};
