#pragma once

#include "JWCommon.h"
#include "JWLine.h"

namespace JW_GUI
{
	class JWBorder final
	{
	private:
		LPDIRECT3DDEVICE9 m_pDevice;

		UNIQUE_PTR<JWLine> m_Line;
		D3DXVECTOR2 m_Position;
		D3DXVECTOR2 m_Size;

	public:
		JWBorder() {};
		~JWBorder() {};

		auto JWBorder::Create(LPDIRECT3DDEVICE9 pDevice)->Error;

		void JWBorder::MakeBorder(D3DXVECTOR2 Size);
		void JWBorder::UpdateBorder(D3DXVECTOR2 Size);

		void JWBorder::Draw();
	};
};