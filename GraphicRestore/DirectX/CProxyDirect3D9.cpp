/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CProxyDirect3D9.cpp
 *  PURPOSE:     Direct3D 9 function hooking proxy
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "stdafx.h"
#include "CProxyDirect3D9.h"

std::vector<IDirect3D9*> ms_CreatedDirect3D9List;

HRESULT HandleCreateDeviceResult(HRESULT hResult, IDirect3D9* pDirect3D, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);

CProxyDirect3D9::CProxyDirect3D9(IDirect3D9* pInterface)
{
	DEBUG << "CProxyDirect3D9::CProxyDirect3D9 " << this << std::endl;
	m_pDevice = pInterface;
	ms_CreatedDirect3D9List.push_back(m_pDevice);
}

CProxyDirect3D9::~CProxyDirect3D9()
{
	DEBUG << "CProxyDirect3D9::~CProxyDirect3D9 " << this << std::endl;
	m_pDevice = NULL;

	std::vector<IDirect3D9*>::iterator it = ms_CreatedDirect3D9List.begin();

	while (it != ms_CreatedDirect3D9List.end())
	{
		if (m_pDevice == *it)
			it = ms_CreatedDirect3D9List.erase(it);
		else
			++it;
	}
}

/*** IUnknown methods ***/
HRESULT CProxyDirect3D9::QueryInterface(REFIID riid, void** ppvObj)
{
	return m_pDevice->QueryInterface(riid, ppvObj);
}

ULONG CProxyDirect3D9::AddRef(VOID)
{
	return m_pDevice->AddRef();
}

ULONG CProxyDirect3D9::Release(VOID)
{
	// Call original function
	ULONG ulRefCount = m_pDevice->Release();
	if (ulRefCount == 0)
		delete this;
	return ulRefCount;
}

/*** IDirect3D9 methods ***/
HRESULT CProxyDirect3D9::RegisterSoftwareDevice(void* pInitializeFunction)
{
	return m_pDevice->RegisterSoftwareDevice(pInitializeFunction);
}

UINT CProxyDirect3D9::GetAdapterCount(VOID)
{
	return m_pDevice->GetAdapterCount();
}

HRESULT CProxyDirect3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
{
	return m_pDevice->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT CProxyDirect3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
	return m_pDevice->GetAdapterModeCount(Adapter, Format);
}

HRESULT CProxyDirect3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
	return m_pDevice->EnumAdapterModes(Adapter, Format, Mode, pMode);
}

HRESULT CProxyDirect3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
{
	return m_pDevice->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT CProxyDirect3D9::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
	return m_pDevice->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

HRESULT CProxyDirect3D9::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType,
	D3DFORMAT CheckFormat)
{
	return m_pDevice->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT CProxyDirect3D9::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed,
	D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
	return m_pDevice->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

HRESULT CProxyDirect3D9::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat,
	D3DFORMAT DepthStencilFormat)
{
	return m_pDevice->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT CProxyDirect3D9::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
	return m_pDevice->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT CProxyDirect3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
	return m_pDevice->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR CProxyDirect3D9::GetAdapterMonitor(UINT Adapter)
{
	return m_pDevice->GetAdapterMonitor(Adapter);
}

HMONITOR CProxyDirect3D9::StaticGetAdapterMonitor(UINT Adapter)
{
	if (ms_CreatedDirect3D9List.empty())
		return NULL;
	return ms_CreatedDirect3D9List[0]->GetAdapterMonitor(Adapter);
}

IDirect3D9* CProxyDirect3D9::StaticGetDirect3D(void)
{
	if (ms_CreatedDirect3D9List.empty())
		return NULL;
	return ms_CreatedDirect3D9List[0];
}

HRESULT CProxyDirect3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	// Do not change the code at the start of this function.
	// Some graphic card drivers seem sensitive to the content of the CreateDevice function.
	HRESULT hResult;

	DEBUG << "CProxyDirect3D9::CreateDevice" << std::endl;

	DEBUG << "    Adapter:" << Adapter << "  DeviceType:" << DeviceType << "  BehaviorFlags:" << BehaviorFlags << std::endl;

	// Make sure DirectX Get calls will work
	BehaviorFlags &= ~D3DCREATE_PUREDEVICE;

	// Enable the auto depth stencil parameter
	pPresentationParameters->EnableAutoDepthStencil = true;

	// Create our object.
	hResult = m_pDevice->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	// Make sure the object was created successfully.
	if (hResult == D3D_OK)
	{
		*ppReturnedDeviceInterface = new CProxyDirect3DDevice9(*ppReturnedDeviceInterface);

		// Debug output
		D3DDEVICE_CREATION_PARAMETERS parameters;
		(*ppReturnedDeviceInterface)->GetCreationParameters(&parameters);

		DEBUG << "    Adapter:%d" << parameters.AdapterOrdinal << "  DeviceType:%d" << parameters.DeviceType << "  BehaviorFlags:0x%x" <<
			parameters.BehaviorFlags << std::endl;
	}

	return hResult;
}

////////////////////////////////////////////////
//
// CreateDeviceInsist
//
// Keep trying create device for a little bit
//
////////////////////////////////////////////////
HRESULT CreateDeviceInsist(uint32_t uiMinTries, uint32_t uiTimeout, IDirect3D9* pDirect3D, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	return pDirect3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
}

////////////////////////////////////////////////
//
// HandleCreateDeviceResult
//
// Log result and possibly fix everything
//
////////////////////////////////////////////////
HRESULT HandleCreateDeviceResult(HRESULT hResult, IDirect3D9* pDirect3D, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	// Log graphic card name
	D3DADAPTER_IDENTIFIER9 AdapterIdent;
	pDirect3D->GetAdapterIdentifier(Adapter, 0, &AdapterIdent);

	uint32_t uiCurrentStatus = 0;            //  0-unknown  1-fail  2-success after retry  3-success

	if (hResult == D3D_OK)
	{
		// Log success and creation parameters
		DEBUG << "CreateDevice success" << std::endl;
	}

	if (hResult != D3D_OK)
	{
		// Handle failure of initial create device call
		DEBUG << "CreateDevice failed #0: " << hResult << std::endl;

		// Handle fatal error
		DEBUG << "There was a problem starting GraphicFix\n\n" << "Direct3D CreateDevice error: " << hResult << std::endl;
	}

	return hResult;
}
