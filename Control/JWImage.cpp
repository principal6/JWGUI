#include "JWImage.h"

auto JWImage::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->JWERROR
{
	if (JW_FAILED(JWControl::Create(pDevice, pFont)))
		return JWERROR::ControlNotCreated;

	m_Shape = UNIQUE_PTR<JWShape>(new JWShape);
	if (JW_FAILED(m_Shape->Create(pDevice)))
		return JWERROR::ShapeNotCreated;

	m_ControlType = CONTROL_TYPE::Image;
	m_ControlState = CONTROL_STATE::Normal;

	return JWERROR::Ok;
}

void JWImage::MakeImage(WSTRING TextureFileName, D3DXVECTOR2 Size)
{
	// Create background
	m_Shape->MakeImage(Size, TextureFileName);

	// Create border line
	JWControl::MakeBorder();

	// Update control region
	SetRegion();
}

void JWImage::Draw()
{
	// Draw background
	m_Shape->Draw();

	// Draw border
	JWControl::DrawBorder();
}

void JWImage::SetSize(D3DXVECTOR2 Size)
{
	m_Size = Size;

	// Set background size
	m_Shape->SetSize(m_Size);

	// Set border size
	JWControl::UpdateBorder();

	// Update control region
	SetRegion();
}

void JWImage::SetPosition(D3DXVECTOR2 Position)
{
	m_Position = Position;

	// Set background position
	m_Shape->SetPosition(Position);

	// Set border position
	JWControl::UpdateBorder();

	// Set control region
	SetRegion();
}