/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CProxyDirect3DDevice9.cpp
 *  PURPOSE:     Direct3D 9 device function hooking proxy
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "stdafx.h"
#include "CProxyDirect3DDevice9.h"
#include "CProxyDirect3D9.h"
#include "CDirect3DEvents9.h"

bool                                    g_bInGTAScene = false;
CProxyDirect3DDevice9* g_pProxyDevice = NULL;

// Proxy constructor and destructor.
CProxyDirect3DDevice9::CProxyDirect3DDevice9(IDirect3DDevice9* pDevice)
{
	DEBUG << "CProxyDirect3DDevice9::CProxyDirect3DDevice9 " << this << std::endl;

	// Set our wrapped device.
	m_pDevice = pDevice;

	g_pProxyDevice = this;

	//
	// Get video card installed memory
	//
	D3DDEVICE_CREATION_PARAMETERS creationParameters;
	m_pDevice->GetCreationParameters(&creationParameters);
	int iAdapter = creationParameters.AdapterOrdinal;

	IDirect3D9* pD3D9 = CProxyDirect3D9::StaticGetDirect3D();
	if (!pD3D9)
		m_pDevice->GetDirect3D(&pD3D9);

	D3DADAPTER_IDENTIFIER9 adaptIdent;
	ZeroMemory(&adaptIdent, sizeof(D3DADAPTER_IDENTIFIER9));
	pD3D9->GetAdapterIdentifier(iAdapter, 0, &adaptIdent);

	// Call event handler
	CDirect3DEvents9::OnDirect3DDeviceCreate(pDevice);
}

CProxyDirect3DDevice9::~CProxyDirect3DDevice9()
{
	DEBUG << "CProxyDirect3DDevice9::~CProxyDirect3DDevice9 " << this << std::endl;
	g_pProxyDevice = NULL;
}

/*** IUnknown methods ***/
HRESULT CProxyDirect3DDevice9::QueryInterface(REFIID riid, void** ppvObj)
{
	return m_pDevice->QueryInterface(riid, ppvObj);
}

ULONG CProxyDirect3DDevice9::AddRef(VOID)
{
	return m_pDevice->AddRef();
}

ULONG CProxyDirect3DDevice9::Release(VOID)
{
	// Check if will be final release
	m_pDevice->AddRef();
	ULONG ulRefCount = m_pDevice->Release();
	if (ulRefCount == 1)
	{
		DEBUG << "Releasing IDirect3DDevice9 Proxy..." << std::endl;
		// Call event handler
		CDirect3DEvents9::OnDirect3DDeviceDestroy(m_pDevice);
		delete this;
	}

	return m_pDevice->Release();
}

/*** IDirect3DDevice9 methods ***/
HRESULT CProxyDirect3DDevice9::TestCooperativeLevel(VOID)
{
	return m_pDevice->TestCooperativeLevel();
}

UINT CProxyDirect3DDevice9::GetAvailableTextureMem(VOID)
{
	return m_pDevice->GetAvailableTextureMem();
}

HRESULT CProxyDirect3DDevice9::EvictManagedResources(VOID)
{
	return m_pDevice->EvictManagedResources();
}

HRESULT CProxyDirect3DDevice9::GetDirect3D(IDirect3D9** ppD3D9)
{
	return m_pDevice->GetDirect3D(ppD3D9);
}

HRESULT CProxyDirect3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps)
{
	return m_pDevice->GetDeviceCaps(pCaps);
}

HRESULT CProxyDirect3DDevice9::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
	return m_pDevice->GetDisplayMode(iSwapChain, pMode);
}

HRESULT CProxyDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
	return m_pDevice->GetCreationParameters(pParameters);
}

HRESULT CProxyDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
	return m_pDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void CProxyDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags)
{
	m_pDevice->SetCursorPosition(X, Y, Flags);
}

BOOL CProxyDirect3DDevice9::ShowCursor(BOOL bShow)
{
	return m_pDevice->ShowCursor(bShow);
}

HRESULT CProxyDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
	return m_pDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT CProxyDirect3DDevice9::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
	return m_pDevice->GetSwapChain(iSwapChain, pSwapChain);
}

