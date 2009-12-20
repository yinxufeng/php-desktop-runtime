#include "stdafx.h"
#include "CPDR.h"
#include "pdr_com.h"

ZEND_FUNCTION(pdr_com_open)
{
	char * psComPort ;
	int nComPort=0 ;
	long nDesiredAccess=GENERIC_READ|GENERIC_WRITE ;
	bool bAsync = false ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|bl", &psComPort, &nComPort, &nDesiredAccess, &bAsync )==FAILURE )
	{
		RETURN_FALSE
	}

	pdr_com_handle * pComHandle = new pdr_com_handle() ;
	pComHandle->hCom = ::CreateFile( psComPort,nDesiredAccess,0,NULL,OPEN_EXISTING,bAsync?FILE_FLAG_OVERLAPPED:0,NULL ) ;

	if(pComHandle->hCom==(HANDLE)-1)
	{
		RETURN_FALSE
	}
	else
	{
		int nResrc = _pdr_get_resrc_com() ;
		ZEND_REGISTER_RESOURCE( return_value, (void*)pComHandle, nResrc )
	}
}

// ȡ���Ѿ��򿪵� COM��� ��Դ
#define PDR_GetComHandleFromResrc(type_spec,other_param) zval * zvalResrc ;\
	pdr_com_handle * pComHandle = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &zvalResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pComHandle, pdr_com_handle*, &zvalResrc, -1, resrc_name_pdr_com, _pdr_get_resrc_com()) ;\
	if(!pComHandle || !pComHandle->hCom )\
	{\
		zend_error(E_WARNING, "PDR : you was given a avalid COM handle." );\
		RETURN_FALSE\
	}

ZEND_FUNCTION(pdr_com_stat)
{
	int nBaudRate=9600, nByteSize=8, nParity=0, nStopBits=0 ;
	PDR_GetComHandleFromResrc("r|llll",__PDR_RESRC_MPARAM(&nBaudRate,&nByteSize,&nParity,&nStopBits))

	DCB dcb;
	//��ȡ�����豸״̬��Ϣ
	if (!GetCommState(pComHandle->hCom, &dcb))
	{
		RETURN_FALSE
    }

	dcb.BaudRate = nBaudRate ;		// set the baud rate 9600
	dcb.ByteSize = nByteSize ;		// data size, xmit, and rcv
	dcb.Parity = nParity ;			// no parity bit 0
	dcb.StopBits = nStopBits ;		// one stop bit 0

	//���ô����豸״̬
	if (!SetCommState(pComHandle->hCom, &dcb))
	{
		RETURN_FALSE
	}
	
	RETURN_TRUE
}

