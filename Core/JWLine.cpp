#include "JWLine.h"

// Static member variable declaration
LPDIRECT3DDEVICE9 JWLine::m_pDevice = nullptr;

JWLine::JWLine()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_Position = D3DXVECTOR2(0.0f, 0.0f);
	m_Alpha = 255;
}

void JWLine::ClearVertexAndIndexData()
{
	m_Vertices.clear();
	m_Indices.clear();
}

auto JWLine::Create(LPDIRECT3DDEVICE9 pDevice)->JWERROR
{
	if (nullptr == (m_pDevice = pDevice))
		return JWERROR::NullDevice;
	
	ClearVertexAndIndexData();

	return JWERROR::Ok;
}

void JWLine::Destroy()
{
	m_pDevice = nullptr; // Just set to nullptr cuz it's newed in <DX9Base> class

	ClearVertexAndIndexData();

	JW_RELEASE(m_pIndexBuffer);
	JW_RELEASE(m_pVertexBuffer);
}

void JWLine::AddLine(D3DXVECTOR2 StartPos, D3DXVECTOR2 Length, DWORD Color)
{
	m_Vertices.push_back(VertexLine(StartPos.x, StartPos.y, Color));
	m_Vertices.push_back(VertexLine(StartPos.x + Length.x, StartPos.y + Length.y, Color));

	int tIndicesCount = static_cast<int>(m_Indices.size());
	m_Indices.push_back(Index2(tIndicesCount * 2, tIndicesCount * 2 + 1));
}

void JWLine::UpdateLine(int LineID, D3DXVECTOR2 StartPos, D3DXVECTOR2 Length, DWORD Color)
{
	if (m_Vertices.size())
	{
		m_Vertices[LineID * 2] = VertexLine(StartPos.x, StartPos.y, Color);
		m_Vertices[LineID * 2 + 1] = VertexLine(StartPos.x + Length.x, StartPos.y + Length.y, Color);

		UpdateVertexBuffer();
	}
}

void JWLine::AddBox(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size, DWORD Color)
{
	AddLine(StartPos, D3DXVECTOR2(Size.x, 0), Color);
	AddLine(StartPos, D3DXVECTOR2(0, Size.y), Color);
	AddLine(D3DXVECTOR2(StartPos.x + Size.x, StartPos.y), D3DXVECTOR2(0, Size.y), Color);
	AddLine(D3DXVECTOR2(StartPos.x, StartPos.y + Size.y), D3DXVECTOR2(Size.x, 0), Color);
}

void JWLine::AddEnd()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
	UpdateVertexBuffer();
	UpdateIndexBuffer();
}

void JWLine::CreateVertexBuffer()
{
	if (m_pVertexBuffer == nullptr)
	{
		int rVertSize = sizeof(VertexLine) * static_cast<int>(m_Vertices.size());
		if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0, D3DFVF_ALL, D3DPOOL_MANAGED, &m_pVertexBuffer, nullptr)))
		{
			return;
		}
	}
}

void JWLine::CreateIndexBuffer()
{
	if (m_pIndexBuffer == nullptr)
	{
		int rIndSize = sizeof(Index2) * static_cast<int>(m_Indices.size());
		if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, nullptr)))
		{
			return;
		}
	}
}
\
void JWLine::UpdateVertexBuffer()
{
	if (m_Vertices.size() > 0 && m_pVertexBuffer)
	{
		int rVertSize = sizeof(VertexLine) * static_cast<int>(m_Vertices.size());
		VOID* pVertices;
		if (FAILED(m_pVertexBuffer->Lock(0, rVertSize, (void**)&pVertices, 0)))
			return;
		memcpy(pVertices, &m_Vertices[0], rVertSize);
		m_pVertexBuffer->Unlock();
	}
}

void JWLine::UpdateIndexBuffer()
{
	if (m_Indices.size() > 0 && m_pIndexBuffer)
	{
		int rIndSize = sizeof(Index2) * static_cast<int>(m_Indices.size());
		VOID* pIndices;
		if (FAILED(m_pIndexBuffer->Lock(0, rIndSize, (void **)&pIndices, 0)))
			return;
		memcpy(pIndices, &m_Indices[0], rIndSize);
		m_pIndexBuffer->Unlock();
	}
}

void JWLine::Draw() const
{
	if (m_pDevice && m_pVertexBuffer && m_pIndexBuffer)
	{
		// Set alpha blending on
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		// No texture
		m_pDevice->SetTexture(0, nullptr);

		m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VertexLine));
		m_pDevice->SetFVF(D3DFVF_NO_TEXTURE);
		m_pDevice->SetIndices(m_pIndexBuffer);
		m_pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, static_cast<int>(m_Vertices.size()), 0, static_cast<int>(m_Indices.size()));
	}
}

void JWLine::SetAlpha(int Alpha)
{
	if (m_Vertices.size())
	{
		Alpha = min(255, Alpha);
		Alpha = max(0, Alpha);
		m_Alpha = Alpha;

		for (VertexLine& iterator : m_Vertices)
		{
			iterator.color = D3DCOLOR_ARGB(m_Alpha, 255, 255, 255);
		}
		UpdateVertexBuffer();
	}
}

void JWLine::SetPosition(D3DXVECTOR2 Position)
{
	// Set position as offset
	for (VertexLine& iterator : m_Vertices)
	{
		iterator.x = iterator.x - m_Position.x;
		iterator.y = iterator.y - m_Position.y;
	}

	m_Position = Position;

	for (VertexLine& iterator : m_Vertices)
	{
		iterator.x = iterator.x + m_Position.x;
		iterator.y = iterator.y + m_Position.y;
	}

	UpdateVertexBuffer();
}

auto JWLine::GetAlpha() const->int
{
	return m_Alpha;
}
auto JWLine::GetPosition() const->D3DXVECTOR2
{
	return m_Position;
}