UINT CProxyDirect3DDevice9::GetNumberOfSwapChains(VOID)
{
	return m_pDevice->GetNumberOfSwapChains();
}

////////////////////////////////////////////////
//
// ResetDeviceInsist
//
// Keep trying reset device for a little bit
//
////////////////////////////////////////////////
HRESULT ResetDeviceInsist(uint32_t uiMinTries, uint32_t uiTimeout, IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return pDevice->Reset(pPresentationParameters);
}

////////////////////////////////////////////////
//
// DoResetDevice
//
// In various ways
//
////////////////////////////////////////////////
HRESULT DoResetDevice(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DPRESENT_PARAMETERS& presentationParametersOrig)
{
	HRESULT hResult;
	hResult = pDevice->Reset(pPresentationParameters);

	if (SUCCEEDED(hResult))
	{
		// Log success
		DEBUG << "Reset success" << std::endl;
	}
	else if (FAILED(hResult))
	{
		// Handle failure of initial reset device call
		DEBUG << "Reset failed #0: " << hResult << std::endl;

		// Try reset device again
		hResult = ResetDeviceInsist(5, 1000, pDevice, pPresentationParameters);

		// Handle retry result
		if (FAILED(hResult))
		{
			// Record problem
			DEBUG << "Direct3DDevice9::Reset  Fail1:" << hResult << std::endl;

			// Try with original presentation parameters
			*pPresentationParameters = presentationParametersOrig;
			hResult = ResetDeviceInsist(5, 1000, pDevice, pPresentationParameters);

			if (FAILED(hResult))
			{
				// Record problem
				DEBUG << "Direct3DDevice9::Reset  Fail2:" << hResult << std::endl;

				// Handle fatal error
				DEBUG << "There was a problem resetting Direct3D\n\n" << "Direct3DDevice9 Reset error: " << hResult << std::endl;
			}
		}
	}
	return hResult;
}

HRESULT CProxyDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	DEBUG << "CProxyDirect3DDevice9::Reset" << std::endl;

	// Save presentation parameters
	D3DPRESENT_PARAMETERS presentationParametersOrig = *pPresentationParameters;

	HRESULT hResult;

	// Call our event handler.
	CDirect3DEvents9::OnInvalidate(m_pDevice);

	// Call the real reset routine.
	hResult = DoResetDevice(m_pDevice, pPresentationParameters, presentationParametersOrig);

	// Call our event handler.
	CDirect3DEvents9::OnRestore(m_pDevice);

	return hResult;
}

HRESULT CProxyDirect3DDevice9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	CDirect3DEvents9::OnPresent(m_pDevice);

	return m_pDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT CProxyDirect3DDevice9::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	return m_pDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT CProxyDirect3DDevice9::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
	return m_pDevice->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT CProxyDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
{
	return m_pDevice->SetDialogBoxMode(bEnableDialogs);
}

VOID CProxyDirect3DDevice9::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	m_pDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
}

VOID CProxyDirect3DDevice9::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
	m_pDevice->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT CProxyDirect3DDevice9::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture,
	HANDLE* pSharedHandle)
{
	return m_pDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT CProxyDirect3DDevice9::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
	IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
	return m_pDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT CProxyDirect3DDevice9::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
	IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
	return m_pDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT CProxyDirect3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer,
	HANDLE* pSharedHandle)
{
	return m_pDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT CProxyDirect3DDevice9::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer,
	HANDLE* pSharedHandle)
{
	return m_pDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT CProxyDirect3DDevice9::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,
	BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return m_pDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT CProxyDirect3DDevice9::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,
	BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return m_pDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT CProxyDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface,
	CONST POINT* pDestPoint)
{
	return m_pDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT CProxyDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
	return m_pDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT CProxyDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
	return m_pDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT CProxyDirect3DDevice9::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	return m_pDevice->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT CProxyDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect,
	D3DTEXTUREFILTERTYPE Filter)
{
	return m_pDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT CProxyDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
	return m_pDevice->ColorFill(pSurface, pRect, color);
}

