#pragma once

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

		POINT m_MousePosition;
		POINT m_MouseDownPosition;

	protected:
		virtual auto JWControl::MakeBorder()->Error;
		virtual void JWControl::UpdateBorder();

		virtual auto JWControl::IsMouseOnRegion(Int2 MousePosition) const->bool;

	public:
		JWControl();
		virtual ~JWControl() {};

		// Creation
		virtual auto JWControl::Create(LPDIRECT3DDEVICE9 pDevice)->Error;

		// Make
		// @warning: these functions are defined in each sub-class
		virtual void JWControl::MakeLabel(WSTRING Text, D3DXVECTOR2 Size, DWORD ColorFont = JWCOLOR_FONT,
			DWORD ColorBG = JWCOLOR_LABEL) {};

		// Update
		virtual void JWControl::UpdateState(Int2 MousePosition, Int2 MouseDownPosition, bool IsLeftButtonDown);

		// Draw
		virtual void JWControl::Draw() {};
		virtual void JWControl::DrawBorder();
		virtual void JWControl::DrawString();

		// Setter
		virtual void JWControl::SetRegion();
		virtual void JWControl::SetControlState(CONTROL_STATE State);
		virtual void JWControl::SetBackgroundAlpha(BYTE Alpha) {};
		virtual void JWControl::SetBackgroundXRGB(DWORD XRGB) {};
		virtual void JWControl::SetAlignmentHorz(ALIGNMENT_HORZ Align);
		virtual void JWControl::SetAlignmentVert(ALIGNMENT_VERT Align);
		virtual void JWControl::SetDrawBorder(bool Value);
		virtual void JWControl::SetSize(D3DXVECTOR2 Size) {};
		virtual void JWControl::SetPosition(D3DXVECTOR2 Position) {};
		virtual void JWControl::SetFontAlpha(BYTE Alpha);
		virtual void JWControl::SetFontXRGB(DWORD XRGB);

		// Getter only
		// This function has no setter, becuase control type is decided when the control is created
		virtual auto JWControl::GetControlType() const->CONTROL_TYPE;

		// Getter
		virtual auto JWControl::GetRegion() const->REGION;
		virtual auto JWControl::GetControlState() const->CONTROL_STATE;
		virtual auto JWControl::GetAlignmentHorz() const->ALIGNMENT_HORZ;
		virtual auto JWControl::GetAlignmentVert() const->ALIGNMENT_VERT;
		virtual auto JWControl::GetDrawBorder() const->bool;
		virtual auto JWControl::GetPosition() const->D3DXVECTOR2;
	};
};
