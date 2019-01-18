#include "JWFont.h"

using namespace JW_GUI;

JWFont::JWFont()
{
	m_pDevice = nullptr;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexture = nullptr;

	ClearString();
}

void JWFont::ClearString()
{
	memset(m_String, 0, MAX_TEXT_LEN);
	memset(m_StringCharID, 0, MAX_TEXT_LEN);
	m_StringLen = 0;
	m_StringCharLen = 0;
	m_StringCharAdvance = 0;
}

void JWFont::ClearVertexAndIndexData()
{
	m_Vertices.clear();
	m_Indices.clear();
}

auto JWFont::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (nullptr == (m_pDevice = pDevice))
		return Error::NullDevice;

	ClearVertexAndIndexData();

	return Error::Ok;
}

void JWFont::Destroy()
{
	m_pDevice = nullptr;

	ClearVertexAndIndexData();
	ClearString();

	JW_RELEASE(m_pTexture);
	JW_RELEASE(m_pIndexBuffer);
	JW_RELEASE(m_pVertexBuffer);
}

auto JWFont::MakeFont(WSTRING FileName_FNT)->Error
{
	WSTRING NewFileName = ASSET_DIR + FileName_FNT;

	if (Parse(NewFileName))
	{
		// MakeOutter rectangles with max size (MAX_TEXT_LEN)
		for (UINT i = 0; i < MAX_TEXT_LEN; i++)
		{
			m_Vertices.push_back(VertexShape(0, 0, 0, 0));
			m_Vertices.push_back(VertexShape(0, 0, 1, 0));
			m_Vertices.push_back(VertexShape(0, 0, 0, 1));
			m_Vertices.push_back(VertexShape(0, 0, 1, 1));
			m_Indices.push_back(Index3(i * 4, i * 4 + 1, i * 4 + 3));
			m_Indices.push_back(Index3(i * 4, i * 4 + 3, i * 4 + 2));
		}

		CreateVertexBuffer();
		CreateIndexBuffer();
		UpdateVertexBuffer();
		UpdateIndexBuffer();

		// JWFont will always use only one page in the BMFont, whose ID is '0'
		if (JW_FAILED(CreateTexture(m_FontData.Pages[0].File)))
			return Error::TextureNotCreated;

		return Error::Ok;
	}

	return Error::FontNotCreated;
}

auto JWFont::CreateTexture(WSTRING FileName)->Error
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	WSTRING NewFileName = ASSET_DIR;
	NewFileName += FileName;

	// Craete texture without color key
	if (FAILED(D3DXCreateTextureFromFileExW(m_pDevice, NewFileName.c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &m_pTexture)))
		return Error::TextureNotCreated;

	return Error::Ok;
}

auto JWFont::CreateVertexBuffer()->Error
{
	if (m_Vertices.size())
	{
		int rVertSize = sizeof(VertexShape) * static_cast<int>(m_Vertices.size());
		if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0, D3DFVF_ALL, D3DPOOL_MANAGED, &m_pVertexBuffer, nullptr)))
		{
			return Error::VertexBufferNotCreated;
		}
		return Error::Ok;
	}
	return Error::NullVertex;
}

auto JWFont::CreateIndexBuffer()->Error
{
	if (m_Indices.size())
	{
		int rIndSize = sizeof(Index3) * static_cast<int>(m_Indices.size());
		if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, nullptr)))
		{
			return Error::IndexBufferNotCreated;
		}
		return Error::Ok;
	}
	return Error::NullIndex;
}

auto JWFont::UpdateVertexBuffer()->Error
{
	if (m_Vertices.size())
	{
		int rVertSize = sizeof(VertexShape) * static_cast<int>(m_Vertices.size());
		VOID* pVertices;
		if (FAILED(m_pVertexBuffer->Lock(0, rVertSize, (void**)&pVertices, 0)))
		{
			return Error::VertexBufferNotLocked;
		}
		memcpy(pVertices, &m_Vertices[0], rVertSize);
		m_pVertexBuffer->Unlock();
		return Error::Ok;
	}
	return Error::NullVertex;
}

auto JWFont::UpdateIndexBuffer()->Error
{
	if (m_Indices.size())
	{
		int rIndSize = sizeof(Index3) * static_cast<int>(m_Indices.size());
		VOID* pIndices;
		if (FAILED(m_pIndexBuffer->Lock(0, rIndSize, (void **)&pIndices, 0)))
		{
			return Error::IndexBufferNotLocked;
		}
		memcpy(pIndices, &m_Indices[0], rIndSize);
		m_pIndexBuffer->Unlock();
		return Error::Ok;
	}
	return Error::NullIndex;
}

