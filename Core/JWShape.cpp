#include "JWShape.h"

using namespace JW_GUI;

JWShape::JWShape()
{
	m_pDevice = nullptr;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexture = nullptr;

	m_Position = D3DXVECTOR2(0.0f, 0.0f);
	m_Size = D3DXVECTOR2(0.0f, 0.0f);
	m_Color = 0xFFFFFFFF;
}

auto JWShape::Create(LPDIRECT3DDEVICE9 pDevice)->Error
{
	if (nullptr == (m_pDevice = pDevice))
		return Error::NullDevice;

	ClearVertexAndIndexData();

	return Error::Ok;
}

void JWShape::Destroy()
{
	m_pDevice = nullptr;

	ClearVertexAndIndexData();

	JW_RELEASE(m_pTexture);
	JW_RELEASE(m_pIndexBuffer);
	JW_RELEASE(m_pVertexBuffer);
}

void JWShape::MakeRectangle(D3DXVECTOR2 Size, DWORD Color)
{
	m_Type = Type::Rectangle;
	m_Size = Size;

	// Make rectangle from (0, 0) to (Size.x, Size.y)
	m_Vertices.push_back(VertexShape(0, 0, Color));
	m_Vertices.push_back(VertexShape(Size.x, 0, Color));
	m_Vertices.push_back(VertexShape(0, Size.y, Color));
	m_Vertices.push_back(VertexShape(Size.x, Size.y, Color));
	m_Indices.push_back(Index3(0, 1, 3));
	m_Indices.push_back(Index3(0, 3, 2));

	CreateVertexBuffer();
	CreateIndexBuffer();
	UpdateVertexBuffer();
	UpdateIndexBuffer();
}

void JWShape::MakeImage(D3DXVECTOR2 Size, WSTRING TextureFileName)
{
	m_Type = Type::Rectangle;
	m_Size = Size;

	// Make rectangle from (0, 0) to (Size.x, Size.y)
	m_Vertices.push_back(VertexShape(0, 0, 0, 0));
	m_Vertices.push_back(VertexShape(Size.x, 0, 1, 0));
	m_Vertices.push_back(VertexShape(0, Size.y, 0, 1));
	m_Vertices.push_back(VertexShape(Size.x, Size.y, 1, 1));
	m_Indices.push_back(Index3(0, 1, 3));
	m_Indices.push_back(Index3(0, 3, 2));

	CreateVertexBuffer();
	CreateIndexBuffer();
	UpdateVertexBuffer();
	UpdateIndexBuffer();

	CreateTexture(TextureFileName);
}

void JWShape::CreateTexture(WSTRING FileName)
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	WSTRING NewFileName = ASSET_DIR;
	NewFileName += FileName;

	D3DXIMAGE_INFO tempImageInfo;
	if (FAILED(D3DXCreateTextureFromFileExW(m_pDevice, NewFileName.c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, &tempImageInfo, nullptr, &m_pTexture)))
		return;

	D3DXVECTOR2 tempSize;
	tempSize.x = static_cast<float>(tempImageInfo.Width);
	tempSize.y = static_cast<float>(tempImageInfo.Height);

	// Set the image size to the file info's size because the size was not specified when JWShape was created
	if (m_Size.x == 0 && m_Size.y == 0)
		SetSize(tempSize);
}

void JWShape::ClearVertexAndIndexData()
{
	m_Vertices.clear();
	m_Indices.clear();
}

void JWShape::CreateVertexBuffer()
{
	if (m_Vertices.size())
	{
		int rVertSize = sizeof(VertexShape) * static_cast<int>(m_Vertices.size());
		if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0, D3DFVF_ALL, D3DPOOL_MANAGED, &m_pVertexBuffer, nullptr)))
		{
			return;
		}
	}
}

void JWShape::CreateIndexBuffer()
{
	if (m_Indices.size())
	{
		int rIndSize = sizeof(Index3) * static_cast<int>(m_Indices.size());
		if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, nullptr)))
		{
			return;
		}
	}
}

void JWShape::UpdateVertexBuffer()
{
	if (m_Vertices.size())
	{
		int rVertSize = sizeof(VertexShape) * static_cast<int>(m_Vertices.size());
		VOID* pVertices;
		if (FAILED(m_pVertexBuffer->Lock(0, rVertSize, (void**)&pVertices, 0)))
		{
			return;
		}
		memcpy(pVertices, &m_Vertices[0], rVertSize);
		m_pVertexBuffer->Unlock();
	}
}

void JWShape::UpdateIndexBuffer()
{
	if (m_Indices.size())
	{
		int rIndSize = sizeof(Index3) * static_cast<int>(m_Indices.size());
		VOID* pIndices;
		if (FAILED(m_pIndexBuffer->Lock(0, rIndSize, (void **)&pIndices, 0)))
		{
			return;
		}
		memcpy(pIndices, &m_Indices[0], rIndSize);
		m_pIndexBuffer->Unlock();
	}
}

void JWShape::Draw() const
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

		// Texture alpha * Diffuse color alpha
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	}
	else
	{
		// No texture
		m_pDevice->SetTexture(0, nullptr);
	}

	m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VertexShape));
	m_pDevice->SetFVF(D3DFVF_ALL);
	m_pDevice->SetIndices(m_pIndexBuffer);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		static_cast<int>(m_Vertices.size()), 0, static_cast<int>(m_Indices.size()));
}

void JWShape::SetPosition(D3DXVECTOR2 Position)
{
	// Set position as offset
	for (VertexShape& iterator : m_Vertices)
	{
		iterator.x = iterator.x - m_Position.x;
		iterator.y = iterator.y - m_Position.y;
	}

	m_Position = Position;

	for (VertexShape& iterator : m_Vertices)
	{
		iterator.x = iterator.x + m_Position.x;
		iterator.y = iterator.y + m_Position.y;
	}

	UpdateVertexBuffer();
}

void JWShape::SetSize(D3DXVECTOR2 Size)
{
	m_Size = Size;

	switch (m_Type)
	{
	case JWShape::Type::Rectangle:
		m_Vertices[1].x = m_Position.x + m_Size.x;
		m_Vertices[2].y = m_Position.y + m_Size.y;
		m_Vertices[3].x = m_Position.x + m_Size.x;
		m_Vertices[3].y = m_Position.y + m_Size.y;
		break;
	default:
		break;
	}

	UpdateVertexBuffer();
}

void JWShape::SetXRGB(DWORD XRGB)
{
	if (m_Vertices.size())
	{
		JW_GUI::SetXRGB(&m_Color, XRGB);

		for (VertexShape& iterator : m_Vertices)
		{
			JW_GUI::SetXRGB(&iterator.color, XRGB);
		}

		UpdateVertexBuffer();
	}
}

void JWShape::SetAlpha(BYTE Alpha)
{
	if (m_Vertices.size())
	{
		JW_GUI::SetAlpha(&m_Color, Alpha);

		for (VertexShape& iterator : m_Vertices)
		{
			JW_GUI::SetAlpha(&iterator.color, Alpha);
		}

		UpdateVertexBuffer();
	}
}

auto JWShape::GetPosition() const->D3DXVECTOR2
{
	return m_Position;
}

auto JWShape::GetSize() const->D3DXVECTOR2
{
	return m_Size;
}

auto JWShape::GetAlpha() const->BYTE
{
	return JW_GUI::GetAlpha(m_Color);
}

auto JWShape::GetXRGB() const->DWORD
{
	return JW_GUI::GetXRGB(m_Color);
}