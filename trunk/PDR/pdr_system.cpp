#include "stdafx.h"
#include "CPDR.h"
#include "Psapi.h"

ZEND_FUNCTION(pdr_get_last_error)
{
	RETURN_LONG( ::GetLastError() )
}

ZEND_FUNCTION(pdr_execute)
{
	char * psFile ;
	int nFileLen = 0 ;

	char * psParams = NULL ;
	int nParamsLen = 0 ;

	char * psDir = NULL ;
	int nDirLen = 0 ;

	char * psOperation ;
	int nOperationLen = 0 ;

	int nShow = SW_SHOWNORMAL ;
	int nWnd = 0 ;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|sssll"
			, &psFile ,&nFileLen
			, &psParams ,&nParamsLen
			, &psDir ,&nDirLen
			, &psOperation, &nOperationLen
			, &nShow
			, &nWnd) == FAILURE )
	{
		RETURN_FALSE ;
	}

	HINSTANCE nRes = ::ShellExecute((HWND)nWnd
			, nOperationLen?psOperation:"open"
			, psFile, psParams, psDir, nShow ) ;

	RETURN_LONG( (long)nRes )
}


ZEND_FUNCTION(pdr_get_process_filename)
{
	DWORD nProcessId = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nProcessId) == FAILURE )
	{
		RETURN_FALSE
	}

	HANDLE hProcess ;
	HMODULE hModule ;
	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, nProcessId );
	if(!hProcess)
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid process handle." );
		RETURN_FALSE
	}

	DWORD cbNeeded ;
	if(!EnumProcessModules( hProcess, &hModule, sizeof(hModule), &cbNeeded))
	{
		CloseHandle( hProcess );
		RETURN_FALSE
	}

	char psProcessName[MAX_PATH] = "unknown" ;
	if(!GetModuleFileNameEx( hProcess, hModule, psProcessName, MAX_PATH))
	{
		CloseHandle( hProcess );
		RETURN_FALSE
	}

	CloseHandle( hProcess );
	RETURN_STRING(psProcessName,1) ;
} 

