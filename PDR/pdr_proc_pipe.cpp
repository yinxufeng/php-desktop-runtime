#include "stdafx.h"
#include "CPDR.h"
#include "pdr_proc_pipe.h"


ZEND_FUNCTION(pdr_pipe_create)
{
	long nSize = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &nSize )==FAILURE )
	{
		RETURN_FALSE
	}

	pdr_pipe_handle * pPipe = new pdr_pipe_handle() ;

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 


	if( ::CreatePipe( &pPipe->hRead, &pPipe->hWrite, &saAttr, nSize ) )
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

ZEND_FUNCTION(pdr_pipe_peek)
{
	long nHandle=0,nSize = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &nSize )==FAILURE )
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