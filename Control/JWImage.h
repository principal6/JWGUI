#pragma once

#include "JWControl.h"

namespace JW_GUI
{
	class JWImage : public JWControl
	{
	private:
		UNIQUE_PTR<JWShape> m_Shape;

	public:
		JWImage() {};
		virtual ~JWImage() {};

		// Creation
		virtual auto JWImage::Create(LPDIRECT3DDEVICE9 pDevice)->Error override;

		// Make
		virtual void JWImage::MakeImage(WSTRING TextureFileName, D3DXVECTOR2 Size = D3DXVECTOR2(0, 0));

		// Draw
		virtual void JWImage::Draw();

		// Setter
		virtual void JWImage::SetSize(D3DXVECTOR2 Size);

		// Setter only (Getter is in JWControl class)
		virtual void JWImage::SetPosition(D3DXVECTOR2 Position);
	};
};