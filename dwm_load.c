#include "dwm_load.h"

#include <windows.h>
#include <winbase.h>

#define E_INVALID_FUNCTION HRESULT_FROM_WIN32(ERROR_INVALID_FUNCTION)

static void
dwmload_getproc(LPCSTR procname, FARPROC *pp)
{
	if( *pp )  {
		return;
	}

	static HMODULE g_h_dwmapi = NULL;
	if(!g_h_dwmapi) do {
		HMODULE h_dwmapi = NULL;
		if( !GetModuleHandleExA(0, "dwmapi", &h_dwmapi) ) {
			h_dwmapi = LoadLibraryA("dwmapi");
		}
		if( !h_dwmapi ) {
			break;
		}

		/* loading the dwmapi module may trigger a rance
		 * condition if dwmload is used concurrently.
		 * We catch this here, by performing an atomic
		 * compare and exchange here.
		 *
		 * If the C&E returns a not NULL pointer another
		 * thread beat the race; the resolve is to release
		 * the module reference just acquired. */
		if( InterlockedCompareExchangePointer(
			(PVOID volatile *)&g_h_dwmapi,
			(PVOID)h_dwmapi,
			NULL )
		) {
			FreeLibrary(h_dwmapi);
		}
	} while(0);

	FARPROC p = NULL;
	if( g_h_dwmapi ) {
		p = GetProcAddress(g_h_dwmapi, procname);
	}

	/* The compiler should optimize this to a single
	 * unconditional code path. */
	switch(sizeof(FARPROC)) {
	/* 8 bit and 16 bit sizes for FARPROC are quite unlikely,
	 * but we're good and cover these cases as well. */
	case 1:
		InterlockedExchange8((char volatile*)pp, (char)p);
		break;
	case 2:
		InterlockedExchange16((SHORT volatile*)pp, (SHORT)p);
		break;
	case 4:
		InterlockedExchange((LONG volatile*)pp, (LONG)p);
		break;
	case 8:
		InterlockedExchange64((LONGLONG volatile*)pp, (LONGLONG)p);
		break;

	/* practically we should never end up here */
	default:
		InterlockedExchangePointer((PVOID volatile *)pp, (PVOID)p);
		break;
	}
}

BOOL dwmload_DefWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam,
    LRESULT *plResult
)
{
	static BOOL (*p)(HWND, UINT, WPARAM, LPARAM, LRESULT*) = NULL;
	dwmload_getproc("DwmDefWindowProc", (FARPROC*)&p);
	return p ? p(hwnd, msg, wParam, lParam, plResult) : FALSE;
}

HRESULT dwmload_EnableBlurBehindWindow(
    HWND hWnd,
    const DWM_BLURBEHIND *pBlurBehind
)
{
	static HRESULT (*p)(HWND, const DWM_BLURBEHIND*) = NULL;
	dwmload_getproc("DwmEnableBlurBehindWindow", (FARPROC*)&p);
	return p ? p(hWnd, pBlurBehind) : E_INVALID_FUNCTION;
}

HRESULT dwmload_EnableComposition(
    UINT uCompositionAction
)
{
	static HRESULT (*p)(UINT) = NULL;
	dwmload_getproc("DwmEnableComposition", (FARPROC*)&p);
	return p ? p(uCompositionAction) : E_INVALID_FUNCTION;
}

HRESULT dwmload_EnableMMCSS(
    BOOL fEnableMMCSS
)
{
	static HRESULT (*p)(BOOL) = NULL;
	dwmload_getproc("DwmEnableMMCSS", (FARPROC*)&p);
	return p ? p(fEnableMMCSS) : E_INVALID_FUNCTION;
}

HRESULT dwmload_ExtendFrameIntoClientArea(
    HWND hWnd,
    const MARGINS *pMarInset
)
{
	static HRESULT (*p)(HWND, const MARGINS*) = NULL;
	dwmload_getproc("DwmExtendFrameIntoClientArea", (FARPROC*)&p);
	return p ? p(hWnd, pMarInset) : E_INVALID_FUNCTION;
}

HRESULT dwmload_GetColorizationColor(
    DWORD *pcrColorization,
    BOOL *pfOpaqueBlend
)
{
	static HRESULT (*p)(DWORD*, BOOL*) = NULL;
	dwmload_getproc("DwmGetColorizationColor", (FARPROC*)&p);
	/* TODO: Implement Fallback that returns GDI system colors */
	return p ? p(pcrColorization, pfOpaqueBlend) : E_INVALID_FUNCTION;
}

HRESULT dwmload_GetCompositionTimingInfo(
    HWND hwnd,
    DWM_TIMING_INFO *pTimingInfo
)
{
	static HRESULT (*p)(HWND, DWM_TIMING_INFO*) = NULL;
	dwmload_getproc("DwmGetCompositionTimingInfo", (FARPROC*)&p);
	return p ? p(hwnd, pTimingInfo) : E_INVALID_FUNCTION;
}

