#pragma once

#include "JWCommon.h"

namespace JW_GUI
{
	class JWLine final
	{
	private:
		LPDIRECT3DDEVICE9 m_pDevice;
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;

		std::vector<VertexLine> m_Vertices;
		std::vector<Index2> m_Indices;

		D3DXVECTOR2 m_Position;
		int m_Alpha;

	protected:
		void JWLine::CreateVertexBuffer();
		void JWLine::CreateIndexBuffer();
		void JWLine::UpdateVertexBuffer();
		void JWLine::UpdateIndexBuffer();

	public:
		JWLine();
		~JWLine() {};

		// Creation & destruction
		auto JWLine::Create(LPDIRECT3DDEVICE9 pDevice)->Error;
		void JWLine::Destroy();

		// Clear
		void JWLine::ClearVertexAndIndexData();

		// Add
		void JWLine::AddLine(D3DXVECTOR2 StartPos, D3DXVECTOR2 Length, DWORD Color);
		void JWLine::AddBox(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size, DWORD Color);
		void JWLine::AddEnd();

		// Update
		void JWLine::UpdateLine(int LineID, D3DXVECTOR2 StartPos, D3DXVECTOR2 Length, DWORD Color);

		// Draw
		void JWLine::Draw() const;

		// Setter
		void JWLine::SetAlpha(int Alpha);
		void JWLine::SetPosition(D3DXVECTOR2 Position);

		// Getter
		auto JWLine::GetAlpha() const->int;
		auto JWLine::GetPosition() const->D3DXVECTOR2;
	};
};