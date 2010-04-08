#include "stdafx.h"
#include "CPDR.h"

ZEND_FUNCTION(pdr_cb_open)
{
	int nNewOwnerWnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nNewOwnerWnd ) == FAILURE )
	{
		RETURN_FALSE
	}

	if( ::OpenClipboard( (HWND)nNewOwnerWnd ) )
	{
		RETURN_TRUE ;
	}

	else
	{
		set_last_error ;
		RETURN_FALSE ;
	}
}

ZEND_FUNCTION(pdr_cb_close)
{
	if( ::CloseClipboard() )
	{
		RETURN_TRUE ;
	}

	else
	{
		set_last_error ;
		RETURN_FALSE ;
	}
}

ZEND_FUNCTION(pdr_cb_empty)
{
	if( ::EmptyClipboard () )
	{
		RETURN_TRUE ;
	}

	else
	{
		set_last_error ;
		RETURN_FALSE ;
	}
}

ZEND_FUNCTION(pdr_cb_set)
{
	char * psData ;
	int nFormat=CF_UNICODETEXT, nDataLen=0 ;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &psData, &nDataLen, &nFormat ) == FAILURE )
	{
		RETURN_FALSE
	}

    HANDLE hClip = GlobalAlloc(GMEM_MOVEABLE,nDataLen) ;
	if(!hClip)
	{
		set_last_error ;
		RETURN_FALSE ;
	}
	char *pBuf = (char*)::GlobalLock(hClip) ;
	if(!pBuf)
	{
		set_last_error ;
		RETURN_FALSE ;
	}

    memcpy(pBuf,psData,nDataLen) ;

    GlobalUnlock(hClip) ;
    SetClipboardData(nFormat,hClip) ;
}

ZEND_FUNCTION(pdr_cb_get)
{
	int nFormat=CF_UNICODETEXT ;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nFormat ) == FAILURE )
	{
		RETURN_FALSE
	}

	HANDLE hClip = ::GetClipboardData(nFormat) ;
	if(!hClip)
	{
		set_last_error ;
		RETURN_FALSE ;
	}

	DWORD dwSize = GlobalSize(hClip) ;
	if(!dwSize)
	{
		set_last_error ;
		RETURN_FALSE ;
	}

    char *pBuf = (char*)GlobalLock(hClip) ;
	if(pBuf)
	{
		set_last_error ;
		RETURN_FALSE ;
	}

    GlobalUnlock(hClip) ;
	RETURN_STRINGL(pBuf,dwSize,1) ;
}
