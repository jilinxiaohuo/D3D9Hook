#include <windows.h>
#include <stdio.h>

#include <cstdint>
#include <d3d9.h>
#include <D3dx9tex.h>

#pragma comment(lib, "D3dx9.lib")

#include "dllmain.h"

TCreateDevice			OldCreateDevice;
TClear					OldClear;
TDrawIndexedPrimitive	OldDrawIndexedPrimitive;
TDrawIndexedPromitiveUP OldDrawIndexedPrimitiveUP;
TDrawPrimitive			OldDrawPrimitive;
TDrawPrimitiveUP		OldDrawPrimitiveUP;
TDrawRectPatch			OldDrawRectPatch;
TDrawTriPatch			OldDrawTriPatch;
TPresent				OldPresent;
TSetRenderTarget		OldSetRenderTarget;

void* EntryPoints[14];

uint32_t nSeed = 0;
uint32_t nDrawCalls = 0;
BOOL	 bCapture = FALSE;
BOOL     bShowInfo = FALSE;
BOOL     bChange = FALSE;

#pragma pack(1)
typedef struct _JMPCODE
{
	BYTE jmp;
	DWORD addr;
}JMPCODE, *PJMPCODE;


int __cdecl D3DRecordHRESULT(int hr, char* str1, char* str2, int arg)
{
	char szError[1024];
	sprintf(szError, "hr:%X arg:%X error: %s\n%s\n", hr, arg, str1, str2);
	OutputDebugStringA(szError);
	return 0;
}


BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			OutputDebugString(L"DllMain");
			HINSTANCE hInst = LoadLibraryA(R"(c:\windows\syswow64\d3d9.dll)");
			EntryPoints[0] = GetProcAddress(hInst, "D3DPERF_BeginEvent");
			EntryPoints[1] = GetProcAddress(hInst, "D3DPERF_EndEvent");
			EntryPoints[2] = GetProcAddress(hInst, "D3DPERF_GetStatus");
			EntryPoints[3] = GetProcAddress(hInst, "D3DPERF_QueryRepeatFrame");
			EntryPoints[4] = GetProcAddress(hInst, "D3DPERF_SetMarker");
			EntryPoints[5] = GetProcAddress(hInst, "D3DPERF_SetOptions");
			EntryPoints[6] = GetProcAddress(hInst, "D3DPERF_SetRegion");
			EntryPoints[7] = GetProcAddress(hInst, "DebugSetLevel");
			EntryPoints[8] = GetProcAddress(hInst, "DebugSetMute");
			EntryPoints[9] = GetProcAddress(hInst, "Direct3DCreate9");
			EntryPoints[10] = GetProcAddress(hInst, "Direct3DCreate9Ex");
			EntryPoints[11] = GetProcAddress(hInst, "Direct3DShaderValidatorCreate9");
			EntryPoints[12] = GetProcAddress(hInst, "PSGPError");
			EntryPoints[13] = GetProcAddress(hInst, "PSGPSampleTexture");		

			/*void* oldaddr = (void*)((int)hInst + 0x1A89B);

			JMPCODE shellcode;

			shellcode.jmp = 0xe9;
			shellcode.addr = (int)D3DRecordHRESULT - (int)oldaddr - 5;

			WriteProcessMemory(GetCurrentProcess(), oldaddr, &shellcode, sizeof(JMPCODE), NULL);*/
			//hook D3DRecordHRESULT
		}
	}
	return TRUE;
}

VOID __declspec(naked) __declspec(dllexport) D3DPERF_BeginEvent()
{
	__asm jmp dword ptr[EntryPoints + 0];
}

VOID __declspec(naked) __declspec(dllexport) D3DPERF_EndEvent(int)
{
	__asm jmp dword ptr[EntryPoints + 4];
}

VOID __declspec(naked) __declspec(dllexport) D3DPERF_GetStatus(int)
{
	__asm jmp dword ptr[EntryPoints + 8];
}

