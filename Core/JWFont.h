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

	private:
		void JWFont::ClearVertexAndIndexData();

	public:
		JWFont();
		~JWFont() {};

		// Creation & destruction
		auto JWFont::Create(LPDIRECT3DDEVICE9 pDevice)->Error;
		void JWFont::Destroy();

		auto JWFont::MakeFont(STRING FileName_FNT)->Error;
	};
};