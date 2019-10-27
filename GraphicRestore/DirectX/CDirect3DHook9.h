#pragma once

typedef IDirect3D9* (__stdcall* pDirect3DCreate)(int SDKVersion);

class CDirect3DHook9
{
public:
	CDirect3DHook9();
	~CDirect3DHook9();
	// Hook routines.
	static IDirect3D9* __stdcall API_Direct3DCreate9(UINT SDKVersion);

	// Hook utilities.
	bool ApplyHook();
	bool RemoveHook();

	pDirect3DCreate m_pfnDirect3DCreate9;
	bool            m_bDirect3DCreate9Suspended;
};
