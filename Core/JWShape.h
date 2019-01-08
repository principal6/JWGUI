#pragma once

#include "JWCommon.h"

class JWShape
{
public:
	enum class Type
	{
		Rectangle,
	};

protected:
	struct VertexShape
	{
		FLOAT x, y, z, rhw;
		DWORD color;
		FLOAT u, v;

		VertexShape() :
			x(0), y(0), z(0), rhw(1), color(0xFFFFFFFF), u(0), v(0) {};
		VertexShape(float _x, float _y, DWORD _color) :
			x(_x), y(_y), z(0), rhw(1), color(_color), u(0), v(0) {};
		VertexShape(float _x, float _y, float _u, float _v, DWORD _color) :
			x(_x), y(_y), z(0), rhw(1), color(_color), u(_u), v(_v) {};
		VertexShape(float _x, float _y, float _z, float _rhw, DWORD _color, float _u, float _v) :
			x(_x), y(_y), z(_z), rhw(_rhw), color(_color), u(_u), v(_v) {};
	};

	struct Index3
	{
		WORD _0, _1, _2;

		Index3() : _0(0), _1(0), _2(0) {};
		Index3(int ID0, int ID1, int ID2) : _0(ID0), _1(ID1), _2(ID2) {};
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
	int m_Alpha;

protected:
	virtual void JWShape::ClearVertexAndIndexData();

	virtual void JWShape::CreateVertexBuffer();
	virtual void JWShape::CreateIndexBuffer();

	virtual void JWShape::UpdateVertexBuffer();
	virtual void JWShape::UpdateIndexBuffer();

public:
	JWShape();
	virtual ~JWShape() {};

	// Creation & destruction
	virtual auto JWShape::Create(LPDIRECT3DDEVICE9 pDevice)->JWERROR;
	virtual void JWShape::Destroy();

	// Make
	virtual void JWShape::MakeRectangle(D3DXVECTOR2 Size, DWORD Color);

	// Draw
	virtual void JWShape::Draw() const;

	// Setter
	virtual void JWShape::SetPosition(D3DXVECTOR2 Position);
	virtual void JWShape::SetSize(D3DXVECTOR2 Size);
	virtual void JWShape::SetAlpha(int Alpha);

	// Setter only
	virtual void JWShape::SetColor(DWORD Color);

	// Getter
	virtual auto JWShape::GetPosition() const->D3DXVECTOR2;
	virtual auto JWShape::GetSize() const->D3DXVECTOR2;
	virtual auto JWShape::GetAlpha() const->int;
};