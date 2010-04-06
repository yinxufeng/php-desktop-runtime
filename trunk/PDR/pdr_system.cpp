#include "stdafx.h"
#include "CPDR.h"
#include <Psapi.h>
#include "FileDialogEx.h"
#include "FolderDialog.h"
#include "ModulVer.h"
#include <Winver.h>


ZEND_FUNCTION(pdr_get_last_error)
{
	RETURN_LONG( _pdr_get_global_last_error() )
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



ZEND_FUNCTION(pdr_adjust_token_privileges)
{
	DWORD nPrivilegesId = TOKEN_QUERY|TOKEN_READ|TOKEN_ADJUST_PRIVILEGES ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &nPrivilegesId ) == FAILURE )
	{
		RETURN_FALSE
	}
	
	DWORD dwError ;
	HANDLE hToken=INVALID_HANDLE_VALUE;
	TOKEN_PRIVILEGES TokenPrivileges;

	if(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY|TOKEN_READ|TOKEN_ADJUST_PRIVILEGES,&hToken) == 0)
	{
		set_last_error
		RETURN_FALSE
	}
	TokenPrivileges.PrivilegeCount           = 1;
	TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED ;

	LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&TokenPrivileges.Privileges[0].Luid);
	dwError = GetLastError() ;
	AdjustTokenPrivileges(hToken,FALSE,&TokenPrivileges,sizeof(TOKEN_PRIVILEGES),NULL,NULL);
	dwError = GetLastError() ;
	CloseHandle(hToken);

	if(GetLastError() != ERROR_SUCCESS)
	{
		set_last_error
		RETURN_FALSE
	}
		
	RETURN_TRUE
}

ZEND_FUNCTION(pdr_get_process_filename)
{
	DWORD nProcessId = 0 ;
	bool bFullPath = true ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|b", &nProcessId, &bFullPath) == FAILURE )
	{
		RETURN_FALSE
	}

	HANDLE hProcess ;
	HMODULE hModule ;
	hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ , TRUE, nProcessId );
	if(!hProcess)
	{
		set_last_error

		zend_error(E_WARNING, "PDR: paramter 1 is not a valid process id." );
		RETURN_FALSE
	}

	DWORD cbNeeded ;
	if(!EnumProcessModules( hProcess, &hModule, sizeof(hModule), &cbNeeded))
	{
		set_last_error

		CloseHandle( hProcess );
		RETURN_FALSE
	}

	char psProcessName[MAX_PATH] = "unknown" ;
	BOOL bSuc = FALSE ;
	
	if(bFullPath)
	{
		bSuc = ::GetModuleFileNameEx( hProcess, hModule, psProcessName, MAX_PATH) ;
	}
	else
	{
		bSuc = ::GetModuleBaseName(hProcess, hModule, psProcessName, MAX_PATH) ;
	}

	if(!bSuc)
	{
		set_last_error

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



ZEND_FUNCTION(pdr_peek_msg)
{
	// 取得各项参数
	long nWnd=0, nMsgFilterMin=0, nMsgFilterMax=0, nRemove=PM_NOREMOVE ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|llll", &nWnd,&nMsgFilterMin,&nMsgFilterMax,&nRemove) == FAILURE )
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
	if( !::PeekMessage(&msg,hWnd,nMsgFilterMin,nMsgFilterMax,nRemove) )
	{
		RETURN_NULL()
	}

	zval * pZvalMsg = make_msg_object(&msg) ;

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

	zval * pZvalMsg = make_msg_object(&msg) ;
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

	MSG msg ;
	make_msg_from_object(pZvalMsg,&msg) ;

	RETURN_LONG((long)::DispatchMessage(&msg)) ;
}

ZEND_FUNCTION(pdr_translate_msg)
{
	// 取得参数
	zval * pZvalMsg ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &pZvalMsg) == FAILURE )
	{
		RETURN_FALSE
	}

	MSG msg ;
	make_msg_from_object(pZvalMsg,&msg) ;

	RETURN_BOOL(::TranslateMessage(&msg)) ;
}