ZEND_FUNCTION(pdr_pump_msg)
{
	// 取得各项参数
	long nWnd=0 ;
	bool bRetAtOnce=false ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|lb", &nWnd,&bRetAtOnce) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = NULL ;
	if( nWnd )
	{
		hWnd = (HWND)nWnd ;
		if( !::IsWindow(hWnd) )
		{
			zend_error(E_WARNING, "PDR: the paramter 1 is not a valid window handle." );
			RETURN_FALSE
		}
	}

	if(bRetAtOnce)
	{
		MSG msg;
		BOOL bRet; 

		if( bRet=PeekMessage(&msg, hWnd,0,0,PM_REMOVE) )
		{
			if (bRet == -1)
			{
				// handle the error and possibly exit
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}
	}
	else
	{
		::AfxPumpMessage() ;
	}

	RETURN_TRUE
}

#define _make_msg_object(msg) zval * pZvalMsgPoint = NULL ; \
	MAKE_STD_ZVAL(pZvalMsgPoint) ;\
	array_init(pZvalMsgPoint);\
	add_index_long(pZvalMsgPoint,0,msg.pt.x) ;\
	add_index_long(pZvalMsgPoint,1,msg.pt.y) ;\
\
	zval * pZvalMsg = NULL ;\
	MAKE_STD_ZVAL(pZvalMsg) ;\
	object_init(pZvalMsg);\
	add_property_long(pZvalMsg, "hwnd", (long)msg.hwnd) ;\
	add_property_long(pZvalMsg, "message", (long)msg.message) ;\
	add_property_long(pZvalMsg, "wParam", (long)msg.wParam) ;\
	add_property_long(pZvalMsg, "lParam", (long)msg.lParam) ;\
	add_property_long(pZvalMsg, "time", (long)msg.time) ;\
	add_property_zval(pZvalMsg, "pt", pZvalMsgPoint) ;\



ZEND_FUNCTION(pdr_peek_msg)
{
	// 取得各项参数
	long nWnd=0, nMsgFilterMin=0, nMsgFilterMax=0 ;
	bool bRemove=false ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|lllb", &nWnd,&nMsgFilterMin,&nMsgFilterMax,&bRemove) == FAILURE )
	{
		RETURN_NULL()
	}

	HWND hWnd = NULL ;
	if( nWnd )
	{
		hWnd = (HWND)nWnd ;
		if( !::IsWindow(hWnd) )
		{
			zend_error(E_WARNING, "PDR: the paramter 1 is not a valid window handle." );
			RETURN_FALSE
		}
	}

	MSG msg;
	if( !::PeekMessage(&msg,hWnd,nMsgFilterMin,nMsgFilterMax,bRemove) )
	{
		RETURN_NULL()
	}

	_make_msg_object(msg)
	RETURN_ZVAL(pZvalMsg,0,0) ;
	
}

ZEND_FUNCTION(pdr_get_msg)
{
	// 取得各项参数
	long nWnd=0, nMsgFilterMin=0, nMsgFilterMax=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|lll", &nWnd,&nMsgFilterMin,&nMsgFilterMax) == FAILURE )
	{
		RETURN_NULL()
	}

	HWND hWnd = NULL ;
	if( nWnd )
	{
		hWnd = (HWND)nWnd ;
		if( !::IsWindow(hWnd) )
		{
			zend_error(E_WARNING, "PDR: the paramter 1 is not a valid window handle." );
			RETURN_FALSE
		}
	}

	MSG msg;
	if( ::GetMessage(&msg,hWnd,nMsgFilterMin,nMsgFilterMax)<0 )
	{
		RETURN_NULL()
	}

	_make_msg_object(msg)
	RETURN_ZVAL(pZvalMsg,0,0) ;
}

ZEND_FUNCTION(pdr_dispatch_msg)
{
	// 取得参数
	zval * pZvalMsg ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &pZvalMsg) == FAILURE )
	{
		RETURN_FALSE
	}

	zval * pZvalHwnd = zend_read_property(Z_OBJCE_P(pZvalMsg), pZvalMsg,"hwnd",4,0 TSRMLS_CC) ;
	zval * pZvalMessage = zend_read_property(Z_OBJCE_P(pZvalMsg), pZvalMsg,"message",7,0 TSRMLS_CC) ;
	zval * pZvalWParam = zend_read_property(Z_OBJCE_P(pZvalMsg), pZvalMsg,"wParam",6,0 TSRMLS_CC) ;
	zval * pZvalLParam = zend_read_property(Z_OBJCE_P(pZvalMsg), pZvalMsg,"lParam",6,0 TSRMLS_CC) ;
	zval * pZvalTime = zend_read_property(Z_OBJCE_P(pZvalMsg), pZvalMsg,"time",4,0 TSRMLS_CC) ;
	zval * pZvalPT = zend_read_property(Z_OBJCE_P(pZvalMsg), pZvalMsg,"pt",2,0 TSRMLS_CC) ;
	zval ** ppZvalPTX ;
	zval ** ppZvalPTY ;
	if( pZvalPT && Z_ARRVAL_P(pZvalPT) )
	{
		zend_hash_index_find(Z_ARRVAL_P(pZvalPT), 0, (void**)&ppZvalPTX) ;
		zend_hash_index_find(Z_ARRVAL_P(pZvalPT), 1, (void**)&ppZvalPTY) ;
	}
	else
	{
		ppZvalPTX = NULL ;
		ppZvalPTY = NULL ;
	}

	MSG msg ;
	msg.hwnd = (HWND)Z_LVAL_P(pZvalHwnd) ;
	msg.message = (UINT)Z_LVAL_P(pZvalMessage) ;
	msg.wParam = (WPARAM)Z_LVAL_P(pZvalWParam) ;
	msg.lParam = (LPARAM)Z_LVAL_P(pZvalLParam) ;
	msg.time = (DWORD)Z_LVAL_P(pZvalTime) ;
	msg.pt.x = ppZvalPTX? Z_LVAL_PP(ppZvalPTX): 0 ;
	msg.pt.y = ppZvalPTY? Z_LVAL_PP(ppZvalPTY): 0 ;

	::TranslateMessage(&msg) ;
	::DispatchMessage(&msg) ;
}

ZEND_FUNCTION(pdr_create_mutex)
{
	// 取得参数
	char * psMutexName ;
	long nMutexNameLen = 0 ;
	bool bInitialOwner = false ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|bl", &psMutexName, &nMutexNameLen, &bInitialOwner) == FAILURE )
	{
		RETURN_FALSE
	}

	HANDLE nHandle = CreateMutex(0,bInitialOwner,psMutexName) ;
	DWORD nErr = ::GetLastError() ;

	// 
	if(nErr==ERROR_ALREADY_EXISTS)
	{
		::CloseHandle(nHandle) ;
		RETURN_FALSE
	}

	RETURN_LONG((long)nHandle)
	// RETURN_BOOL( nErr!=ERROR_ALREADY_EXISTS )
}

ZEND_FUNCTION(pdr_open_mutex)
{
	// 取得参数
	char * psMutexName ;
	long nMutexNameLen = 0 ;
	bool bInheritHandle=true ;
	long nDesiredAccess = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|bl", &psMutexName, &nMutexNameLen, &bInheritHandle, &nDesiredAccess) == FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_LONG((long)::OpenMutex(nDesiredAccess,bInheritHandle,psMutexName)) ;
}

ZEND_FUNCTION(pdr_release_mutex)
{
	long nHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nHandle) == FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_BOOL(::ReleaseMutex((HANDLE)nHandle)) ;
}




ZEND_FUNCTION(pdr_close_handle)
{
	// 取得参数
	long nHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nHandle) == FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_BOOL(::CloseHandle((HANDLE)nHandle)) ;
}