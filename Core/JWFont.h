#pragma once

#include "JWCommon.h"
#include "../Dependency/BMFontParser.h"

using namespace BMF;

namespace JW_GUI
{
	class JWFont final : public BMFontParser
	{
	private:
		LPDIRECT3DDEVICE9 m_pDevice;
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
		LPDIRECT3DTEXTURE9 m_pTexture;

		std::vector<VertexShape> m_Vertices;
		std::vector<Index3> m_Indices;

		D3DXVECTOR2 m_PositionOffset;

		wchar_t m_String[MAX_TEXT_LEN];
		size_t m_StringLen;

		wchar_t m_StringCharID[MAX_TEXT_LEN];
		size_t m_StringCharLen;
		size_t m_StringCharAdvance;

	private:
		void JWFont::ClearString();
		void JWFont::ClearVertexAndIndexData();

		auto JWFont::CreateVertexBuffer()->Error;
		auto JWFont::CreateIndexBuffer()->Error;
		auto JWFont::CreateTexture(WSTRING FileName)->Error;

		auto JWFont::UpdateVertexBuffer()->Error;
		auto JWFont::UpdateIndexBuffer()->Error;

		void JWFont::AddChar(wchar_t CharID, wchar_t Character);

	public:
		JWFont();
		~JWFont() {};

		// Creation & destruction
		auto JWFont::Create(LPDIRECT3DDEVICE9 pDevice)->Error;
		void JWFont::Destroy();

		auto JWFont::MakeFont(WSTRING FileName_FNT)->Error;

		//auto JWFont::SetText(WSTRING Text)->Error;
		auto JWFont::SetText(WSTRING Text)->Error;
		auto JWFont::SetPosition(D3DXVECTOR2 Offset)->Error;
		auto JWFont::SetAlpha(BYTE Alpha)->Error;
		auto JWFont::SetXRGB(DWORD Color)->Error;

		void JWFont::Draw() const;
	};
};