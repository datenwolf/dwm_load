#ifndef DWM_LOAD_H
#define DWM_LOAD_H

#include <dwmapi.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL dwmload_DefWindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam,
    LRESULT *plResult
);
#define DwmDefWindowProc dwmload_DefWindowProc

HRESULT dwmload_EnableBlurBehindWindow(
    HWND hWnd,
    const DWM_BLURBEHIND *pBlurBehind
);
#define DwmEnableBlurBehindWindow dwmload_EnableBlurBehindWindow

HRESULT dwmload_EnableComposition(
    UINT uCompositionAction
);
#define DwmEnableComposition dwmload_EnableComposition

HRESULT dwmload_EnableMMCSS(
    BOOL fEnableMMCSS
);
#define DwmEnableMMCSS dwmload_EnableMMCSS

HRESULT dwmload_ExtendFrameIntoClientArea(
    HWND hWnd,
    const MARGINS *pMarInset
);
#define DwmExtendFrameIntoClientArea dwmload_ExtendFrameIntoClientArea

HRESULT dwmload_GetColorizationColor(
    DWORD *pcrColorization,
    BOOL *pfOpaqueBlend
);
#define DwmGetColorizationColor dwmload_GetColorizationColor

HRESULT dwmload_GetCompositionTimingInfo(
    HWND hwnd,
    DWM_TIMING_INFO *pTimingInfo
);
#define DwmGetCompositionTimingInfo dwmload_GetCompositionTimingInfo

HRESULT dwmload_GetWindowAttribute(
    HWND hwnd,
    DWORD dwAttribute,
    PVOID pvAttribute,
    DWORD cbAttribute
);
#define DwmGetWindowAttribute dwmload_GetWindowAttribute

HRESULT dwmload_IsCompositionEnabled(
    BOOL *pfEnabled
);
#define DwmIsCompositionEnabled dwmload_IsCompositionEnabled

HRESULT dwmload_ModifyPreviousDxFrameDuration(
    HWND hwnd,
    INT cRefreshes,
    BOOL fRelative
);
#define DwmModifyPreviousDxFrameDuration dwmload_ModifyPreviousDxFrameDuration

HRESULT dwmload_QueryThumbnailSourceSize(
    HTHUMBNAIL hThumbnail,
    PSIZE pSize
);
#define DwmQueryThumbnailSourceSize dwmload_QueryThumbnailSourceSize

HRESULT dwmload_RegisterThumbnail(
    HWND hwndDestination,
    HWND *hwndSource,
    PHTHUMBNAIL phThumbnailId
);
#define DwmRegisterThumbnail dwmload_RegisterThumbnail

HRESULT dwmload_SetDxFrameDuration(
    HWND hwnd,
    INT cRefreshes
);
#define DwmSetDxFrameDuration dwmload_SetDxFrameDuration

HRESULT dwmload_SetPresentParameters(
    HWND hwnd,
    DWM_PRESENT_PARAMETERS *pPresentParams
);
#define DwmSetPresentParameters dwmload_SetPresentParameters

HRESULT dwmload_SetWindowAttribute(
    HWND hwnd,
    DWORD dwAttribute,
    LPCVOID pvAttribute,
    DWORD cbAttribute
);
#define DwmSetWindowAttribute dwmload_SetWindowAttribute

HRESULT dwmload_UnregisterThumbnail(
    HTHUMBNAIL hThumbnailId
);
#define DwmUnregisterThumbnail dwmload_UnregisterThumbnail

HRESULT dwmload_UpdateThumbnailProperties(
    HTHUMBNAIL hThumbnailId,
    const DWM_THUMBNAIL_PROPERTIES *ptnProperties
);
#define DwmUpdateThumbnailProperties dwmload_UpdateThumbnailProperties

#endif/*DWM_LOAD_H*/
