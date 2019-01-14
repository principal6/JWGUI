#pragma once

#include "JWCommon.h"

namespace JW_GUI
{
	class JWShape final
	{
	public:
		enum class Type
		{
			Rectangle,
		};

	protected:
		LPDIRECT3DDEVICE9 m_pDevice;
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
		LPDIRECT3DTEXTURE9 m_pTexture;

		std::vector<VertexShape> m_Vertices;
		std::vector<Index3> m_Indices;

		Type m_Type;
		D3DXVECTOR2 m_Size;
		D3DXVECTOR2 m_Position;
		DWORD m_Color;

	protected:
		void JWShape::ClearVertexAndIndexData();

		void JWShape::CreateVertexBuffer();
		void JWShape::CreateIndexBuffer();

		void JWShape::UpdateVertexBuffer();
		void JWShape::UpdateIndexBuffer();

		void JWShape::CreateTexture(WSTRING FileName);

	public:
		JWShape();
		~JWShape() {};

		// Creation & destruction
		auto JWShape::Create(LPDIRECT3DDEVICE9 pDevice)->Error;
		void JWShape::Destroy();

		// Make
		void JWShape::MakeRectangle(D3DXVECTOR2 Size, DWORD Color);
		void JWShape::MakeImage(D3DXVECTOR2 Size, WSTRING TextureFileName);

		// Draw
		void JWShape::Draw() const;

		// Setter
		void JWShape::SetPosition(D3DXVECTOR2 Position);
		void JWShape::SetSize(D3DXVECTOR2 Size);
		void JWShape::SetAlpha(BYTE Alpha);
		void JWShape::SetXRGB(DWORD XRGB);

		// Getter
		auto JWShape::GetPosition() const->D3DXVECTOR2;
		auto JWShape::GetSize() const->D3DXVECTOR2;
		auto JWShape::GetAlpha() const->BYTE;
		auto JWShape::GetXRGB() const->DWORD;
	};
};