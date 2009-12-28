#include "stdafx.h"
#include "SAPI.h"



class pdr_thread_resrc
{
public :
	pdr_thread_resrc() ;
	~ pdr_thread_resrc() ;

	char psEntrancePath [MAX_PATH] ;
	char * psEntranceFunctionName ;

	DWORD nThreadId ;
	HANDLE nThreadHandle ;

	sapi_module_struct php_module ;
	// void ***tsrm_ls ;
} ;

DWORD WINAPI pdr_thread_callback(LPVOID pParam) ;



