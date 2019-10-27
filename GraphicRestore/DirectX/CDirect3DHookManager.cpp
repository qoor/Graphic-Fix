/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CDirect3DHookManager.cpp
 *  PURPOSE:     Direct3D version independant hook manager
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "stdafx.h"
#include "CDirect3DHookManager.h"

CDirect3DHookManager::CDirect3DHookManager()
{
	DEBUG << "CDirect3DHookManager::CDirect3DHookManager" << std::endl;

	// Set our defaults.
	m_pDirect3DHook9 = NULL;
}

CDirect3DHookManager::~CDirect3DHookManager()
{
	DEBUG << "CDirect3DHookManager::~CDirect3DHookManager" << std::endl;

	if (m_pDirect3DHook9)
	{
		m_pDirect3DHook9->RemoveHook();
		delete m_pDirect3DHook9;
	}
}

void CDirect3DHookManager::ApplyHook()
{
	if (m_pDirect3DHook9)
	{
		DEBUG << "CDirect3DHookManager::ApplyHook - Ignoring second call" << std::endl;
		return;
	}

	DEBUG << "CDirect3DHookManager::ApplyHook" << std::endl;

	PBYTE pbDirect3D9;

	// First load the module(s).
	pbDirect3D9 = reinterpret_cast<PBYTE>(LoadLibraryA("D3D9.DLL"));

	if (pbDirect3D9 != NULL)
	{
		m_pDirect3DHook9 = new CDirect3DHook9();
		m_pDirect3DHook9->ApplyHook();
		// Self-maintaining code.  Will delete on shutdown...
	}
}

void CDirect3DHookManager::RemoveHook()
{
	// Make sure we have a valid pointer.
	if (m_pDirect3DHook9 != NULL)
	{
		// Remove the hook.
		m_pDirect3DHook9->RemoveHook();
	}
}