VOID __declspec(naked) __declspec(dllexport) D3DPERF_QueryRepeatFrame(int)
{
	__asm jmp dword ptr[EntryPoints + 12];
}

VOID __declspec(naked) __declspec(dllexport) D3DPERF_SetMarker()
{
	__asm jmp dword ptr[EntryPoints + 16];
}

VOID __declspec(naked) __declspec(dllexport) D3DPERF_SetOptions()
{
	__asm jmp dword ptr[EntryPoints + 20];
}

VOID __declspec(naked) __declspec(dllexport) D3DPERF_SetRegion()
{
	__asm jmp dword ptr[EntryPoints + 24];
}

VOID __declspec(naked) __declspec(dllexport) DebugSetLevel()
{
	__asm jmp dword ptr[EntryPoints + 28];
}

VOID __declspec(naked) __declspec(dllexport) DebugSetMute()
{
	__asm jmp dword ptr[EntryPoints + 32];
}

/*VOID __declspec(naked) __declspec(dllexport) Direct3DCreate9()
{
	__asm jmp dword ptr[EntryPoints + 36];
}*/

IDirect3D9* WINAPI Direct3DCreate9(
	UINT SDKVersion
	)
{
	char szVer[1024];
	sprintf(szVer, "Direct3DCreate9 %d", SDKVersion);

	OutputDebugStringA(szVer);

	IDirect3D9* p = ((decltype(Direct3DCreate9)*)EntryPoints[9])(SDKVersion);

	auto tmp = (TCreateDevice)HookVirtualTable(p, 0x40, MyCreateDevice);

	OldCreateDevice = tmp ? tmp : OldCreateDevice;

	return p;
}

VOID __declspec(naked) __declspec(dllexport) Direct3DCreate9Ex()
{
	__asm jmp dword ptr[EntryPoints + 40];
}

VOID __declspec(naked) __declspec(dllexport) Direct3DShaderValidatorCreate9()
{
	__asm jmp dword ptr[EntryPoints + 44];
}

VOID __declspec(naked) __declspec(dllexport) PSGPError()
{
	__asm jmp dword ptr[EntryPoints + 48];
}

VOID __declspec(naked) __declspec(dllexport) PSGPSampleTexture()
{
	__asm jmp dword ptr[EntryPoints + 52];
}

LPVOID HookVirtualTable(
	LPVOID instance,
	uint32_t index,
	LPVOID newaddr)
{
	uintptr_t vptr = *reinterpret_cast<uintptr_t*>(instance);
	uintptr_t old = *reinterpret_cast<uintptr_t*>(vptr + index);

	if (old == reinterpret_cast<uintptr_t>(newaddr))
	{
		return nullptr;
	}

	WriteProcessMemory(
		GetCurrentProcess(),
		(LPVOID)(vptr + index),
		&newaddr,
		sizeof(uintptr_t),
		NULL);

	return (LPVOID)old;
}



