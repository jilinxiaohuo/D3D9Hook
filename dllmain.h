#pragma once

LPVOID HookVirtualTable(
	LPVOID instance,
	UINT index,
	LPVOID newaddr);


LPVOID HookDeviceVirtualTable(
	LPVOID instance,
	UINT index,
	LPVOID newaddr);

HRESULT WINAPI MyCreateDevice(
	IDirect3D9*			  Direct3D9,
	UINT                  Adapter,
	D3DDEVTYPE            DeviceType,
	HWND                  hFocusWindow,
	DWORD                 BehaviorFlags,
	D3DPRESENT_PARAMETERS *pPresentationParameters,
	IDirect3DDevice9      **ppReturnedDeviceInterface
	);

HRESULT WINAPI MyClear(
	IDirect3DDevice9	*pDevice,
	DWORD				Count,
	const D3DRECT		*pRects,
	DWORD				Flags,
	D3DCOLOR			Color,
	float				Z,
	DWORD				Stencil
	);

HRESULT WINAPI MyDrawIndexedPrimitive(
	IDirect3DDevice9*	pDevice,
	D3DPRIMITIVETYPE	Type,
	INT					BaseVertexIndex,
	UINT				MinIndex,
	UINT				NumVertices,
	UINT				StartIndex,
	UINT				PrimitiveCount
	);

HRESULT WINAPI MyDrawIndexedPrimitiveUP(
	IDirect3DDevice9	*pDevice,
	D3DPRIMITIVETYPE	PrimitiveType,
	UINT				MinVertexIndex,
	UINT				NumVertices,
	UINT				PrimitiveCount,
	const void			*pIndexData,
	D3DFORMAT			IndexDataFormat,
	const void			*pVertexStreamZeroData,
	UINT				VertexStreamZeroStride
	);

HRESULT WINAPI MyDrawPrimitive(
	IDirect3DDevice9	*pDevice,
	D3DPRIMITIVETYPE	PrimitiveType,
	UINT				StartVertex,
	UINT				PrimitiveCount
	);

HRESULT WINAPI MyDrawPrimitiveUP(
	IDirect3DDevice9	*pDevice,
	D3DPRIMITIVETYPE	PrimitiveType,
	UINT				PrimitiveCount,
	const void			*pVertexStreamZeroData,
	UINT				VertexStreamZeroStride
	);

HRESULT WINAPI MyDrawRectPatch(
	IDirect3DDevice9	*pDevice,
	UINT				Handle,
	const float			*pNumSegs,
	const D3DRECTPATCH_INFO *pRectPatchInfo
	);

HRESULT WINAPI MyDrawTriPatch(
	IDirect3DDevice9	*pDevice,
	UINT				Handle,
	const float			*pNumSegs,
	const D3DTRIPATCH_INFO *pTriPatchInfo
	);

HRESULT WINAPI MyPresent(
	IDirect3DDevice9* pDevice,
	const RECT    *pSourceRect,
	const RECT    *pDestRect,
	HWND    hDestWindowOverride,
	const RGNDATA *pDirtyRegion
	);

using TCreateDevice = decltype(&MyCreateDevice);
using TClear = decltype(&MyClear);
using TDrawIndexedPrimitive = decltype(&MyDrawIndexedPrimitive);
using TDrawIndexedPromitiveUP = decltype(&MyDrawIndexedPrimitiveUP);
using TDrawPrimitive = decltype(&MyDrawPrimitive);
using TDrawPrimitiveUP = decltype(&MyDrawPrimitiveUP);
using TDrawRectPatch = decltype(&MyDrawRectPatch);
using TDrawTriPatch = decltype(&MyDrawTriPatch);
using TPresent = decltype(&MyPresent);

