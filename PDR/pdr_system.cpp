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


ZEND_FUNCTION(pdr_register_hot_key)
{
	long nHwnd, nId, nModifyKey, nVK ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &nHwnd, &nId, &nModifyKey, &nVK) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hHWnd = (HWND)nHwnd ;
	if(!::IsWindow(hHWnd))
	{
		zend_error(E_WARNING, "PDR: first param is not a valid window handle." );
		RETURN_FALSE;
	}

	RETURN_BOOL( ::RegisterHotKey(hHWnd,nId,nModifyKey,nVK) ) ;
}
ZEND_FUNCTION(pdr_unregister_hot_key)
{
	long nHwnd, nId ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nHwnd, &nId) == FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_BOOL( ::UnregisterHotKey((HWND)nHwnd,nId) ) ;
}



ZEND_FUNCTION(pdr_get_system_path)
{
	long nFolder ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nFolder) == FAILURE )
	{
		RETURN_FALSE
	}

	char pPath[MAX_PATH] ;

	if( ::SHGetSpecialFolderPath(0,(LPSTR)pPath,nFolder,0) )
	{
		RETURN_STRING(pPath,1)
	}

	else
	{
		RETURN_FALSE
	}

}



ZEND_FUNCTION(pdr_browse_file)
{
	bool bOpenFileDialog = true ;
	char * psDefExt, * psFileName, * psFilter ;
	long nDefExtLen=0, nFileNameLen=0, nFilterLen=0, nFlags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, nParentWnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|bsslsl",
				&bOpenFileDialog 
				, &psDefExt, &nDefExtLen
				, &psFileName, &nFileNameLen
				, &nFlags
				, &psFilter, &nFilterLen
				, &nParentWnd) == FAILURE )
	{
		RETURN_FALSE
	}


	CWnd * pParentWnd = NULL ;
	if( nParentWnd )
	{
		if( !::IsWindow((HWND)nParentWnd) )
		{
			zend_error(E_WARNING, "PDR: first param is not a valid window handle." );
			RETURN_FALSE;
		}

		pParentWnd = new CWnd() ;
		pParentWnd->Attach((HWND)nParentWnd) ;
	}

	CFileDialog aDlg(
		bOpenFileDialog
		, nDefExtLen? psDefExt: NULL
		, nFileNameLen? psFileName: NULL
		, nFlags
		, nFilterLen? psFilter: NULL
		, pParentWnd
		, 0
	) ;

	if(pParentWnd)
	{
		pParentWnd->Detach() ;
		delete pParentWnd ;
	}

	if( aDlg.DoModal()==IDOK )
	{
		CString strPath = aDlg.GetPathName() ;
		RETURN_STRING((char *)(LPCTSTR)strPath,1)
	}

	else
	{
		RETURN_NULL()
	}

}

LPITEMIDLIST GetIDListFromPath(LPCTSTR lpszPath)  
{  
	LPITEMIDLIST pidl = NULL;  
	LPSHELLFOLDER pDesktopFolder;  
	OLECHAR szOleChar[MAX_PATH];    
	ULONG chEaten;    
	ULONG dwAttributes;    
	HRESULT hr;  

	//   Get   a   pointer   to   the   Desktop's   IShellFolder   interface.    
	if( SUCCEEDED( ::SHGetDesktopFolder( &pDesktopFolder ) ) )    
	{    
		//   IShellFolder::ParseDisplayName   requires   the   file   name   be   in    
		//   Unicode.    
		#if   !defined( _UNICODE )  
			::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, lpszPath, -1, szOleChar, MAX_PATH );  
		#else  
			::_tcscpy(   szOleChar,   lpszPath   );  
		#endif  

		//   Convert   the   path   to   an   ITEMIDLIST.    
		hr = pDesktopFolder->ParseDisplayName( NULL,NULL,szOleChar, &chEaten,&pidl,&dwAttributes) ;    

		pDesktopFolder->Release();  
		if (SUCCEEDED( hr ))
		{
			return   pidl;  
		}
	}  

	return   NULL;  
}
ZEND_FUNCTION(pdr_browse_folder)
{
	char *psRoot, * psTitle ;
	long nRootLen=0, nTitleLen=0 ;
	long nFlags = BIF_DONTGOBELOWDOMAIN|BIF_RETURNONLYFSDIRS, nParentWnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ssll",
				&psRoot, &nRootLen
				, &psTitle, &nTitleLen
				, &nFlags
				, &nParentWnd) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hParentWnd = (HWND)nParentWnd ;
	if( hParentWnd )
	{
		if( !::IsWindow(hParentWnd) )
		{
			zend_error(E_WARNING, "PDR: first param is not a valid window handle." );
			RETURN_FALSE;
		}
	}

	BROWSEINFO struInfo ;
	TCHAR pszPath[MAX_PATH] ;

	struInfo.hwndOwner = hParentWnd ;
	struInfo.pszDisplayName = pszPath ;
	struInfo.pidlRoot = nRootLen? GetIDListFromPath(psRoot): NULL ;  
	struInfo.ulFlags = nFlags ;  
	struInfo.lpfn=NULL ; 
	struInfo.lpszTitle = nTitleLen? psTitle: NULL ;
	struInfo.iImage=0 ;

	LPCITEMIDLIST pidl = SHBrowseForFolder(&struInfo) ;
	if(pidl==NULL)  
	{
		RETURN_NULL()
	}

	if(SHGetPathFromIDList(pidl,pszPath)==FALSE)
	{
		RETURN_NULL()
	}

	RETURN_STRING(pszPath,1) ;
}



ZEND_FUNCTION(pdr_get_locale)
{
	LCID nLCId = GetSystemDefaultLCID() ;
	RETURN_LONG((long)nLCId) ;
}
ZEND_FUNCTION(pdr_get_charset)
{
    char szCodePage[128] ;
	if(!GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTCODEPAGE,szCodePage, sizeof(szCodePage)-1))
    {
		zend_error(E_WARNING, "PDR: GetLocaleInfo failed." );
		RETURN_FALSE ;
    }

	RETURN_LONG((long)atoi(szCodePage)) ;
}

ZEND_FUNCTION(pdr_get_php_path)
{
    char psPHPPath[MAX_PATH] ;

	DWORD nPathLen = GetModuleFileName(NULL,psPHPPath,MAX_PATH) ;
	psPHPPath[nPathLen] = '\0' ;

	RETURN_STRING(psPHPPath,1) ;
}

 