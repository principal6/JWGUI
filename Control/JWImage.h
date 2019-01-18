#pragma once

#include "JWControl.h"

namespace JW_GUI
{
	class JWImage : public JWControl
	{
	private:
		UNIQUE_PTR<JWShape> m_BG;

	public:
		JWImage() {};
		virtual ~JWImage() {};

		// Creation
		virtual auto JWImage::Create(LPDIRECT3DDEVICE9 pDevice)->Error override;

		// MakeOutter
		virtual void JWImage::MakeImage(WSTRING TextureFileName, D3DXVECTOR2 Size = D3DXVECTOR2(0, 0));

		// Draw
		virtual void JWImage::Draw();

		// Setter
		virtual void JWImage::SetSize(D3DXVECTOR2 Size);
		virtual void JWImage::SetControlPosition(D3DXVECTOR2 Position) override;
	};
};