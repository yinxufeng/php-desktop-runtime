#include "stdafx.h"
#include "CPDR.h"
#include "pdr_file.h"

ZEND_FUNCTION(pdr_file_create)
{
	char * psFilename ;
	int nFilenameLen=0 ;
	long nDesiredAccess=GENERIC_READ|GENERIC_WRITE ;
	long nCreationDisposition=OPEN_EXISTING ;
	long nShareMode=0 ;
	long nFlagsAndAttributes=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|llll", &psFilename, &nFilenameLen
			, &nDesiredAccess, &nCreationDisposition, &nShareMode, &nFlagsAndAttributes )==FAILURE )
	{
		RETURN_FALSE
	}

	pdr_file_handle * pFileHandle = new pdr_file_handle() ;
	pFileHandle->hFile = ::CreateFile(
			psFilename
			, nDesiredAccess
			, nShareMode
			, NULL
			, nCreationDisposition
			, nFlagsAndAttributes
			, NULL
	) ;

	if(pFileHandle->hFile==(HANDLE)-1)
	{
		delete pFileHandle ;
		RETURN_FALSE
	}
	else
	{
		int nResrc = _pdr_get_resrc_file() ;
		ZEND_REGISTER_RESOURCE( return_value, (void*)pFileHandle, nResrc )
	}
}


ZEND_FUNCTION(pdr_file_write)
{
	char * psData ;
	int nDataLen ;
	PDR_GetWin32FileHandleFromResrc("rs",__PDR_RESRC_MPARAM(&psData,&nDataLen))

	DWORD dwBytesWrite=0 ;
	if(!WriteFile(pFileHandle->hFile,psData,nDataLen,&dwBytesWrite,NULL)) 
	{
		RETURN_FALSE
	}
	PurgeComm(pFileHandle->hFile, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR) ;

	RETURN_LONG(dwBytesWrite)
}

ZEND_FUNCTION(pdr_file_read)
{
	int nReadLen=1024 ;
	PDR_GetWin32FileHandleFromResrc("r|l",__PDR_RESRC_MPARAM(&nReadLen))

	char * psData = new char[nReadLen+1] ;
	DWORD nCount;//读取的字节数
	if( !ReadFile(pFileHandle->hFile,psData,nReadLen,&nCount,NULL) )
	{
		RETURN_FALSE
	}

	psData[nCount]='\0';

	zval * pvRetString ;
	MAKE_STD_ZVAL(pvRetString)
	ZVAL_STRING(pvRetString,psData,1)
	delete [] psData ;

	RETURN_ZVAL(pvRetString,0,0) 
}

ZEND_FUNCTION(pdr_file_close)
{
	PDR_GetWin32FileHandleFromResrc("r",)

	if( !::CloseHandle(pFileHandle->hFile) )
	{
		RETURN_FALSE
	}

	else
	{
		pFileHandle->hFile = NULL ;
		RETURN_TRUE
	}

}


ZEND_FUNCTION(pdr_handle_write)
{
	char * psData ;
	int nDataLen ;
	long nHandle=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nHandle,&psData,&nDataLen )==FAILURE )
	{
		RETURN_FALSE
	}

	DWORD dwBytesWrite=0 ;
	if(!WriteFile((HANDLE)nHandle,psData,nDataLen,&dwBytesWrite,NULL)) 
	{
		set_last_error
		RETURN_FALSE
	}
	PurgeComm((HANDLE)nHandle, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR) ;

	RETURN_LONG(dwBytesWrite)
}

ZEND_FUNCTION(pdr_handle_read)
{
	long nHandle=0 ;
	int nReadLen=1024 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &nHandle,&nReadLen )==FAILURE )
	{
		RETURN_FALSE
	}

	char * psData = new char[nReadLen+1] ;
	DWORD nCount;//读取的字节数
	if( !ReadFile((HANDLE)nHandle,psData,nReadLen,&nCount,NULL) )
	{
		RETURN_FALSE
	}

	psData[nCount]='\0';

	zval * pvRetString ;
	MAKE_STD_ZVAL(pvRetString)
	ZVAL_STRING(pvRetString,psData,1)
	delete [] psData ;

	RETURN_ZVAL(pvRetString,0,0) 
}


ZEND_FUNCTION(pdr_handle_close)
{
	// 取得参数
	long nHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nHandle) == FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_BOOL(::CloseHandle((HANDLE)nHandle)) ;
}