auto JWFont::SetText(WSTRING Text)->Error
{
	if (Text.size() > MAX_TEXT_LEN)
		return Error::NotEnoughBuffer;

	ClearString();

	wcscpy_s(m_String, Text.c_str());
	m_StringLen = Text.length();

	size_t Offset = 0;
	wchar_t Character = 0;
	wchar_t CharID = 0;

	// MakeOutter text images from the text string
	for (size_t i = 0; i < m_StringLen; i++)
	{
		Character = m_String[i];

		// Find corresponding char item in m_FontData.Chars
		for (size_t j = 0; j < m_FontData.Chars.size(); j++)
		{
			if (m_FontData.Chars[j].ID == Character)
			{
				CharID = static_cast<wchar_t>(j);
				break;
			}
		}

		// Null end is not used in image string
		if (Character)
			AddChar(CharID, Character);
	}

	UpdateVertexBuffer();

	return Error::Ok;
}

void JWFont::AddChar(wchar_t CharID, wchar_t Character)
{
	size_t vertexID = m_StringCharLen * 4;

	float u1 = static_cast<float>(m_FontData.Chars[CharID].X) / static_cast<float>(m_FontData.Common.ScaleW);
	float v1 = static_cast<float>(m_FontData.Chars[CharID].Y) / static_cast<float>(m_FontData.Common.ScaleH);
	float u2 = u1 + static_cast<float>(m_FontData.Chars[CharID].Width) / static_cast<float>(m_FontData.Common.ScaleW);
	float v2 = v1 + static_cast<float>(m_FontData.Chars[CharID].Height) / static_cast<float>(m_FontData.Common.ScaleH);

	int xoffset = m_FontData.Chars[CharID].XOffset;
	int yoffset = m_FontData.Chars[CharID].YOffset;

	if (m_StringCharLen)
	{
		int prevCharID = m_StringCharID[m_StringCharLen - 1];
		m_StringCharAdvance += m_FontData.Chars[prevCharID].XAdvance;
	}

	UINT sizex = m_FontData.Chars[CharID].Width;
	UINT sizey = m_FontData.Chars[CharID].Height;

	float x1 = static_cast<float>(m_StringCharAdvance) + static_cast<float>(xoffset);
	float x2 = x1 + static_cast<float>(sizex);
	float y1 = static_cast<float>(yoffset);
	float y2 = y1 + static_cast<float>(sizey);

	m_Vertices[vertexID] = VertexShape(x1, y1, u1, v1);
	m_Vertices[vertexID + 1] = VertexShape(x2, y1, u2, v1);
	m_Vertices[vertexID + 2] = VertexShape(x1, y2, u1, v2);
	m_Vertices[vertexID + 3] = VertexShape(x2, y2, u2, v2);

	m_StringCharID[m_StringCharLen] = CharID;

	m_StringCharLen++;
}

auto JWFont::SetPosition(D3DXVECTOR2 Offset)->Error
{
	if (m_Vertices.size())
	{
		// Set position as offset
		for (VertexShape& iterator : m_Vertices)
		{
			iterator.x = iterator.x - m_PositionOffset.x;
			iterator.y = iterator.y - m_PositionOffset.y;
		}

		m_PositionOffset = Offset;

		for (VertexShape& iterator : m_Vertices)
		{
			iterator.x = iterator.x + m_PositionOffset.x;
			iterator.y = iterator.y + m_PositionOffset.y;
		}

		UpdateVertexBuffer();
		return Error::Ok;
	}
	return Error::NullVertex;
}

auto JWFont::SetAlpha(BYTE Alpha)->Error
{
	if (m_Vertices.size())
	{
		for (VertexShape& iterator : m_Vertices)
		{
			JW_GUI::SetAlpha(&iterator.color, Alpha);
		}

		UpdateVertexBuffer();
	}
	return Error::NullVertex;
}

auto JWFont::SetXRGB(DWORD Color)->Error
{
	if (m_Vertices.size())
	{
		for (VertexShape& iterator : m_Vertices)
		{
			JW_GUI::SetXRGB(&iterator.color, Color);
		}

		UpdateVertexBuffer();
	}
	return Error::NullVertex;
}

void JWFont::Draw() const
{
	// Set alpha blending on
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	if (m_pTexture)
	{
		// Texture exists
		m_pDevice->SetTexture(0, m_pTexture);

		// Texture alpha * Diffuse alpha
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		// Texture color * Diffuse color
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	}

	m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VertexShape));
	m_pDevice->SetFVF(D3DFVF_ALL);
	m_pDevice->SetIndices(m_pIndexBuffer);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		static_cast<int>(m_Vertices.size()), 0, static_cast<int>(m_Indices.size()));
}