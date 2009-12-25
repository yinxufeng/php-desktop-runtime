#include "stdafx.h"
#include "CPDR.h"
#include "pdr_proc_pipe.h"


ZEND_FUNCTION(pdr_pipe_create)
{
	long nSize = 10240 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &nSize )==FAILURE )
	{
		RETURN_FALSE
	}

	pdr_pipe_handle * pPipe = new pdr_pipe_handle() ;
	pPipe->nSize = nSize ;

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 


	if( !::CreatePipe( &pPipe->hRead, &pPipe->hWrite, &saAttr, pPipe->nSize ) )
	{
		delete pPipe ;
		RETURN_FALSE
	}
	else
	{
		int nResrc = _pdr_get_resrc_pipe() ;
		ZEND_REGISTER_RESOURCE( return_value, (void*)pPipe, nResrc )
	}
}

ZEND_FUNCTION(pdr_pipe_get_read_handle)
{
	PDR_GetPipeHandleFromResrc("r",)
	RETURN_LONG((long)pPipe->hRead)
}
ZEND_FUNCTION(pdr_pipe_get_write_handle)
{
	PDR_GetPipeHandleFromResrc("r",)
	RETURN_LONG((long)pPipe->hWrite)
}
ZEND_FUNCTION(pdr_pipe_get_size)
{
	PDR_GetPipeHandleFromResrc("r",)
	RETURN_LONG((long)pPipe->nSize)
}

ZEND_FUNCTION(pdr_pipe_peek)
{
	long nHandle=0,nSize = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &nHandle,&nSize )==FAILURE )
	{
		RETURN_FALSE
	}

	HANDLE hPipe = (HANDLE)nHandle ;
	char * pBuffer = NULL ;
	if(nSize)
	{
		pBuffer = new char[nSize] ;
	}

	DWORD nBytesRead, nTotalBytesAvail, nBytesLeftThisMessage ;
	
	if( !::PeekNamedPipe(hPipe,pBuffer,nSize,&nBytesRead,&nTotalBytesAvail,&nBytesLeftThisMessage) )
	{
		if(nSize)
		{
			delete []pBuffer ;
		}

		RETURN_FALSE
	}

	else
	{
		zval * pZvalData = NULL ;
		MAKE_STD_ZVAL(pZvalData) ;
		object_init(pZvalData);
		add_property_long(pZvalData, "nBytesRead", (long)nBytesRead) ;
		add_property_long(pZvalData, "nTotalBytesAvail", (long)nTotalBytesAvail) ;
		add_property_long(pZvalData, "nBytesLeftThisMessage", (long)nBytesLeftThisMessage) ;

		if(nSize)
		{
			add_property_string( pZvalData, "sBytes", pBuffer, 1 ) ;
			delete [] pBuffer ;
		}
		else
		{
			add_property_string( pZvalData, "sBytes", "", 1 ) ;
		}

		RETURN_ZVAL(pZvalData,0,0) ;
	}
}

ZEND_FUNCTION(pdr_pipe_get_std)
{
	long nHanleType = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nHanleType )==FAILURE )
	{
		RETURN_FALSE
	}
	if( nHanleType>STD_INPUT_HANDLE || nHanleType<STD_ERROR_HANDLE )
	{
		zend_error(E_WARNING, "PDR Pipe: you was given a avalid pipe handle type." );
		RETURN_FALSE
	}

	RETURN_LONG((long)::GetStdHandle(nHanleType))
}
ZEND_FUNCTION(pdr_pipe_set_std)
{
	long nHanleType = 0 ;
	long nHanle = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nHanleType, &nHanle )==FAILURE )
	{
		RETURN_FALSE
	}
	if( nHanleType>STD_INPUT_HANDLE || nHanleType<STD_ERROR_HANDLE )
	{
		zend_error(E_WARNING, "PDR Pipe: you was given a avalid pipe handle type." );
		RETURN_FALSE
	}

	RETURN_BOOL(::SetStdHandle(nHanleType,(HANDLE)nHanle))
}