HRESULT WINAPI MyCreateDevice(
	IDirect3D9*			  Direct3D9,
	UINT                  Adapter,
	D3DDEVTYPE            DeviceType,
	HWND                  hFocusWindow,
	DWORD                 BehaviorFlags,
	D3DPRESENT_PARAMETERS *pPresentationParameters,
	IDirect3DDevice9      **ppReturnedDeviceInterface
	)
{
	char szDeviceType[1024];

	sprintf(szDeviceType, 
		"Adapter:                   %x\n"
		"DeviceType:                %x\n"
		"AutoDepthStencilFormat:    %x\n"
		"BackBufferCount:           %x\n"
		"BackBufferFormat:          %x\n"
		"BackBufferHeight:          %x\n"
		"BackBufferWidht:           %x\n"
		"EnableAutoDepthStencil:    %x\n"
		"Flags:                     %x\n"
		"FullScreen_RefreshRateInHz:%x\n"
		"MultiSampleQuality:        %x\n"
		"MultiSampleType:           %x\n"
		"PresentationInterval:      %x\n"
		"SwapEffect:                %x\n", 
		Adapter, 
		DeviceType, 
		pPresentationParameters->AutoDepthStencilFormat,
		pPresentationParameters->BackBufferCount,
		pPresentationParameters->BackBufferFormat,
		pPresentationParameters->BackBufferHeight,
		pPresentationParameters->BackBufferWidth,
		pPresentationParameters->EnableAutoDepthStencil,
		pPresentationParameters->Flags,
		pPresentationParameters->FullScreen_RefreshRateInHz,
		pPresentationParameters->MultiSampleQuality,
		pPresentationParameters->MultiSampleType,
		pPresentationParameters->PresentationInterval,
		pPresentationParameters->SwapEffect);

	OutputDebugStringA(szDeviceType);

	auto hr = OldCreateDevice(
		Direct3D9,
		Adapter,
		DeviceType,
		hFocusWindow,
		BehaviorFlags,
		pPresentationParameters,
		ppReturnedDeviceInterface);

	if (*ppReturnedDeviceInterface)
	{
		LPVOID inst = (LPVOID)(*reinterpret_cast<uintptr_t*>(*ppReturnedDeviceInterface) - sizeof(uintptr_t));

		OldClear = (TClear)HookVirtualTable(inst, 0x0AC, MyClear);
		OldDrawIndexedPrimitive = (TDrawIndexedPrimitive)HookVirtualTable(inst, 0x148, MyDrawIndexedPrimitive);
		OldDrawPrimitiveUP = (TDrawPrimitiveUP)HookVirtualTable(inst, 0x14C, MyDrawPrimitiveUP);
		OldDrawPrimitive = (TDrawPrimitive)HookVirtualTable(inst, 0x144, MyDrawPrimitive);
		OldDrawIndexedPrimitiveUP = (TDrawIndexedPromitiveUP)HookVirtualTable(inst, 0x150, MyDrawIndexedPrimitiveUP);
		OldDrawRectPatch = (TDrawRectPatch)HookVirtualTable(inst, 0x1CC, MyDrawRectPatch);
		OldDrawTriPatch = (TDrawTriPatch)HookVirtualTable(inst, 0x1D0, MyDrawTriPatch);
		OldPresent = (TPresent)HookVirtualTable(inst, 0x044, MyPresent);
		OldSetRenderTarget = (TSetRenderTarget)HookVirtualTable(inst, 0x094, MySetRenderTarget);

		/*OldClear = (TClear)HookVirtualTable(*ppReturnedDeviceInterface, 0x0AC, MyClear);
		OldDrawIndexedPrimitive = (TDrawIndexedPrimitive)HookVirtualTable(*ppReturnedDeviceInterface, 0x148, MyDrawIndexedPrimitive);
		OldDrawPrimitiveUP = (TDrawPrimitiveUP)HookVirtualTable(*ppReturnedDeviceInterface, 0x14C, MyDrawPrimitiveUP);
		OldDrawPrimitive = (TDrawPrimitive)HookVirtualTable(*ppReturnedDeviceInterface, 0x144, MyDrawPrimitive);
		OldDrawIndexedPrimitiveUP = (TDrawIndexedPromitiveUP)HookVirtualTable(*ppReturnedDeviceInterface, 0x150, MyDrawIndexedPrimitiveUP);
		OldDrawRectPatch = (TDrawRectPatch)HookVirtualTable(*ppReturnedDeviceInterface, 0x1CC, MyDrawRectPatch);
		OldDrawTriPatch = (TDrawTriPatch)HookVirtualTable(*ppReturnedDeviceInterface, 0x1D0, MyDrawTriPatch);
		OldPresent = (TPresent)HookVirtualTable(*ppReturnedDeviceInterface, 0x044, MyPresent);
		OldSetRenderTarget = (TSetRenderTarget)HookVirtualTable(*ppReturnedDeviceInterface, 0x094, MySetRenderTarget);*/
	}

	return hr;
}

