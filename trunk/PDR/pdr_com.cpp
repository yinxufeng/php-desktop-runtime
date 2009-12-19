#include "stdafx.h"
#include "CPDR.h"

ZEND_FUNCTION(dhtml_com_open)
{
	char * psComPort ;
	int nComPort=0 ;
	long nDesiredAccess=GENERIC_READ|GENERIC_WRITE ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &psComPort, &nComPort, &nDesiredAccess )==FAILURE )
	{
		RETURN_FALSE
	}

	HANDLE hCom = ::CreateFile( psComPort,nDesiredAccess,0,NULL,OPEN_EXISTING,0,NULL ) ;

	if(hCom==(HANDLE)-1)
	{
		RETURN_FALSE
	}
	else
	{
		int nResrc = _pdr_get_resrc_com() ;
		ZEND_REGISTER_RESOURCE( return_value, (void*)(long)hCom, nResrc )
	}
}

// ȡ���Ѿ��򿪵� COM��� ��Դ
#define PDR_GetComHandleFromResrc(type_spec,other_param) zval * zvalResrc ;\
	void * pResrc = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &zvalResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pResrc, void*, &zvalResrc, -1, resrc_name_pdr_com, _pdr_get_resrc_com()) ;\
	if(!pResrc)\
	{\
		zend_error(E_WARNING, "PDR : you was given a avalid COM handle." );\
		RETURN_FALSE\
	}\
	HANDLE hCom = (HANDLE)(long)pResrc ;

ZEND_FUNCTION(dhtml_com_stat)
{
	int nBaudRate=9600, nByteSize=8, nParity=0, nStopBits=0 ;
	PDR_GetComHandleFromResrc("r|llll",__PDR_RESRC_MPARAM(&nBaudRate,&nByteSize,&nParity,&nStopBits))

	DCB dcb;
	//��ȡ�����豸״̬��Ϣ
	if (!GetCommState(hCom, &dcb))
	{
		RETURN_FALSE
    }

	dcb.BaudRate = nBaudRate ;		// set the baud rate 9600
	dcb.ByteSize = nByteSize ;		// data size, xmit, and rcv
	dcb.Parity = nParity ;			// no parity bit 0
	dcb.StopBits = nStopBits ;		// one stop bit 0

	//���ô����豸״̬
	if (!SetCommState(hCom, &dcb))
	{
		RETURN_FALSE
	}
	
	RETURN_TRUE
}

ZEND_FUNCTION(dhtml_com_set_timeouts)
{
	long nReadIntervalTimeout = 100 ;			// ����ʱ�����ַ�������ʱ��
	long nReadTotalTimeoutMultiplier = 100 ;	// ��ȡÿ�ֽڵĳ�ʱ
	long nReadTotalTimeoutConstant = 100 ;		// ���������ݵĹ̶���ʱ
	long nWriteTotalTimeoutMultiplier = 100 ;	// дÿ�ֽڵĳ�ʱ
	long nWriteTotalTimeoutConstant = 100 ;		// д�������ݵĹ̶���ʱ

	int nOutputBuffSize=1024, nInputBuffSize=1024 ;
	PDR_GetComHandleFromResrc("r|lllll",__PDR_RESRC_MPARAM(
			&nReadIntervalTimeout
			, &nReadTotalTimeoutMultiplier
			, &nReadTotalTimeoutConstant
			, &nWriteTotalTimeoutMultiplier
			, &nWriteTotalTimeoutConstant
	))
	
	COMMTIMEOUTS comTimeOut;
	comTimeOut.ReadIntervalTimeout = nReadIntervalTimeout ;
	comTimeOut.ReadTotalTimeoutMultiplier = nReadTotalTimeoutMultiplier ;
	comTimeOut.ReadTotalTimeoutConstant = nReadTotalTimeoutConstant ;
	comTimeOut.WriteTotalTimeoutMultiplier = nWriteTotalTimeoutMultiplier ;
	comTimeOut.WriteTotalTimeoutConstant = nWriteTotalTimeoutConstant ;

	//�豸�����豸��ʱʱ��
	RETURN_BOOL(SetCommTimeouts(hCom,&comTimeOut))
}

ZEND_FUNCTION(dhtml_com_setup_buffer)
{
	int nOutputBuffSize=1024, nInputBuffSize=1024 ;
	PDR_GetComHandleFromResrc("r|ll",__PDR_RESRC_MPARAM(&nInputBuffSize,&nOutputBuffSize))
		
	//�豸�����豸��д��������С
	RETURN_BOOL( SetupComm(hCom,nInputBuffSize,nOutputBuffSize) )
}

ZEND_FUNCTION(dhtml_com_write)
{
	char * psData ;
	int nDataLen ;
	PDR_GetComHandleFromResrc("rs",__PDR_RESRC_MPARAM(&psData,&nDataLen))

	DWORD dwBytesWrite=0 ;
	if(!WriteFile(hCom,psData,nDataLen,&dwBytesWrite,NULL)) 
	{
		RETURN_FALSE
	}
	PurgeComm(hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR) ;

	RETURN_LONG(dwBytesWrite)
}

ZEND_FUNCTION(dhtml_com_read)
{
	int nReadLen=1024 ;
	PDR_GetComHandleFromResrc("r|l",__PDR_RESRC_MPARAM(&nReadLen))

	char * psData = new char(nReadLen+1) ;
	DWORD nCount;//��ȡ���ֽ���
	if( !ReadFile(hCom,psData,nReadLen,&nCount,NULL) )
	{
		RETURN_FALSE
	}

	psData[nCount]='\0';

	zval * pvRetString ;
	MAKE_STD_ZVAL(pvRetString)
	ZVAL_STRING(pvRetString,psData,1)
	delete psData ;

	RETURN_ZVAL(pvRetString,0,0) 
}

ZEND_FUNCTION(dhtml_com_close)
{
	PDR_GetComHandleFromResrc("r")
	RETURN_BOOL(::CloseHandle(hCom)) ;
}