HRESULT CProxyDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface,
	HANDLE* pSharedHandle)
{
	return m_pDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT CProxyDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
	return m_pDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT CProxyDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
	return m_pDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT CProxyDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	return m_pDevice->SetDepthStencilSurface(pNewZStencil);
}

HRESULT CProxyDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	return m_pDevice->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT CProxyDirect3DDevice9::BeginScene(VOID)
{
	HRESULT hResult;

	// Call the real routine.
	hResult = m_pDevice->BeginScene();
	if (hResult == D3D_OK)
		g_bInGTAScene = true;

	// Call our event handler.
	CDirect3DEvents9::OnBeginScene(m_pDevice);

	// Possible fix for missing textures on some chipsets
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	return hResult;
}

HRESULT CProxyDirect3DDevice9::EndScene(VOID)
{
	// Call our event handler.
	if (CDirect3DEvents9::OnEndScene(m_pDevice))
	{
		// Call real routine.
		HRESULT hResult = m_pDevice->EndScene();
		g_bInGTAScene = false;

		return hResult;
	}

	return D3D_OK;
}

HRESULT CProxyDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return m_pDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT CProxyDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	// Call original
	return m_pDevice->SetTransform(State, pMatrix);
}

HRESULT CProxyDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	return m_pDevice->GetTransform(State, pMatrix);
}

HRESULT CProxyDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return m_pDevice->MultiplyTransform(State, pMatrix);
}

HRESULT CProxyDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	// Call original
	return m_pDevice->SetViewport(pViewport);
}

HRESULT CProxyDirect3DDevice9::GetViewport(D3DVIEWPORT9* pViewport)
{
	return m_pDevice->GetViewport(pViewport);
}

HRESULT CProxyDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	return m_pDevice->SetMaterial(pMaterial);
}

HRESULT CProxyDirect3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial)
{
	return m_pDevice->GetMaterial(pMaterial);
}

HRESULT CProxyDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
{
	return m_pDevice->SetLight(Index, pLight);
}

HRESULT CProxyDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9* pLight)
{
	return m_pDevice->GetLight(Index, pLight);
}

HRESULT CProxyDirect3DDevice9::LightEnable(DWORD Index, BOOL Enable)
{
	return m_pDevice->LightEnable(Index, Enable);
}

HRESULT CProxyDirect3DDevice9::GetLightEnable(DWORD Index, BOOL* pEnable)
{
	return m_pDevice->GetLightEnable(Index, pEnable);
}

HRESULT CProxyDirect3DDevice9::SetClipPlane(DWORD Index, CONST float* pPlane)
{
	return m_pDevice->SetClipPlane(Index, pPlane);
}

HRESULT CProxyDirect3DDevice9::GetClipPlane(DWORD Index, float* pPlane)
{
	return m_pDevice->GetClipPlane(Index, pPlane);
}

HRESULT CProxyDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	return m_pDevice->SetRenderState(State, Value);
}

HRESULT CProxyDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
	return m_pDevice->GetRenderState(State, pValue);
}

HRESULT CProxyDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
	return m_pDevice->CreateStateBlock(Type, ppSB);
}

HRESULT CProxyDirect3DDevice9::BeginStateBlock(VOID)
{
	return m_pDevice->BeginStateBlock();
}

HRESULT CProxyDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	return m_pDevice->EndStateBlock(ppSB);
}

HRESULT CProxyDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	return m_pDevice->SetClipStatus(pClipStatus);
}

HRESULT CProxyDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	return m_pDevice->GetClipStatus(pClipStatus);
}

HRESULT CProxyDirect3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
	return m_pDevice->GetTexture(Stage, ppTexture);
}

HRESULT CProxyDirect3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
	return m_pDevice->SetTexture(Stage, pTexture);
}

HRESULT CProxyDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	return m_pDevice->GetTextureStageState(Stage, Type, pValue);
}

HRESULT CProxyDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	return m_pDevice->SetTextureStageState(Stage, Type, Value);
}

HRESULT CProxyDirect3DDevice9::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
	return m_pDevice->GetSamplerState(Sampler, Type, pValue);
}

HRESULT CProxyDirect3DDevice9::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
	return m_pDevice->SetSamplerState(Sampler, Type, Value);
}