ZEND_FUNCTION(pdr_com_set_timeouts)
{/*
  ReadIntervalTimeout:ָ��ʱ�����ֵ�����룩��������յ�2���ֽڼ���ʱ��Ҳ��  
  ��˵���ս�����һ���ֽں󣬵���ReadIntervalTimeoutʱ���û���µ��ֽڵ����  
  ��Ϊ���ζ����ڲ���������������ֽڵ���һ�ζ�ȡ��������������ʹ�����8���ֽڣ�  
  ������2���ֽں󣬹���ReadIntervalTimeoutʱ��󣬵�3���ֽڻ�û����ʵ���Ͼ�ֻ��  
  ��2���ֽڡ�  
   
  ReadTotalTimeoutMultiplier:ָ���������ӣ����룩��ʵ���������ö�ȡһ���ֽں͵�  
  ����һ���ֽ������ʱ�䣬�����ܵĳ�ʱʱ��Ϊ��ȡ���ֽ������Ը�ֵ��ͬ��һ�ζ�ȡ  
  �����������ʱ���Ҳ��Ϊ���ζ���������������  
   
  ReadTotalTimeoutConstant:�������Ϊһ������ʱ�䣬ʵ���Ͼ��ǰ�ReadTotalTimeout  
  Multiplier������ĳ�ʱʱ���ټ��ϸ�ʱ�����Ϊ������ʱʱ�䡣  
   
  WriteTotalTimeoutMultiplier��WriteTotalTimeoutConstant   ��˼���ơ�  
  �����Ϊ0ֵ��ʾ��Щ�����������á�  
   
  �������Ǻ����׿���������������д�ɿ���Ҫ����  
  ��ͬ�����ʺ�Ӧ��ʵ��������ú��ʵ�ֵ����ͬ��ģʽ���첽ģʽ����ͬ��ͨ��Э���޹ء�  
   
  �ص��������:��ʱ����ζ�ű��ζ�д����������  
   
  ��������:  
  һ�㶼�����������ã�  
  TimeOuts.ReadIntervalTimeout=MAXDWORD;                        
  //   �Ѽ����ʱ��Ϊ��󣬰��ܳ�ʱ��Ϊ0������ReadFile�������ز���ɲ���  
   
  TimeOuts.ReadTotalTimeoutMultiplier=0;                        
  //��ʱ��ϵ��  
   
  TimeOuts.ReadTotalTimeoutConstant=0;                          
  //��ʱ�䳣��      
   
  TimeOuts.WriteTotalTimeoutMultiplier=50;                
  //�ܳ�ʱ=ʱ��ϵ��*Ҫ���/д���ַ���+ʱ�䳣��  
   
  TimeOuts.WriteTotalTimeoutConstant=2000;                
  //����д��ʱ��ָ��WriteComm��Ա�����е�   
 
 */

	long nReadIntervalTimeout = 0 ;						// ����ʱ�����ַ�������ʱ��
	/*(��)*/long nReadTotalTimeoutMultiplier = 0 ;		// ��ȡÿ�ֽڵĳ�ʱ(��)
	/*(΢��)*/long nReadTotalTimeoutConstant = 0 ;		// ���������ݵĹ̶���ʱ

	long nWriteTotalTimeoutMultiplier = 0 ;				// дÿ�ֽڵĳ�ʱ
	long nWriteTotalTimeoutConstant = 0 ;				// д�������ݵĹ̶���ʱ

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
	RETURN_BOOL(SetCommTimeouts(pComHandle->hCom,&comTimeOut))
}

ZEND_FUNCTION(pdr_com_setup_buffer)
{
	int nOutputBuffSize=1024, nInputBuffSize=1024 ;
	PDR_GetComHandleFromResrc("r|ll",__PDR_RESRC_MPARAM(&nInputBuffSize,&nOutputBuffSize))
		
	//�豸�����豸��д��������С
	RETURN_BOOL( SetupComm(pComHandle->hCom,nInputBuffSize,nOutputBuffSize) )
}

ZEND_FUNCTION(pdr_com_write)
{
	char * psData ;
	int nDataLen ;
	PDR_GetComHandleFromResrc("rs",__PDR_RESRC_MPARAM(&psData,&nDataLen))

	DWORD dwBytesWrite=0 ;
	if(!WriteFile(pComHandle->hCom,psData,nDataLen,&dwBytesWrite,NULL)) 
	{
		RETURN_FALSE
	}
	PurgeComm(pComHandle->hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR) ;

	RETURN_LONG(dwBytesWrite)
}

ZEND_FUNCTION(pdr_com_read)
{
	int nReadLen=1024 ;
	PDR_GetComHandleFromResrc("r|l",__PDR_RESRC_MPARAM(&nReadLen))

	char * psData = new char[nReadLen+1] ;
	DWORD nCount;//��ȡ���ֽ���
	if( !ReadFile(pComHandle->hCom,psData,nReadLen,&nCount,NULL) )
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

ZEND_FUNCTION(pdr_com_close)
{
	PDR_GetComHandleFromResrc("r",)

	if( !::CloseHandle(pComHandle->hCom) )
	{
		RETURN_FALSE
	}

	else
	{
		pComHandle->hCom = NULL ;
		RETURN_TRUE
	}

}
