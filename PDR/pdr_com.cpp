#include "stdafx.h"
#include "CPDR.h"
#include "pdr_file.h"

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

	pdr_file_handle * pFileHandle = new pdr_file_handle() ;
	pFileHandle->hFile = ::CreateFile( psComPort,nDesiredAccess,0,NULL,OPEN_EXISTING,bAsync?FILE_FLAG_OVERLAPPED:0,NULL ) ;

	if(pFileHandle->hFile==(HANDLE)-1)
	{
		RETURN_FALSE
	}
	else
	{
		int nResrc = _pdr_get_resrc_file() ;
		ZEND_REGISTER_RESOURCE( return_value, (void*)pFileHandle, nResrc )
	}
}

ZEND_FUNCTION(pdr_com_stat)
{
	int nBaudRate=9600, nByteSize=8, nParity=0, nStopBits=0 ;
	PDR_GetWin32FileHandleFromResrc("r|llll",__PDR_RESRC_MPARAM(&nBaudRate,&nByteSize,&nParity,&nStopBits))

	DCB dcb;
	//��ȡ�����豸״̬��Ϣ
	if (!GetCommState(pFileHandle->hFile, &dcb))
	{
		RETURN_FALSE
    }

	dcb.BaudRate = nBaudRate ;		// set the baud rate 9600
	dcb.ByteSize = nByteSize ;		// data size, xmit, and rcv
	dcb.Parity = nParity ;			// no parity bit 0
	dcb.StopBits = nStopBits ;		// one stop bit 0

	//���ô����豸״̬
	if (!SetCommState(pFileHandle->hFile, &dcb))
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
	PDR_GetWin32FileHandleFromResrc("r|lllll",__PDR_RESRC_MPARAM(
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
	RETURN_BOOL(SetCommTimeouts(pFileHandle->hFile,&comTimeOut))
}

ZEND_FUNCTION(pdr_com_setup_buffer)
{
	int nOutputBuffSize=1024, nInputBuffSize=1024 ;
	PDR_GetWin32FileHandleFromResrc("r|ll",__PDR_RESRC_MPARAM(&nInputBuffSize,&nOutputBuffSize))
		
	//�豸�����豸��д��������С
	RETURN_BOOL( SetupComm(pFileHandle->hFile,nInputBuffSize,nOutputBuffSize) )
}

ZEND_FUNCTION(pdr_com_write)
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