HRESULT CProxyDirect3DDevice9::ValidateDevice(DWORD* pNumPasses)
{
	return m_pDevice->ValidateDevice(pNumPasses);
}

HRESULT CProxyDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	return m_pDevice->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT CProxyDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	return m_pDevice->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT CProxyDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return m_pDevice->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT CProxyDirect3DDevice9::GetCurrentTexturePalette(UINT* PaletteNumber)
{
	return m_pDevice->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT CProxyDirect3DDevice9::SetScissorRect(CONST RECT* pRect)
{
	return m_pDevice->SetScissorRect(pRect);
}

HRESULT CProxyDirect3DDevice9::GetScissorRect(RECT* pRect)
{
	return m_pDevice->GetScissorRect(pRect);
}

HRESULT CProxyDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	return m_pDevice->SetSoftwareVertexProcessing(bSoftware);
}

BOOL CProxyDirect3DDevice9::GetSoftwareVertexProcessing(VOID)
{
	return m_pDevice->GetSoftwareVertexProcessing();
}

HRESULT CProxyDirect3DDevice9::SetNPatchMode(float nSegments)
{
	return m_pDevice->SetNPatchMode(nSegments);
}

FLOAT CProxyDirect3DDevice9::GetNPatchMode(VOID)
{
	return m_pDevice->GetNPatchMode();
}

HRESULT CProxyDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	return m_pDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT CProxyDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex,
	UINT primCount)
{
	return m_pDevice->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT CProxyDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData,
	UINT VertexStreamZeroStride)
{
	return m_pDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT CProxyDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount,
	CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData,
	UINT VertexStreamZeroStride)
{
	return m_pDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData,
		VertexStreamZeroStride);
}

HRESULT CProxyDirect3DDevice9::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer,
	IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
	return m_pDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT CProxyDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT CProxyDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	return m_pDevice->SetVertexDeclaration(pDecl);
}

HRESULT CProxyDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pDevice->GetVertexDeclaration(ppDecl);
}

HRESULT CProxyDirect3DDevice9::SetFVF(DWORD FVF)
{
	return m_pDevice->SetFVF(FVF);
}

HRESULT CProxyDirect3DDevice9::GetFVF(DWORD* pFVF)
{
	return m_pDevice->GetFVF(pFVF);
}

HRESULT CProxyDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
	return m_pDevice->CreateVertexShader(pFunction, ppShader);
}

HRESULT CProxyDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	return m_pDevice->SetVertexShader(pShader);
}

HRESULT CProxyDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	return m_pDevice->GetVertexShader(ppShader);
}

HRESULT CProxyDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return m_pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT CProxyDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return m_pDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT CProxyDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return m_pDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT CProxyDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return m_pDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT CProxyDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
	return m_pDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT CProxyDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return m_pDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT CProxyDirect3DDevice9::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	return m_pDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT CProxyDirect3DDevice9::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
	return m_pDevice->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT CProxyDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber, UINT Setting)
{
	return m_pDevice->SetStreamSourceFreq(StreamNumber, Setting);
}

HRESULT CProxyDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting)
{
	return m_pDevice->GetStreamSourceFreq(StreamNumber, pSetting);
}

HRESULT CProxyDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	return m_pDevice->SetIndices(pIndexData);
}

HRESULT CProxyDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	return m_pDevice->GetIndices(ppIndexData);
}

HRESULT CProxyDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
	return m_pDevice->CreatePixelShader(pFunction, ppShader);
}

HRESULT CProxyDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	return m_pDevice->SetPixelShader(pShader);
}

HRESULT CProxyDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	return m_pDevice->GetPixelShader(ppShader);
}

HRESULT CProxyDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return m_pDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT CProxyDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return m_pDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT CProxyDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return m_pDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT CProxyDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return m_pDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT CProxyDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
	return m_pDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT CProxyDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return m_pDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT CProxyDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return m_pDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT CProxyDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return m_pDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT CProxyDirect3DDevice9::DeletePatch(UINT Handle)
{
	return m_pDevice->DeletePatch(Handle);
}

HRESULT CProxyDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	return m_pDevice->CreateQuery(Type, ppQuery);
}
