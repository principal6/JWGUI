#include "JWShape.h"

JWShape::JWShape()
{
	m_pDevice = nullptr;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexture = nullptr;

	ClearVertexAndIndexData();

	m_Position = D3DXVECTOR2(0.0f, 0.0f);
	m_Size = D3DXVECTOR2(0.0f, 0.0f);
	m_Alpha = 255;
}

auto JWShape::Create(LPDIRECT3DDEVICE9 pDevice)->JWERROR
{
	if (nullptr == (m_pDevice = pDevice))
		return JWERROR::NullDevice;

	ClearVertexAndIndexData();

	return JWERROR::Ok;
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
	
	// No texture
	m_pDevice->SetTexture(0, nullptr);

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

void JWShape::SetColor(DWORD Color)
{
	if (m_Vertices.size())
	{
		m_Color = Color;

		for (VertexShape& iterator : m_Vertices)
		{
			iterator.color = m_Color;
		}
		UpdateVertexBuffer();
	}
}

void JWShape::SetAlpha(int Alpha)
{
	if (m_Vertices.size())
	{
		Alpha = min(255, Alpha);
		Alpha = max(0, Alpha);
		m_Alpha = Alpha;

		for (VertexShape& iterator : m_Vertices)
		{
			iterator.color = D3DCOLOR_ARGB(m_Alpha, 255, 255, 255);
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

auto JWShape::GetAlpha() const->int
{
	return m_Alpha;
}