/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CDirect3DEvents9.h
 *  PURPOSE:     Header file for Direct3D 9 events class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <d3d9.h>

class CDirect3DEvents9
{
public:
	static void    OnDirect3DDeviceCreate(IDirect3DDevice9* pDevice);
	static void    OnDirect3DDeviceDestroy(IDirect3DDevice9* pDevice);
	static void    OnPresent(IDirect3DDevice9* pDevice);
	static void    OnBeginScene(IDirect3DDevice9* pDevice);
	static bool    OnEndScene(IDirect3DDevice9* pDevice);
	static void    OnInvalidate(IDirect3DDevice9* pDevice);
	static void    OnRestore(IDirect3DDevice9* pDevice);
};