ZEND_FUNCTION(pdr_create_mutex)
{
	// 取得参数
	char * psMutexName ;
	long nMutexNameLen = 0 ;
	bool bInitialOwner = false ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &psMutexName, &nMutexNameLen, &bInitialOwner) == FAILURE )
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

	if( nErr )
	{
		set_last_error
		RETURN_FALSE
	}

	else
	{
		RETURN_LONG((long)nHandle)
	}
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
	char * psDefExt, * psFileName, * psFilter, * psDlgTitle ;
	long nDefExtLen=0, nFileNameLen=0, nFilterLen=0, nFlags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, nParentWnd=0, nDlgTitleLen=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|bssslsl",
				&bOpenFileDialog 
				, &psFilter, &nFilterLen
				, &psFileName, &nFileNameLen
				, &psDlgTitle, &nDlgTitleLen
				, &nFlags
				, &psDefExt, &nDefExtLen
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

	CFileDialogEx aDlg(
		bOpenFileDialog
		, nDefExtLen? psDefExt: NULL
		, nFileNameLen? psFileName: NULL
		, nFlags
		, nFilterLen? psFilter: NULL
		, pParentWnd
		, 0
		, nDlgTitleLen? psDlgTitle: NULL
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

ZEND_FUNCTION(pdr_browse_folder2)
{
	char *psPath, * psTitle ;
	long nPathLen=0, nTitleLen=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ss",
				&psPath, &nPathLen
				, &psTitle, &nTitleLen) == FAILURE )
	{
		RETURN_FALSE
	}

	CString strRetPath ;
	CFolderDialog aFolderDlg(&strRetPath, nPathLen?(LPCTSTR)psPath:NULL, nTitleLen?(LPCTSTR)psTitle:NULL) ;
	if( aFolderDlg.DoModal()==IDOK )
	{
		RETURN_STRING((char*)(LPCTSTR)strRetPath,1)
	}
	else
	{
		RETURN_NULL()
	}
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


ZEND_FUNCTION(pdr_msgbox)
{
	char * psMsg ;
	long nMsgLen = 0 ;
	char * psTitle ;
	long nTitleLen = 0 ;
	long nType=MB_OK ;
	long nOwnerWnd = 0 ;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ssll", &psMsg, &nMsgLen, &psTitle, &nTitleLen, &nType, &nOwnerWnd )==FAILURE )
	{
		RETURN_FALSE
	}

	
	RETURN_LONG( ::MessageBox( (HWND)nOwnerWnd, nMsgLen?psMsg:"", nTitleLen?psTitle:"", nType ) ) ;
}

ZEND_FUNCTION(pdr_appbar_get_rect)
{
	APPBARDATA  appBarData ; 
	memset(&appBarData,0,sizeof(appBarData)); 
	//appBarData.cbSize  =  sizeof(appBarData); 
	//appBarData.hWnd  =  FindWindow("Shell_TrayWnd", NULL);

	SHAppBarMessage(ABM_QUERYPOS,&appBarData) ;
	
	zval * pRetRect ;
	MAKE_STD_ZVAL(pRetRect) ;
	array_init(pRetRect) ;

	add_index_long(pRetRect,0,appBarData.rc.left) ;
	add_index_long(pRetRect,1,appBarData.rc.top) ;
	add_index_long(pRetRect,2,appBarData.rc.right) ;
	add_index_long(pRetRect,3,appBarData.rc.bottom) ;

	RETURN_ZVAL(pRetRect,0,0)
}

ZEND_FUNCTION(pdr_var_ref_cnt)
{
	zval * pzVar ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &pzVar )==FAILURE )
	{
		RETURN_FALSE
	}

	// -1 ， 减去在当前栈中的引用计数
	RETURN_LONG( pzVar->__refcount - 1 ) 
}

ZEND_FUNCTION(pdr_get_module_handle)
{
	char * psPEPath ;
	int nPEPathLen ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &psPEPath, &nPEPathLen )==FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_LONG( (long)GetModuleHandle( nPEPathLen? (LPCTSTR)psPEPath: NULL ) );
}

