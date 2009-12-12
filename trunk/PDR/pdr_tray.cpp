#include "stdafx.h"
#include "CPDR.h"

#define _tray_icon_set long nHwnd, nTrayId, nTipLen=0, nIconFileLen=0 ;\
	char *psTip= "", *psIconFile= NULL;\
    if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls|s",&nHwnd, &nTrayId, &psIconFile, &nIconFileLen, &psTip, &nTipLen) == FAILURE )\
	{\
		RETURN_FALSE;\
	}\
	HWND hHWnd = (HWND)nHwnd ;\
	if(!::IsWindow(hHWnd))\
	{\
		zend_error(E_WARNING, "PDR Tray Icon: first param is not a valid window handle." );\
		RETURN_FALSE;\
	}\
	NOTIFYICONDATA tray ;\
	tray.cbSize = sizeof(NOTIFYICONDATA) ;\
	tray.hWnd = hHWnd ;\
	tray.uID = nTrayId ; \
	tray.uCallbackMessage = WM_TRAY_ICON ;\
	tray.uFlags = NIF_MESSAGE|NIF_TIP|NIF_ICON ;\
	tray.hIcon = (HICON)LoadImage(NULL, psIconFile, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE|LR_LOADTRANSPARENT);\
	if( !tray.hIcon ) {\
		zend_error( E_WARNING, "PDR Tray Icon : bad icon filename" );\
		RETURN_FALSE;\
	}\
	strncpy_s(tray.szTip, psTip, 63);\
	tray.szTip[63]= 0;

ZEND_FUNCTION(pdr_tray_create)
{
	_tray_icon_set
	RETURN_BOOL(Shell_NotifyIcon(NIM_ADD, &tray)) ;
}


ZEND_FUNCTION(pdr_tray_modify)
{
	_tray_icon_set
	RETURN_BOOL(Shell_NotifyIcon(NIM_MODIFY, &tray)) ;
}

ZEND_FUNCTION(pdr_tray_delete)
{
	long nHwnd, nTrayId ;

    if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll",&nHwnd, &nTrayId) == FAILURE )
	{
		RETURN_FALSE;
	}

	HWND hHWnd = (HWND)nHwnd ;
	if(!::IsWindow(hHWnd))
	{
		zend_error(E_WARNING, "PDR Tray Icon: first param is not a valid window handle." );
		RETURN_FALSE;
	}

	NOTIFYICONDATA tray ;
	tray.cbSize = sizeof(NOTIFYICONDATA) ;
	tray.hWnd = hHWnd ;
	tray.uID = nTrayId ; // User ID

	tray.uCallbackMessage = 0 ;
	tray.szTip[0]= '\0' ;
	tray.hIcon= LoadIcon(NULL, IDI_APPLICATION) ;
	tray.uFlags = 0 ;

	RETURN_BOOL(Shell_NotifyIcon(NIM_DELETE, &tray)) ;


}