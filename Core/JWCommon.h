#pragma once

#include <iostream>
#include <Windows.h>
#include <d3dx9.h>
#include <cassert>
#include <vector>
#include <memory>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

#ifndef CONVENIENT_STD
#define CONVENIENT_STD
	using STRING = std::string;
	using WSTRING = std::wstring;

	template <typename T>
	using VECTOR = std::vector<T>;

	template <typename T>
	using UNIQUE_PTR = std::unique_ptr<T>;

	template <typename T>
	using SHARED_PTR = std::shared_ptr<T>;

	#define MAKE_UNIQUE(T) std::make_unique<T>
	#define MAKE_SHARED(T) std::make_shared<T>
#endif

#define JW_OK(func) (func == Error::Ok)
#define JW_FAILED(func) (func != Error::Ok)
#define JW_RELEASE(dx) if (dx) { dx->Release(); dx = nullptr; };
#define JW_DELETE(var) if (var) { delete var; var = nullptr; };

namespace JW_GUI
{
	using DWORD = unsigned long;

	static const UINT MAX_PATH_LEN = 260;
	static const wchar_t ASSET_DIR[] = L"Asset\\";
	static const wchar_t FONT_NAME[] = L"megt20all.fnt";
	static const UINT MAX_TEXT_LEN = 20;

	static const UINT MIN_WINDOW_WIDTH = 100;
	static const UINT MIN_WINDOW_HEIGHT = 60;

	static const DWORD D3DFVF_ALL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static const DWORD D3DFVF_NO_TEXTURE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	static const DWORD JWCOLOR_PLAIN = D3DCOLOR_XRGB(80, 80, 80);
	static const DWORD JWCOLOR_DARK = D3DCOLOR_XRGB(40, 40, 40);
	static const DWORD JWCOLOR_HIGHLIGHT = D3DCOLOR_XRGB(40, 80, 255);
	static const DWORD JWCOLOR_FONT = D3DCOLOR_XRGB(255, 255, 255);
	static const DWORD JWCOLOR_LABEL = D3DCOLOR_ARGB(255, 120, 120, 120);
	static const DWORD JWCOLOR_TRANSPARENT = D3DCOLOR_ARGB(0, 255, 255, 255);
	static const DWORD JWCOLOR_SYSTEM_MARK = D3DCOLOR_XRGB(255, 255, 255);
	static const DWORD JWCOLOR_BORDER_ACTIVE = D3DCOLOR_XRGB(0, 150, 255);

	using FLAG = unsigned char;

	enum class ALIGNMENT_HORZ
	{
		Left,
		Center,
		Right,
	};

	enum class ALIGNMENT_VERT
	{
		Top,
		Center,
		Bottom,
	};

	enum class Error
	{
		// Succeeded
		Ok,
		// Window, DirectX creation failure
		WindowNotCreated,
		DirectXNotCreated,
		TextureNotCreated,
		VertexBufferNotCreated,
		IndexBufferNotCreated,
		// Core class instance creation failure
		FontNotCreated,
		ShapeNotCreated,
		LineNotCreated,
		WinBaseNotCreated,
		DirectInputNotCreated,
		// Control class creation failure
		BorderNotCreated,
		ControlManagerNotCreated,
		ControlNotCreated, // @warning: it refers to JWControl class instance
		LabelNotCreated,
		SystemButtonNotCreated,
		CommandButtonNotCreated,
		ImageNotCreated,
		// GUIWindow class creation failure
		TitlebarNotcreated,
		// Lock
		VertexBufferNotLocked,
		IndexBufferNotLocked,
		// Null pointer
		NullDevice,
		NullVertex,
		NullIndex,
		// Input is invalid
		InvalidType,
		NotEnoughBuffer,
	};

	struct Int2
	{
		int x;
		int y;

		Int2() : x(0), y(0) {};
		Int2(int _x, int _y) : x(_x), y(_y) {};
		Int2(POINT point) : x(point.x), y(point.y) {};

		operator POINT()
		{
			POINT Result;

			Result.x = this->x;
			Result.y = this->y;

			return Result;
		}

		operator D3DXVECTOR2()
		{
			D3DXVECTOR2 Result;

			Result.x = static_cast<float>(this->x);
			Result.y = static_cast<float>(this->y);

			return Result;
		}

		Int2 operator-(Int2& Value)
		{
			Int2 Result = *this;

			Result.x -= Value.x;
			Result.y -= Value.y;

			return Result;
		}

		Int2& operator-=(Int2& Value)
		{
			Int2& Result = *this;

			Result.x -= Value.x;
			Result.y -= Value.y;

			return Result;
		}

		Int2 operator+(Int2& Value)
		{
			Int2 Result = *this;

			Result.x += Value.x;
			Result.y += Value.y;

			return Result;
		}

		Int2& operator+=(Int2& Value)
		{
			Int2& Result = *this;

			Result.x += Value.x;
			Result.y += Value.y;

			return Result;
		}
	};

	struct VertexShape
	{
		FLOAT x, y, z, rhw;
		DWORD color;
		FLOAT u, v;

		VertexShape() :
			x(0), y(0), z(0), rhw(1), color(0xFFFFFFFF), u(0), v(0) {};
		VertexShape(float _x, float _y) :
			x(_x), y(_y), z(0), rhw(1), color(0xFFFFFFFF), u(0), v(0) {};
		VertexShape(float _x, float _y, DWORD _color) :
			x(_x), y(_y), z(0), rhw(1), color(_color), u(0), v(0) {};
		VertexShape(float _x, float _y, float _u, float _v) :
			x(_x), y(_y), z(0), rhw(1), color(0xFFFFFFFF), u(_u), v(_v) {};
		VertexShape(float _x, float _y, float _u, float _v, DWORD _color) :
			x(_x), y(_y), z(0), rhw(1), color(_color), u(_u), v(_v) {};
		VertexShape(float _x, float _y, float _z, float _rhw, DWORD _color, float _u, float _v) :
			x(_x), y(_y), z(_z), rhw(_rhw), color(_color), u(_u), v(_v) {};
	};

	struct VertexLine
	{
		FLOAT x, y, z, rhw;
		DWORD color;

		VertexLine() : x(0), y(0), z(0), rhw(1), color(0xffffffff) {};
		VertexLine(float _x, float _y, DWORD _color) : x(_x), y(_y), z(0), rhw(1), color(_color) {};
		VertexLine(float _x, float _y, float _z, float _rhw, DWORD _color) :
			x(_x), y(_y), z(_z), rhw(_rhw), color(_color) {};
	};

	struct Index3
	{
		WORD _0, _1, _2;

		Index3() : _0(0), _1(0), _2(0) {};
		Index3(int ID0, int ID1, int ID2) : _0(ID0), _1(ID1), _2(ID2) {};
	};

	struct Index2
	{
		WORD _0, _1;

		Index2() : _0(0), _1(0) {};
		Index2(int ID0, int ID1) : _0(ID0), _1(ID1) {};
	};
};