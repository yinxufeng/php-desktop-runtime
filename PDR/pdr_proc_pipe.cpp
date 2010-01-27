#include "stdafx.h"
#include "CPDR.h"
#include "pdr_proc_pipe.h"
#include <Psapi.h>


ZEND_FUNCTION(pdr_pipe_create)
{
	long nReadHandle=0, nWriteHandle=0  ;
	long nSize = 10240 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|lll", &nReadHandle,&nWriteHandle,&nSize )==FAILURE )
	{
		RETURN_FALSE
	}

	pdr_pipe_handle * pPipe = new pdr_pipe_handle() ;
	if(!nReadHandle)
	{
		pPipe->hRead = (HANDLE)nReadHandle ;
	}
	if(!nWriteHandle)
	{
		pPipe->hWrite = (HANDLE)nWriteHandle ;
	}
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

	DWORD nExitCode = 0 ;
	if(::GetExitCodeProcess((HANDLE)nProcessHandle,&nExitCode))
	{
		RETURN_LONG(nExitCode) ;
	}

	else
	{
		RETURN_FALSE
	}
}


DWORD GetCmdLine(HANDLE hProc,TCHAR* pCmdLine,DWORD dwBufLen) 
{
	#define BUFFER_LEN    512        //reading buffer for the commandline
     
    DWORD dwRet = -1; 
    DWORD dwAddr = *(DWORD*)((DWORD)GetCommandLine + 1);//��2���ֽڿ�ʼ��������Ҫ���ĵ�ַ
    TCHAR tcBuf[BUFFER_LEN] = {0}; 
    DWORD dwRead = 0; 
     
    //�ж�ƽ̨
    DWORD dwVer = GetVersion(); 
    try
    { 
        if(dwVer < 0x80000000)        // Windows NT/2000/XP
        { 
            if(ReadProcessMemory(hProc,(LPVOID)dwAddr,&dwAddr,4,&dwRead)) 
            { 
                if(ReadProcessMemory(hProc,(LPVOID)dwAddr,tcBuf,BUFFER_LEN,&dwRead)) 
                { 
                    _tcsncpy(pCmdLine,tcBuf,dwBufLen);    //��ü��һ��dwRead��dwBufLen�Ĵ�С��ʹ�ý�С���Ǹ�
                    dwRet = 0; 
                }
            }
        }
        else                        // Windows 95/98/Me    and Win32s
        {
            while(true)                //ʹ��while��Ϊ�˳���ʱ��������ѭ��
            {
                if(!ReadProcessMemory(hProc,(LPVOID)dwAddr,&dwAddr,4,&dwRead)) break; 
                if(!ReadProcessMemory(hProc,(LPVOID)dwAddr,&dwAddr,4,&dwRead)) break; 

                if(!ReadProcessMemory(hProc,(LPVOID)(dwAddr + 0xC0),tcBuf,BUFFER_LEN,&dwRead)) break; 
                if(*tcBuf == 0) 
                { 
                    if(!ReadProcessMemory(hProc,(LPVOID)(dwAddr + 0x40),&dwAddr,4,&dwRead)) break; 
                    if(!ReadProcessMemory(hProc,(LPVOID)(dwAddr + 0x8),&dwAddr,4,&dwRead)) break; 
                    if(!ReadProcessMemory(hProc,(LPVOID)dwAddr,tcBuf,BUFFER_LEN,&dwRead)) break; 
                } 
                 
                _tcsncpy(pCmdLine,tcBuf,dwBufLen);    //��ü��һ��dwRead��dwBufLen�Ĵ�С��ʹ�ý�С���Ǹ�
                dwRet = 0; 
                break; 
            } 
        } 
    } 
    catch(...) 
    { 
        dwRet = ERROR_INVALID_ACCESS;    //exception
    }
     
	DWORD dwErr = ::GetLastError() ;
    return dwRet; 
}

ZEND_FUNCTION(pdr_proc_enum)
{
	long nReturnType = PDR_ENUM_PROCS_ID ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &nReturnType )==FAILURE )
	{
		RETURN_FALSE
	}

	DWORD pdwProcesses[1024], dwNeeded;
	if( !EnumProcesses(pdwProcesses,sizeof(pdwProcesses),&dwNeeded) )
	{
		set_last_error
		RETURN_NULL() ;
	}

	zval * pzvRet ;
	MAKE_STD_ZVAL(pzvRet) ;
	array_init(pzvRet) ;

	DWORD nProcessCount = dwNeeded / sizeof(DWORD);
	for(unsigned int i=0;i<nProcessCount;i++)
	{
		zval * pzvProc ;
		MAKE_STD_ZVAL(pzvProc) ;
		array_init(pzvProc) ;

		// �������id
		if(nReturnType&PDR_ENUM_PROCS_ID)
		{
			add_assoc_long(pzvProc,"id",pdwProcesses[i]) ;
		}

		// �򿪽��̣�ȡ�ý��̸���ϸ������
		if(nReturnType>PDR_ENUM_PROCS_ID)
		{
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pdwProcesses[i]) ;
			if (hProcess)
			{
				HMODULE hModule;
				DWORD nModuleNeeded;

				// ö�ٵ�ǰ���̵��õ�����ģ��
				if ( EnumProcessModules(hProcess, &hModule, sizeof(hModule), &nModuleNeeded) )
				{
					// ��ò�������̵�����
					if(nReturnType&PDR_ENUM_PROCS_FILENAME)
					{
						char szName[MAX_PATH] ;
						GetModuleBaseName(hProcess, hModule, szName, sizeof(szName));
						add_assoc_string(pzvProc,"filename",szName,1) ;
					}

					// ��ò�������̵�����
					if(nReturnType&PDR_ENUM_PROCS_FULLPATH)
					{
						char szName[MAX_PATH] ;
						GetModuleFileNameEx(hProcess, hModule, szName, sizeof(szName));
						add_assoc_string(pzvProc,"path",szName,1) ;
					}
					
					// ��ò�������̵�����
					/*if(nReturnType&PDR_ENUM_PROCS_CL)
					{
						char szCl[MAX_PATH] ;
						GetCmdLine(hProcess,szCl,MAX_PATH) ;
						add_assoc_string(pzvProc,"cl",szCl,1) ;
					}*/
				}
				else
				{
					set_last_error
				}

				CloseHandle(hProcess);
			}
			else
			{
				set_last_error
			}
		}

		pzvProc->refcount__gc -- ;
		add_index_zval(pzvRet,i,pzvProc) ;
	}

	pzvRet->refcount__gc -- ;
	RETURN_ZVAL(pzvRet,1,0) ;
}