ZEND_FUNCTION(pdr_load_library)
{
	char * psPEPath ;
	int nPEPathLen ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &psPEPath, &nPEPathLen )==FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_LONG( (long)::LoadLibrary((LPCSTR)psPEPath) )
}

ZEND_FUNCTION(pdr_free_library)
{
	long nModuleHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nModuleHandle )==FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_BOOL( ::FreeLibrary((HMODULE)nModuleHandle) )
}


ZEND_FUNCTION(pdr_exe_version_info)
{
	char * psFilePath, * psItemName ;
	int nFilePathLen, nItemNameLen ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &psFilePath, &nFilePathLen, &psItemName, &nItemNameLen )==FAILURE )
	{
		RETURN_FALSE
	}

	CModuleVersion aVersionInfo ;
	if( !aVersionInfo.GetFileVersionInfo((LPCTSTR)psFilePath) )
	{
		set_last_error
		RETURN_FALSE
	}

	CString strValue = aVersionInfo.GetValue(psItemName) ;
	
	RETURN_STRING((char *)(LPCTSTR)strValue,1)
}

ZEND_FUNCTION(pdr_get_os_version)
{
	RETURN_LONG((unsigned long)::GetVersion()) ;
}


ZEND_FUNCTION(pdr_get_static_property)
{
	char * psClassName, * psMemberName ;
	int nClassLen, nMemberLen ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &psClassName, &nClassLen, &psMemberName, &nMemberLen )==FAILURE )
	{
		RETURN_FALSE
	}

	
	/*_zend_executor_globals * ppp = ((zend_executor_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(executor_globals_id)]) ;
	Bucket * pBucket = ppp->class_table->pListTail ;
	zend_class_entry * pClass = NULL ;
	for(int i=ppp->class_table->nNumOfElements;i>0;i--)
	{
		if((unsigned int)pBucket->pData!=0xabababab)
		{
			// pClass = *((zend_class_entry **)pBucket->pData) ;
		}
		pBucket -- ;
	}*/
	//zval * pZ = *((zval **)(pClass->static_members->pListHead->pData)) ;

	// 找到 class entry
	zend_class_entry ** ppClass = NULL ;
	if( zend_hash_find(EG(class_table),psClassName,nClassLen+1,(void **)&ppClass)!=SUCCESS || !ppClass )
	{
		RETURN_NULL()
	}

	// 找到静态成员
	zval ** ppzvlProperty = NULL ;
	if( zend_hash_find((*ppClass)->static_members,psMemberName,nMemberLen+1,(void **)&ppzvlProperty)!=SUCCESS || !ppzvlProperty )
	{
		RETURN_NULL() ;
	}

	RETURN_ZVAL(*ppzvlProperty,1,0) ;
}


ZEND_FUNCTION(pdr_kill_var)
{
	zval * pzvVar ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &pzvVar )==FAILURE )
	{
		RETURN_FALSE
	}

	// 传入函数符号表中 保留一个引用
	// 这里接收参数后，分离处理， 保留一个引用
	// 这两个引用可以正常回收
	pzvVar->__refcount = 2 ;
}

ZEND_FUNCTION(pdr_debug_)
{
}

ZEND_FUNCTION(pdr_set_cursor_pos)
{
	long nX, nY ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nX, &nY )==FAILURE )
	{
		RETURN_FALSE
	}

	if(::SetCursorPos(nX,nY))
	{
		RETURN_TRUE ;
	}
	else
	{
		set_last_error ;
		RETURN_FALSE ;
	}
}
ZEND_FUNCTION(pdr_get_cursor_pos)
{
	zval * pzvRet ;
	MAKE_STD_ZVAL(pzvRet) ;
	array_init(pzvRet) ;

	POINT point ;
	if( !::GetCursorPos(&point) )
	{
		set_last_error ;
		RETURN_FALSE ;
	}

	else
	{
		add_index_long(pzvRet,0,point.x) ;
		add_index_long(pzvRet,1,point.y) ;
	}

	RETURN_ZVAL(pzvRet,1,0) ;
}