// process //////////////////////////////////////////////

ZEND_FUNCTION(pdr_proc_create)
{
	long nReadPipeHandle=0,nWritePipeHandle=0, nErrorPipeHandle=0 ;
	char * psApplicationName ;
	long nApplicationNameLen = 0 ;
	char * psCommandLine ;
	long nCommandLineLen = 0 ;
	bool bInheritHandles = true ;
	long nCreateFlags = 0 ;
	char * psCurrentDir ;
	long nCurrentDirLen = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ssbllll"
			, &psApplicationName, &nApplicationNameLen
			, &psCommandLine, &nCommandLineLen
			, &psCurrentDir, &nCurrentDirLen
			, &bInheritHandles
			, &nReadPipeHandle,&nWritePipeHandle,&nErrorPipeHandle
			, &nCreateFlags
		)==FAILURE )
	{
		RETURN_FALSE
	}

	STARTUPINFO sui ;
	PROCESS_INFORMATION pi ;
	ZeroMemory(&sui,sizeof(STARTUPINFO)) ;
	sui.cb = sizeof(STARTUPINFO) ;
	sui.dwFlags = STARTF_USESTDHANDLES ;
	sui.hStdInput = nReadPipeHandle? (HANDLE)nReadPipeHandle: NULL ;
	sui.hStdOutput = nWritePipeHandle? (HANDLE)nWritePipeHandle: NULL ;
	sui.hStdError = nErrorPipeHandle? (HANDLE)nErrorPipeHandle: NULL ;

	if( !::CreateProcess(
		nApplicationNameLen? psApplicationName: NULL
		, nCommandLineLen? psCommandLine: NULL
		, NULL, NULL
		, bInheritHandles
		, nCreateFlags
		, NULL
		, nCurrentDirLen? psCurrentDir: NULL
		, &sui, &pi
	) )
	{
		RETURN_FALSE
	}

	else
	{
		pdr_proc_handle * pProc = new pdr_proc_handle() ;
		pProc->hProcess = pi.hProcess ;
		pProc->hThread = pi.hThread ;
		pProc->dwProcessId = pi.dwProcessId ;
		pProc->dwThreadId = pi.dwThreadId ;

		int nResrc = _pdr_get_resrc_proc() ;
		ZEND_REGISTER_RESOURCE( return_value, (void*)pProc, nResrc )
	}
}


ZEND_FUNCTION(pdr_proc_get_handle)
{
	PDR_GetProcHandleFromResrc("r",)
	RETURN_LONG((long)pProcess->hProcess)
}

ZEND_FUNCTION(pdr_proc_get_tread_handle)
{
	PDR_GetProcHandleFromResrc("r",)
	RETURN_LONG((long)pProcess->hThread)
}

ZEND_FUNCTION(pdr_proc_get_id)
{
	PDR_GetProcHandleFromResrc("r",)
	RETURN_LONG((long)pProcess->dwProcessId)
}

ZEND_FUNCTION(pdr_proc_get_tread_id)
{
	PDR_GetProcHandleFromResrc("r",)
	RETURN_LONG((long)pProcess->dwThreadId)
}

ZEND_FUNCTION(pdr_proc_terminate)
{
	long nProcessHandle ;
	long nExitCode=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &nProcessHandle,&nExitCode )==FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_BOOL(::TerminateProcess((HANDLE)nProcessHandle,nExitCode)) ;
}

ZEND_FUNCTION(pdr_proc_exit_code)
{
	long nProcessHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nProcessHandle )==FAILURE )
	{
		RETURN_FALSE
	}

	DWORD nExitCode ;
	if(::GetExitCodeProcess((HANDLE)nProcessHandle,&nExitCode))
	{
		RETURN_LONG(nExitCode) ;
	}

	else
	{
		RETURN_FALSE
	}
}