HRESULT dwmload_GetWindowAttribute(
    HWND hwnd,
    DWORD dwAttribute,
    PVOID pvAttribute,
    DWORD cbAttribute
)
{
	static HRESULT (*p)(HWND, DWORD, PVOID, DWORD) = NULL;
	dwmload_getproc("DwmGetWindowAttribute", (FARPROC*)&p);
	if(!p) {
		memset(pvAttribute, 0, cbAttribute);
	}
	return p ? p(hwnd, dwAttribute, pvAttribute, cbAttribute) : E_INVALID_FUNCTION;
}

HRESULT dwmload_IsCompositionEnabled(
    BOOL *pfEnabled
)
{
	static HRESULT (*p)(BOOL*) = NULL;
	dwmload_getproc("DwmIsCompositionEnabled", (FARPROC*)&p);
	if(!p) {
		*pfEnabled = FALSE;
	}
	/* Don't return a error if DWM API is not available;
	 * it makes sense to inform the program, that composition
	 * is not enabled ony a system that does not support
	 * composition. */
	return p ? p(pfEnabled) : S_OK;
}

HRESULT dwmload_ModifyPreviousDxFrameDuration(
    HWND hwnd,
    INT cRefreshes,
    BOOL fRelative
)
{
	static HRESULT (*p)(HWND, INT, BOOL) = NULL;
	dwmload_getproc("DwmModifyPreviousDxFrameDuration", (FARPROC*)&p);
	return p ? p(hwnd, cRefreshes, fRelative) : S_OK;
}


HRESULT dwmload_QueryThumbnailSourceSize(
    HTHUMBNAIL hThumbnail,
    PSIZE pSize
)
{
	static HRESULT (*p)(HTHUMBNAIL, PSIZE) = NULL;
	dwmload_getproc("DwmQueryThumbnailSourceSize", (FARPROC*)&p);
	if(!p) {
		memset(pSize, 0, sizeof(*pSize));
	}
	return p ? p(hThumbnail, pSize) : E_INVALID_FUNCTION;
}

HRESULT dwmload_RegisterThumbnail(
    HWND hwndDestination,
    HWND *hwndSource,
    PHTHUMBNAIL phThumbnailId
)
{
	static HRESULT (*p)(HWND, HWND*, PHTHUMBNAIL) = NULL;
	dwmload_getproc("DwmRegisterThumbnail", (FARPROC*)&p);
	if(!p) {
		if( phThumbnailId) *phThumbnailId = NULL;
	}
	return p ? p(hwndDestination, hwndSource, phThumbnailId) : E_INVALID_FUNCTION;
}

HRESULT dwmload_SetDxFrameDuration(
    HWND hwnd,
    INT cRefreshes
)
{
	static HRESULT (*p)(HWND, INT) = NULL;
	dwmload_getproc("DwmSetDxFrameDuration", (FARPROC*)&p);
	return p ? p(hwnd, cRefreshes) : S_OK;
}

HRESULT dwmload_SetPresentParameters(
    HWND hwnd,
    DWM_PRESENT_PARAMETERS *pPresentParams
)
{
	static HRESULT (*p)(HWND, DWM_PRESENT_PARAMETERS*) = NULL;
	dwmload_getproc("DwmSetPresentParameters", (FARPROC*)&p);
	return p ? p(hwnd, pPresentParams) : S_OK;
}

HRESULT dwmload_SetWindowAttribute(
    HWND hwnd,
    DWORD dwAttribute,
    LPCVOID pvAttribute,
    DWORD cbAttribute
)
{
	static HRESULT (*p)(HWND, DWORD, LPCVOID, DWORD) = NULL;
	dwmload_getproc("DwmSetWindowAttribute", (FARPROC*)&p);
	return p ? p(hwnd, dwAttribute, pvAttribute, cbAttribute) : S_OK;
}

HRESULT dwmload_UnregisterThumbnail(
    HTHUMBNAIL hThumbnailId
)
{
	static HRESULT (*p)(HTHUMBNAIL) = NULL;
	dwmload_getproc("DwmUnregisterThumbnail", (FARPROC*)&p);
	return p ? p(hThumbnailId) : S_OK;
}

HRESULT dwmload_UpdateThumbnailProperties(
    HTHUMBNAIL hThumbnailId,
    const DWM_THUMBNAIL_PROPERTIES *ptnProperties
)
{
	static HRESULT (*p)(HTHUMBNAIL, const DWM_THUMBNAIL_PROPERTIES*) = NULL;
	dwmload_getproc("DwmUpdateThumbnailProperties", (FARPROC*)&p);
	return p ? p(hThumbnailId, ptnProperties) : S_OK;
}