HRESULT WINAPI MyClear(
	IDirect3DDevice9	*pDevice,
	DWORD				Count,
	const D3DRECT		*pRects,
	DWORD				Flags,
	D3DCOLOR			Color,
	float				Z,
	DWORD				Stencil
	)
{
	auto hr = OldClear(
		pDevice,
		Count,
		pRects,
		Flags,
		Color,
		Z,
		Stencil);

	//OutputDebugString(L"MyClear");
	/*IDirect3DSurface9* pSurface = nullptr;
	pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);

	if (pSurface)
	{
		D3DXSaveSurfaceToFile(LR"(D:\clear.PNG)", D3DXIFF_PNG, pSurface, nullptr, nullptr);
		OutputDebugString(L"Clear");
	}
	*/
	return hr;
}

HRESULT WINAPI MyDrawIndexedPrimitive(
	IDirect3DDevice9*	pDevice,
	D3DPRIMITIVETYPE	Type,
	INT					BaseVertexIndex,
	UINT				MinIndex,
	UINT				NumVertices,
	UINT				StartIndex,
	UINT				PrimitiveCount
	)
{
	//OutputDebugString(L"MyDrawIndexedPrimitive");
	auto hr = OldDrawIndexedPrimitive(
		pDevice,
		Type,
		BaseVertexIndex,
		MinIndex,
		NumVertices,
		StartIndex,
		PrimitiveCount);


	IDirect3DSurface9* pSurface = nullptr;
	pDevice->GetRenderTarget(0, &pSurface);

	if (pSurface)
	{
		if (bCapture)
		{
			wchar_t szFile[1024];

			swprintf(szFile, L"D:\\images\\%d.png", GetTickCount() + nSeed++);
			D3DXSaveSurfaceToFile(szFile, D3DXIFF_PNG, pSurface, nullptr, nullptr);
		}
		pSurface->Release();
	}

	nDrawCalls++;
	
	return hr;
}

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
	)
{
	//OutputDebugString(L"MyDrawIndexedPrimitiveUP");
	auto hr = OldDrawIndexedPrimitiveUP(
		pDevice,
		PrimitiveType,
		MinVertexIndex,
		NumVertices,
		PrimitiveCount,
		pIndexData,
		IndexDataFormat,
		pVertexStreamZeroData,
		VertexStreamZeroStride
		);

	IDirect3DSurface9* pSurface = nullptr;
	pDevice->GetRenderTarget(0, &pSurface);

	if (pSurface)
	{
		if (bCapture)
		{
			wchar_t szFile[1024];

			swprintf(szFile, L"D:\\images\\%d.png", GetTickCount() + nSeed++);
			D3DXSaveSurfaceToFile(szFile, D3DXIFF_PNG, pSurface, nullptr, nullptr);
		}
		pSurface->Release();
	}

	nDrawCalls++;

	return hr;
}

HRESULT WINAPI MyDrawPrimitive(
	IDirect3DDevice9	*pDevice,
	D3DPRIMITIVETYPE	PrimitiveType,
	UINT				StartVertex,
	UINT				PrimitiveCount
	)
{
	//OutputDebugString(L"MyDrawPrimitive");
	auto hr = OldDrawPrimitive(
		pDevice,
		PrimitiveType,
		StartVertex,
		PrimitiveCount);

	IDirect3DSurface9* pSurface = nullptr;
	pDevice->GetRenderTarget(0, &pSurface);

	if (pSurface)
	{
		if (bCapture)
		{
			wchar_t szFile[1024];

			swprintf(szFile, L"D:\\images\\%d.png", GetTickCount() + nSeed++);
			D3DXSaveSurfaceToFile(szFile, D3DXIFF_PNG, pSurface, nullptr, nullptr);
		}
		pSurface->Release();
	}
	
	nDrawCalls++;

	return hr;
}

