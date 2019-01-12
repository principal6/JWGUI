#include "JWFont.h"

using namespace JW_GUI;

JWFont::JWFont()
{
	m_pDevice = nullptr;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexture = nullptr;

	ClearVertexAndIndexData();
}

void JWFont::ClearVertexAndIndexData()
{
	m_Vertices.clear();
	m_Indices.clear();
}

auto JWFont::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (pDevice == nullptr)
		return Error::NullDevice;

	m_pDevice = pDevice;
	return Error::Ok;
}

void JWFont::Destroy()
{
	m_pDevice = nullptr;

	ClearVertexAndIndexData();

	JW_RELEASE(m_pTexture);
	JW_RELEASE(m_pIndexBuffer);
	JW_RELEASE(m_pVertexBuffer);
}

auto JWFont::MakeFont(STRING FileName_FNT)->Error
{
	if (Parse(FileName_FNT))
	{


		return Error::Ok;
	}

	return Error::FontNotCreated;
}