#include "JWImage.h"

using namespace JW_GUI;

auto JWImage::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (JW_FAILED(JWControl::Create(pDevice)))
		return Error::ControlNotCreated;

	m_BG = MAKE_UNIQUE(JWShape)();
	if (JW_FAILED(m_BG->Create(pDevice)))
		return Error::ShapeNotCreated;

	m_ControlType = CONTROL_TYPE::Image;
	m_ControlState = CONTROL_STATE::Normal;

	return Error::Ok;
}

void JWImage::MakeImage(WSTRING TextureFileName, D3DXVECTOR2 Size)
{
	// Create background
	m_BG->MakeImage(Size, TextureFileName);

	// Create border line
	JWControl::MakeBorder();

	// Update control region
	SetRegion(m_Position);
}

void JWImage::Draw()
{
	// Draw background
	m_BG->Draw();

	// Draw border
	JWControl::DrawBorder();
}

void JWImage::SetSize(D3DXVECTOR2 Size)
{
	m_Size = Size;

	// Set background size
	m_BG->SetSize(m_Size);

	// Set border size
	JWControl::UpdateBorder();

	// Update control region
	SetRegion(m_Position);
}

void JWImage::SetControlPosition(D3DXVECTOR2 Position)
{
	// Set control position
	JWControl::SetControlPosition(Position);

	// Set background position
	m_BG->SetPosition(Position + m_InnerWindowPosition);

	// Set border position
	JWControl::UpdateBorder();

	// Set control region
	SetRegion(m_Position);
}