HRESULT WINAPI MyDrawPrimitiveUP(
	IDirect3DDevice9	*pDevice,
	D3DPRIMITIVETYPE	PrimitiveType,
	UINT				PrimitiveCount,
	const void			*pVertexStreamZeroData,
	UINT				VertexStreamZeroStride
	)
{
	//OutputDebugString(L"MyDrawPrimitiveUP");
	auto hr = OldDrawPrimitiveUP(
		pDevice,
		PrimitiveType,
		PrimitiveCount,
		pVertexStreamZeroData,
		VertexStreamZeroStride);

	IDirect3DSurface9* pSurface = nullptr;
	pDevice->GetRenderTarget(0, &pSurface);

	if (pSurface)
	{
		if (bCapture)
		{
			wchar_t szFile[1024];

			swprintf(szFile, L"D:\\images\\%d.png", GetTickCount() + nSeed++);
			D3DXSaveSurfaceToFile(szFile, D3DXIFF_PNG, pSurface, nullptr, nullptr);
		}
		pSurface->Release();
	}

	nDrawCalls++;

	return hr;
}

HRESULT WINAPI MyDrawRectPatch(
	IDirect3DDevice9	*pDevice,
	UINT				Handle,
	const float			*pNumSegs,
	const D3DRECTPATCH_INFO *pRectPatchInfo
	)
{
	//OutputDebugString(L"MyDrawRectPatch");
	auto hr = OldDrawRectPatch(
		pDevice,
		Handle,
		pNumSegs,
		pRectPatchInfo);

	nDrawCalls++;

	return hr;
}

HRESULT WINAPI MyDrawTriPatch(
	IDirect3DDevice9	*pDevice,
	UINT				Handle,
	const float			*pNumSegs,
	const D3DTRIPATCH_INFO *pTriPatchInfo
	)
{
	//OutputDebugString(L"MyDrawTriPatch");
	auto hr = OldDrawTriPatch(
		pDevice,
		Handle,
		pNumSegs,
		pTriPatchInfo);

	nDrawCalls++;

	return hr;
}

HRESULT WINAPI MyPresent(
	IDirect3DDevice9* pDevice,
	const RECT    *pSourceRect,
	const RECT    *pDestRect,
	HWND    hDestWindowOverride,
	const RGNDATA *pDirtyRegion
	)
{
	//OutputDebugString(L"MyPresent");
	LPD3DXFONT pFont = nullptr;
	D3DXCreateFont(
		pDevice,
		18, 0,   // 字体字符的宽高
		1,       // 是否加粗
		1,       // Mipmap级别
		0,       // 是否为斜体
		DEFAULT_CHARSET,        // 设置默认字符集
		OUT_DEFAULT_PRECIS,     // 输出精度，使用默认值
		DEFAULT_QUALITY,        // 文本质量
		DEFAULT_PITCH | FF_DONTCARE,
		L"Arial",   // 字体类型名称
		&pFont     // 要填充的LPD3DXFONT字体对象
		);

	if (pFont && bShowInfo)
	{
		wchar_t szInfo[1024];
		swprintf(szInfo, L"draw calls:%d\n", nDrawCalls);
		RECT pos = { 0, 0, 200, 200 };
		pFont->DrawTextW(nullptr, szInfo, -1, &pos, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

		pFont->Release();
	}

	nDrawCalls = 0;

	if (GetKeyState(VK_F11) < 0)
	{
		if (bChange)
		{
			bShowInfo = !bShowInfo;
			bChange = FALSE;
		}
	}
	else
	{
		bChange = TRUE;
	}

	bCapture = GetKeyState(VK_F4) < 0 ? TRUE : FALSE;

	auto hr = OldPresent(
		pDevice,
		pSourceRect,
		pDestRect,
		hDestWindowOverride,
		pDirtyRegion);

	return hr;
}

HRESULT WINAPI MySetRenderTarget(
	IDirect3DDevice9*		pDevice,
	DWORD					RenderTargetIndex,
	IDirect3DSurface9		*pRenderTarget
	)
{
	//OutputDebugString(L"MySetRenderTarget");
	auto hr = OldSetRenderTarget(
		pDevice,
		RenderTargetIndex,
		pRenderTarget);

	return hr;
}