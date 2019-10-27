/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CDirect3DHook9.cpp
 *  PURPOSE:     Function hooker for Direct3D 9
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "stdafx.h"
#include "detours/include/detours.h"

#include "CDirect3DHook9.h"
#include "CProxyDirect3D9.h"

static CDirect3DHook9* pThis = NULL;

CDirect3DHook9::CDirect3DHook9()
{
	DEBUG << "CDirect3DHook9::CDirect3DHook9" << std::endl;

	pThis = this;

	m_pfnDirect3DCreate9 = NULL;
	m_bDirect3DCreate9Suspended = false;
}

CDirect3DHook9::~CDirect3DHook9()
{
	DEBUG << "CDirect3DHook9::~CDirect3DHook9" << std::endl;

	m_pfnDirect3DCreate9 = NULL;
}

bool CDirect3DHook9::ApplyHook()
{
	// Hook Direct3DCreate9.
	if (!m_pfnDirect3DCreate9)
	{
		char szLibrary[] = "D3D9.DLL";
		char szFunction[] = "Direct3DCreate9";

		PBYTE pFunc = DetourFindFunction(szLibrary, szFunction);

		if (pFunc)
		{
			m_pfnDirect3DCreate9 = reinterpret_cast<pDirect3DCreate>(DetourFunction(pFunc, reinterpret_cast<PBYTE>(API_Direct3DCreate9)));
		}
		
		DEBUG << "Direct3D9 hook applied " << m_pfnDirect3DCreate9 << std::endl;

		if (!m_pfnDirect3DCreate9)
		{
			DEBUG << "There was a problem hooking Direct3DCreate9" << std::endl;
		}
	}
	else
	{
		DEBUG << "Direct3D9 hook resumed." << std::endl;
		m_bDirect3DCreate9Suspended = false;
	}

	return true;
}

bool CDirect3DHook9::RemoveHook()
{
	m_bDirect3DCreate9Suspended = true;
	DEBUG << "Direct3D9 hook suspended." << std::endl;
	return true;
}

IDirect3D9* CDirect3DHook9::API_Direct3DCreate9(UINT SDKVersion)
{
	// Get our self instance.
	assert(pThis && "API_Direct3DCreate9: No CDirect3DHook9");

	if (pThis->m_bDirect3DCreate9Suspended)
		return pThis->m_pfnDirect3DCreate9(SDKVersion);

	// Create our interface.
	DEBUG << "Calling Direct3DCreate9" << std::endl;
	IDirect3D9* pDirect3D9 = pThis->m_pfnDirect3DCreate9(SDKVersion);

	if (!pDirect3D9)
	{
		DEBUG << "Direct3DCreate9 failed" << std::endl;
		return NULL;
	}

	DEBUG << "Direct3DCreate9 succeded" << std::endl;

	// Create a proxy device.
	CProxyDirect3D9* pProxyDirect3D9 = new CProxyDirect3D9(pDirect3D9);

	return pProxyDirect3D9;
}
