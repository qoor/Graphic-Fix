#include "stdafx.h"

#include "CDirect3DEvents9.h"

bool g_bInMTAScene = false;

void CDirect3DEvents9::OnDirect3DDeviceCreate(IDirect3DDevice9* pDevice)
{
	DEBUG << "CDirect3DEvents9::OnDirect3DDeviceCreate" << std::endl;
}

void CDirect3DEvents9::OnDirect3DDeviceDestroy(IDirect3DDevice9* pDevice)
{
	DEBUG << "CDirect3DEvents9::OnDirect3DDeviceDestroy" << std::endl;
}

void CDirect3DEvents9::OnBeginScene(IDirect3DDevice9* pDevice)
{
	// Notify core
	gr->DoPreFramePulse();
}

bool CDirect3DEvents9::OnEndScene(IDirect3DDevice9* pDevice)
{
	return true;
}

void CDirect3DEvents9::OnInvalidate(IDirect3DDevice9* pDevice)
{
	DEBUG << "CDirect3DEvents9::OnInvalidate" << std::endl;
}

void CDirect3DEvents9::OnRestore(IDirect3DDevice9* pDevice)
{
	DEBUG << "CDirect3DEvents9::OnRestore" << std::endl;

	gr->OnDeviceRestore();
}

void CDirect3DEvents9::OnPresent(IDirect3DDevice9* pDevice)
{
	// Start a new scene. This isn't ideal and is not really recommended by MSDN.
	// I tried disabling EndScene from GTA and just end it after this code ourselves
	// before present, but that caused graphical issues randomly with the sky.
	if (pDevice->BeginScene() == D3D_OK)
		g_bInMTAScene = true;

	// Reset samplers on first call
	static bool bDoneReset = false;
	if (!bDoneReset)
	{
		bDoneReset = true;
		for (uint i = 0; i < 16; i++)
		{
			pDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			pDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}
	}

	gr->DoPostFramePulse();

	// End the scene that we started.
	pDevice->EndScene();
	g_bInMTAScene = false;
}
