#include "JWDXDevice.h"

using namespace JW_GUI;

// Static member variable
LPDIRECT3D9 JWDXDevice::m_pD3D = nullptr;

auto JWDXDevice::Create(HWND hWnd, DWORD ClearColor)->Error
{
	if (nullptr == (m_hWnd = hWnd))
		return Error::NullHWND;

	if (m_pD3D == nullptr)
	{
		// To create D3D only once
		if (nullptr == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
			return Error::Direct3D9NotCraeted;
	}

	m_ClearColor = ClearColor;

	ZeroMemory(&m_D3DPP, sizeof(m_D3DPP));
	m_D3DPP.hDeviceWindow = m_hWnd;
	m_D3DPP.Windowed = TRUE;
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;

	m_ScreenSize.x = GetSystemMetrics(SM_CXSCREEN);
	m_ScreenSize.y = GetSystemMetrics(SM_CYSCREEN);

	m_D3DPP.BackBufferWidth = m_ScreenSize.x;
	m_D3DPP.BackBufferHeight = m_ScreenSize.y;

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_D3DPP, &m_pD3DDevice)))
	{
		return Error::DeviceNotCreated;
	}

	UpdateWindowSize();

	return Error::Ok;
}

void JWDXDevice::Destroy()
{
	JW_RELEASE(m_pD3D);
	JW_RELEASE(m_pD3DDevice);
}

void JWDXDevice::UpdateWindowSize()
{
	GetWindowRect(m_hWnd, &m_WindowRect);

	m_RenderRect.left = 0;
	m_RenderRect.top = 0;
	m_RenderRect.right = m_WindowRect.right - m_WindowRect.left;
	m_RenderRect.bottom = m_WindowRect.bottom - m_WindowRect.top;
}

auto JWDXDevice::CheckDevice()->HRESULT
{
	return m_pD3DDevice->TestCooperativeLevel();
}

auto JWDXDevice::ResetDevice()->HRESULT
{
	ZeroMemory(&m_D3DPP, sizeof(m_D3DPP));
	m_D3DPP.Windowed = TRUE;
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	m_D3DPP.hDeviceWindow = m_hWnd;
	m_D3DPP.BackBufferWidth = m_ScreenSize.x;
	m_D3DPP.BackBufferHeight = m_ScreenSize.y;

	return m_pD3DDevice->Reset(&m_D3DPP);
}

auto JWDXDevice::GetDevice()->LPDIRECT3DDEVICE9
{
	return m_pD3DDevice;
}

auto JWDXDevice::BeginRender()->HRESULT
{
	if (m_pD3DDevice)
	{
		m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.0f, 0);
		return m_pD3DDevice->BeginScene();
	}
	return E_FAIL;
}

auto JWDXDevice::EndRender()-> HRESULT
{
	if (m_pD3DDevice)
	{
		m_pD3DDevice->EndScene();

		return m_pD3DDevice->Present(&m_RenderRect, &m_RenderRect, m_hWnd, nullptr);
	}
	return E_FAIL;
}