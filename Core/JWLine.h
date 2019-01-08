#pragma once

#include "JWCommon.h"

class JWLine
{
private:
	struct VertexLine
	{
		FLOAT x, y, z, rhw;
		DWORD color;

		VertexLine() : x(0), y(0), z(0), rhw(1), color(0xffffffff) {};
		VertexLine(float _x, float _y, DWORD _color) : x(_x), y(_y), z(0), rhw(1), color(_color) {};
		VertexLine(float _x, float _y, float _z, float _rhw, DWORD _color) :
			x(_x), y(_y), z(_z), rhw(_rhw), color(_color) {};
	};

	struct Index2
	{
		WORD _0, _1;

		Index2() : _0(0), _1(0) {};
		Index2(int ID0, int ID1) : _0(ID0), _1(ID1) {};
	};

private:
	static LPDIRECT3DDEVICE9 m_pDevice;

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
	auto JWLine::Create(LPDIRECT3DDEVICE9 pDevice)->JWERROR;
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