/*
0327f03c  01d76f19 d3d9_old!CBaseDevice::QueryInterface
0327f040  01d76992 d3d9_old!CBaseDevice::AddRef
0327f044  01d76969 d3d9_old!CBaseDevice::Release
0327f048  01d92587 d3d9_old!CBaseDevice::TestCooperativeLevel
0327f04c  01e2c6ad d3d9_old!CBaseDevice::GetAvailableTextureMem
0327f050  01e69c8f d3d9_old!CD3DBase::EvictManagedResources
0327f054  01d8b1c5 d3d9_old!CBaseDevice::GetDirect3D
0327f058  01d76ff8 d3d9_old!CBaseDevice::GetDeviceCaps
0327f05c  01dad144 d3d9_old!CBaseDevice::GetDisplayMode
0327f060  01d9084e d3d9_old!CBaseDevice::GetCreationParameters
0327f064  01e2bb74 d3d9_old!CBaseDevice::SetCursorProperties
0327f068  01e2c04d d3d9_old!CBaseDevice::SetCursorPosition
0327f06c  01dadef0 d3d9_old!CBaseDevice::ShowCursor
0327f070  01d8e9a0 d3d9_old!CBaseDevice::CreateAdditionalSwapChain
0327f074  01d89ac7 d3d9_old!CBaseDevice::GetSwapChain
0327f078  01db93ca d3d9_old!CBaseDevice::GetNumberOfSwapChains
0327f07c  01dcf251 d3d9_old!CBaseDevice::Reset
0327f080  01dba064 d3d9_old!CBaseDevice::Present
0327f084  01dd1418 d3d9_old!CBaseDevice::GetBackBuffer
0327f088  01d8bfe9 d3d9_old!CBaseDevice::GetRasterStatus
0327f08c  01e2c139 d3d9_old!CBaseDevice::SetDialogBoxMode
0327f090  01e2c3bf d3d9_old!CBaseDevice::SetGammaRamp
0327f094  01e2c4fd d3d9_old!CBaseDevice::GetGammaRamp
0327f098  01da5ddb d3d9_old!CBaseDevice::CreateTexture
0327f09c  01e2ca68 d3d9_old!CBaseDevice::CreateVolumeTexture
0327f0a0  01dccb2d d3d9_old!CBaseDevice::CreateCubeTexture
0327f0a4  01d92d69 d3d9_old!CBaseDevice::CreateVertexBuffer
0327f0a8  01d932e6 d3d9_old!CBaseDevice::CreateIndexBuffer
0327f0ac  01dc0127 d3d9_old!CBaseDevice::CreateRenderTarget
0327f0b0  01e2cd88 d3d9_old!CBaseDevice::CreateDepthStencilSurface
0327f0b4  01e2e0e0 d3d9_old!CBaseDevice::UpdateSurface
0327f0b8  01da46ab d3d9_old!CBaseDevice::UpdateTexture
0327f0bc  01dbf3bd d3d9_old!CBaseDevice::GetRenderTargetData
0327f0c0  01e2d037 d3d9_old!CBaseDevice::GetFrontBufferData
0327f0c4  01dae108 d3d9_old!CBaseDevice::StretchRect
0327f0c8  01d8d507 d3d9_old!CBaseDevice::ColorFill
0327f0cc  01dbffee d3d9_old!CBaseDevice::CreateOffscreenPlainSurface
0327f0d0  01db9e53 d3d9_old!CD3DBase::SetRenderTarget
0327f0d4  01dba13c d3d9_old!CD3DBase::GetRenderTarget
0327f0d8  01dc108f d3d9_old!CD3DBase::SetDepthStencilSurface
0327f0dc  01dc0f08 d3d9_old!CD3DBase::GetDepthStencilSurface
0327f0e0  01d93889 d3d9_old!CD3DBase::BeginScene
0327f0e4  01d9279f d3d9_old!CD3DBase::EndScene
0327f0e8  01d7d821 d3d9_old!CD3DBase::Clear
0327f0ec  01d7cfde d3d9_old!CD3DBase::SetTransform
0327f0f0  01e70dda d3d9_old!CD3DHal::GetTransform
0327f0f4  01e69ddf d3d9_old!CD3DBase::MultiplyTransform
0327f0f8  01e69ef7 d3d9_old!CD3DBase::SetViewport
0327f0fc  01dbebc1 d3d9_old!CD3DBase::GetViewport
0327f100  01d7fdbe d3d9_old!CD3DHal::SetMaterial_FP
0327f104  01e71753 d3d9_old!CD3DHal::GetMaterial
0327f108  01da3788 d3d9_old!CD3DBase::SetLight
0327f10c  01e71802 d3d9_old!CD3DHal::GetLight
0327f110  01da388b d3d9_old!CD3DBase::LightEnable
0327f114  01e7194b d3d9_old!CD3DHal::GetLightEnable
0327f118  01e6b5c4 d3d9_old!CD3DBase::SetClipPlane
0327f11c  01e7145f d3d9_old!CD3DHal::GetClipPlane
0327f120  01d990b5 d3d9_old!CD3DHal::SetRenderState_FP
0327f124  01e7099c d3d9_old!CD3DHal::GetRenderState
0327f128  01dcd5ec d3d9_old!CD3DBase::CreateStateBlock
0327f12c  01dba418 d3d9_old!CD3DBase::BeginStateBlock
0327f130  01dbc093 d3d9_old!CD3DBase::EndStateBlock
0327f134  01e70f6e d3d9_old!CD3DHal::SetClipStatus
0327f138  01e71018 d3d9_old!CD3DHal::GetClipStatus
0327f13c  01e6a8d8 d3d9_old!CD3DBase::GetTexture
0327f140  01d7d318 d3d9_old!CD3DHal::SetTexture_FP
0327f144  01e70b84 d3d9_old!CD3DHal::GetTextureStageState
0327f148  01d7d4bc d3d9_old!CD3DHal::SetTextureStageState_FP
0327f14c  01e70d02 d3d9_old!CD3DHal::GetSamplerState
0327f150  01d7d5c6 d3d9_old!CD3DHal::SetSamplerState_FP
0327f154  01dc6544 d3d9_old!CD3DHal::ValidateDevice
0327f158  01e6b1c7 d3d9_old!CD3DBase::SetPaletteEntries
0327f15c  01e6b501 d3d9_old!CD3DBase::GetPaletteEntries
0327f160  01e6afca d3d9_old!CD3DBase::SetCurrentTexturePalette
0327f164  01e6b126 d3d9_old!CD3DBase::GetCurrentTexturePalette
0327f168  01e6a021 d3d9_old!CD3DBase::SetScissorRect
0327f16c  01e6a148 d3d9_old!CD3DBase::GetScissorRect
0327f170  01e71526 d3d9_old!CD3DHal::SetSoftwareVertexProcessing
0327f174  01e7097f d3d9_old!CD3DHal::GetSoftwareVertexProcessing
0327f178  01dbef2f d3d9_old!CD3DBase::SetNPatchMode
0327f17c  01dbebaa d3d9_old!CD3DBase::GetNPatchMode
0327f180  01d98de0 d3d9_old!CD3DBase::DrawPrimitive
0327f184  01d9b6b1 d3d9_old!CD3DBase::DrawIndexedPrimitive
0327f188  01dbe894 d3d9_old!CD3DBase::DrawPrimitiveUP
0327f18c  01e6ca36 d3d9_old!CD3DBase::DrawIndexedPrimitiveUP
0327f190  01e725e4 d3d9_old!CD3DHal::ProcessVertices
0327f194  01d9d3ef d3d9_old!CD3DBase::CreateVertexDeclaration
0327f198  01d9d313 d3d9_old!CD3DBase::SetVertexDeclaration
0327f19c  01e67cd5 d3d9_old!CD3DBase::GetVertexDeclaration
0327f1a0  01d99261 d3d9_old!CD3DHal::SetFVF_FP
0327f1a4  01e6bc10 d3d9_old!CD3DBase::GetFVF
0327f1a8  01db5e6f d3d9_old!CD3DBase::CreateVertexShader
0327f1ac  01d9d513 d3d9_old!CD3DHal::SetVertexShader_FP
0327f1b0  01e74994 d3d9_old!CD3DHal::GetVertexShader
0327f1b4  01db33f9 d3d9_old!CD3DBase::SetVertexShaderConstantF_FP
0327f1b8  01e69346 d3d9_old!CD3DBase::GetVertexShaderConstantF
0327f1bc  01e6d09c d3d9_old!CD3DBase::SetVertexShaderConstantI_FP
0327f1c0  01e69533 d3d9_old!CD3DBase::GetVertexShaderConstantI
0327f1c4  01e6d23f d3d9_old!CD3DBase::SetVertexShaderConstantB_FP
0327f1c8  01e69723 d3d9_old!CD3DBase::GetVertexShaderConstantB
0327f1cc  01d93c81 d3d9_old!CD3DBase::SetStreamSource_FP
0327f1d0  01e6b71a d3d9_old!CD3DBase::GetStreamSource
0327f1d4  01e67a3f d3d9_old!CD3DBase::SetStreamSourceFreq_FP
0327f1d8  01e6b951 d3d9_old!CD3DBase::GetStreamSourceFreq
0327f1dc  01d9b900 d3d9_old!CD3DBase::SetIndices_FP
0327f1e0  01e6ba0c d3d9_old!CD3DBase::GetIndices
0327f1e4  01dafd6f d3d9_old!CD3DBase::CreatePixelShader
0327f1e8  01dd0f6d d3d9_old!CD3DHal::SetPixelShader_FP
0327f1ec  01e74d89 d3d9_old!CD3DHal::GetPixelShader
0327f1f0  01db67ea d3d9_old!CD3DBase::SetPixelShaderConstantF_FP
0327f1f4  01e74e52 d3d9_old!CD3DHal::GetPixelShaderConstantF
0327f1f8  01e6d3d9 d3d9_old!CD3DBase::SetPixelShaderConstantI_FP
0327f1fc  01e74f94 d3d9_old!CD3DHal::GetPixelShaderConstantI
0327f200  01e6d58b d3d9_old!CD3DBase::SetPixelShaderConstantB_FP
0327f204  01e750e4 d3d9_old!CD3DHal::GetPixelShaderConstantB
0327f208  01e6c4e8 d3d9_old!CD3DBase::DrawRectPatch
0327f20c  01e6c78f d3d9_old!CD3DBase::DrawTriPatch
0327f210  01e68498 d3d9_old!CD3DBase::DeletePatch
0327f214  01d94019 d3d9_old!CD3DBase::CreateQuery
0327f218  01e6ab61 d3d9_old!CD3DBase::SetConvolutionMonoKernel
0327f21c  01e6ae38 d3d9_old!CD3DBase::ComposeRects
0327f220  01e2dfc4 d3d9_old!CBaseDevice::PresentEx
0327f224  01e2ddb5 d3d9_old!CBaseDevice::GetGPUThreadPriority
0327f228  01da503c d3d9_old!CBaseDevice::SetGPUThreadPriority
0327f22c  01e2d865 d3d9_old!CBaseDevice::WaitForVBlank
0327f230  01e2d96c d3d9_old!CBaseDevice::CheckResourceResidency
0327f234  01d90583 d3d9_old!CBaseDevice::SetMaximumFrameLatency
0327f238  01e2dea3 d3d9_old!CBaseDevice::GetMaximumFrameLatency
0327f23c  01d8bab5 d3d9_old!CBaseDevice::CheckDeviceState
0327f240  01e2cbfe d3d9_old!CBaseDevice::CreateRenderTargetEx
0327f244  01e2e061 d3d9_old!CBaseDevice::CreateOffscreenPlainSurfaceEx
0327f248  01e2cea4 d3d9_old!CBaseDevice::CreateDepthStencilSurfaceEx
0327f24c  01e2df35 d3d9_old!CBaseDevice::ResetEx
0327f250  01d8d6d1 d3d9_old!CBaseDevice::GetDisplayModeEx
0327f254  01d990b5 d3d9_old!CD3DHal::SetRenderState_FP
0327f258  01d7d4bc d3d9_old!CD3DHal::SetTextureStageState_FP
0327f25c  01d7d5c6 d3d9_old!CD3DHal::SetSamplerState_FP
0327f260  01d7fdbe d3d9_old!CD3DHal::SetMaterial_FP
0327f264  01d9d513 d3d9_old!CD3DHal::SetVertexShader_FP
0327f268  01db33f9 d3d9_old!CD3DBase::SetVertexShaderConstantF_FP
0327f26c  01e6d09c d3d9_old!CD3DBase::SetVertexShaderConstantI_FP
0327f270  01e6d23f d3d9_old!CD3DBase::SetVertexShaderConstantB_FP
0327f274  01dd0f6d d3d9_old!CD3DHal::SetPixelShader_FP
0327f278  01db67ea d3d9_old!CD3DBase::SetPixelShaderConstantF_FP
0327f27c  01e6d3d9 d3d9_old!CD3DBase::SetPixelShaderConstantI_FP
0327f280  01e6d58b d3d9_old!CD3DBase::SetPixelShaderConstantB_FP
0327f284  01d99261 d3d9_old!CD3DHal::SetFVF_FP
0327f288  01d7d318 d3d9_old!CD3DHal::SetTexture_FP
0327f28c  01d9b900 d3d9_old!CD3DBase::SetIndices_FP
0327f290  01d93c81 d3d9_old!CD3DBase::SetStreamSource_FP
0327f294  01e67a3f d3d9_old!CD3DBase::SetStreamSourceFreq_FP
0327f298  01e673e4 d3d9_old!CD3DHal::UpdateRenderState
0327f29c  01d7d08a d3d9_old!CD3DHal::SetTransformI
0327f2a0  01e710c2 d3d9_old!CD3DHal::MultiplyTransformI
0327f2a4  01d81214 d3d9_old!CD3DHal::SetClipPlaneI
0327f2a8  01e707c7 d3d9_old!CD3DHal::UpdateDriverStates
0327f2ac  01d7ed49 d3d9_old!CD3DHal::SetViewportI
0327f2b0  01d93d7c d3d9_old!CD3DHal::SetStreamSourceI
0327f2b4  01db3577 d3d9_old!CD3DHal::SetVertexShaderConstantIntF
0327f2b8  01e73518 d3d9_old!CD3DHal::SetVertexShaderConstantIntI
0327f2bc  01e7364c d3d9_old!CD3DHal::SetVertexShaderConstantIntB
0327f2c0  01e7399b d3d9_old!CD3DHal::DrawPointsI
0327f2c4  01da3a0e d3d9_old!CD3DHal::SetLightI
0327f2c8  01da3967 d3d9_old!CD3DHal::LightEnableI
0327f2cc  01d815bc d3d9_old!CD3DHal::SetRenderStateInternal
0327f2d0  01dbe9f7 d3d9_old!CD3DHal::DrawPrimitiveUPI
0327f2d4  01e73b09 d3d9_old!CD3DHal::DrawIndexedPrimitiveUPI
0327f2d8  01d7d968 d3d9_old!CD3DHal::ClearI
0327f2dc  01d7d73c d3d9_old!CD3DHal::PickDrawPrimFn
0327f2e0  01d944dd d3d9_old!CD3DHal::UpdateVShader
0327f2e4  01d94613 d3d9_old!CD3DHal::ValidateDraw
0327f2e8  01d843a5 d3d9_old!CD3DHal::InitDevice
0327f2ec  01d84b7e d3d9_old!CD3DHal::StateInitialize
0327f2f0  01d9103b d3d9_old!CD3DHal::Destroy
0327f2f4  01d91e32 d3d9_old!CD3DHal::`vector deleting destructor'
*/