#pragma once

#include <iostream>
#include <Windows.h>
#include <d3dx9.h>
#include <cassert>
#include <vector>
#include <memory>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

using WSTRING = std::wstring;
using FLAG = unsigned char;

static const DWORD D3DFVF_ALL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
static const DWORD D3DFVF_NO_TEXTURE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
static const wchar_t ASSET_DIR[] = L"\\Asset\\";
static const int MAX_PATH_LEN = 260;

static const DWORD s_DefaultPlainColor = D3DCOLOR_XRGB(80, 80, 80);
static const DWORD s_DefaultDarkColor = D3DCOLOR_XRGB(40, 40, 40);
static const DWORD s_DefaultHighlightColor = D3DCOLOR_XRGB(40, 80, 255);
static const DWORD s_DefualtFontColor = D3DCOLOR_XRGB(255, 255, 255);
static const DWORD s_DefualtLabelColor = D3DCOLOR_ARGB(255, 120, 120, 120);
static const DWORD s_TransparentColor = D3DCOLOR_ARGB(0, 255, 255, 255);
static const DWORD s_SystemMarkColor = D3DCOLOR_XRGB(255, 255, 255);
static const DWORD s_ActiveBordorColor = D3DCOLOR_XRGB(0, 150, 255);

static const int MIN_WINDOW_WIDTH = 100;
static const int MIN_WINDOW_HEIGHT = 60;

#define VECTOR std::vector
#define UNIQUE_PTR std::unique_ptr
#define SHARED_PTR std::shared_ptr
#define MAKE_UNIQUE(T) std::unique_ptr<T>(new T);
#define MAKE_SHARED(T) std::shared_ptr<T>(new T);

#define JW_OK(func) (func == JWERROR::Ok)
#define JW_FAILED(func) (func != JWERROR::Ok)
#define JW_RELEASE(dx) if (dx) { dx->Release(); dx = nullptr; };
#define JW_DELETE(var) if (var) { delete var; var = nullptr; };

class JWCommon
{
protected:
	static wchar_t ms_AppDir[MAX_PATH_LEN];

public:
	JWCommon() {};
	virtual ~JWCommon() {};

	void JWCommon::SetApplicationDir();
};

enum class JWERROR
{
	// Succeeded
	Ok,
	// Window, DirectX creation failure
	WindowNotCreated,
	DirectXNotCreated,
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
	// Null pointer
	NullDevice,
	NullFont,
	// Input is invalid
	InvalidType,
};

struct JW_INT2
{
	int x;
	int y;

	JW_INT2() : x(0), y(0) {};
	JW_INT2(int _x, int _y) : x(_x), y(_y) {};
	JW_INT2(POINT point) : x(point.x), y(point.y) {};

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

	JW_INT2 operator-(JW_INT2& Value)
	{
		JW_INT2 Result = *this;

		Result.x -= Value.x;
		Result.y -= Value.y;

		return Result;
	}

	JW_INT2& operator-=(JW_INT2& Value)
	{
		JW_INT2& Result = *this;

		Result.x -= Value.x;
		Result.y -= Value.y;

		return Result;
	}

	JW_INT2 operator+(JW_INT2& Value)
	{
		JW_INT2 Result = *this;

		Result.x += Value.x;
		Result.y += Value.y;

		return Result;
	}

	JW_INT2& operator+=(JW_INT2& Value)
	{
		JW_INT2& Result = *this;

		Result.x += Value.x;
		Result.y += Value.y;

		return Result;
	}
};

enum class FontID
{
	Main,
};

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