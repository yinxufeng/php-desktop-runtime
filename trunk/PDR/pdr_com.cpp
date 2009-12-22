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
	//获取串口设备状态信息
	if (!GetCommState(pFileHandle->hFile, &dcb))
	{
		RETURN_FALSE
    }

	dcb.BaudRate = nBaudRate ;		// set the baud rate 9600
	dcb.ByteSize = nByteSize ;		// data size, xmit, and rcv
	dcb.Parity = nParity ;			// no parity bit 0
	dcb.StopBits = nStopBits ;		// one stop bit 0

	//设置串口设备状态
	if (!SetCommState(pFileHandle->hFile, &dcb))
	{
		RETURN_FALSE
	}
	
	RETURN_TRUE
}

ZEND_FUNCTION(pdr_com_set_timeouts)
{/*
  ReadIntervalTimeout:指定时间最大值（毫秒），充许接收的2个字节间有时间差。也就  
  是说，刚接收了一个字节后，等了ReadIntervalTimeout时间后还没有新的字节到达，就  
  认为本次读串口操作结束（后面的字节等下一次读取操作来处理）。即使你想读8个字节，  
  但读第2个字节后，过了ReadIntervalTimeout时间后，第3个字节还没到。实际上就只读  
  了2个字节。  
   
  ReadTotalTimeoutMultiplier:指定比例因子（毫秒），实际上是设置读取一个字节和等  
  待下一个字节所需的时间，这样总的超时时间为读取的字节数乘以该值，同样一次读取  
  操作到达这个时间后，也认为本次读操作己经结束。  
   
  ReadTotalTimeoutConstant:可以理解为一个修正时间，实际上就是按ReadTotalTimeout  
  Multiplier计算出的超时时间再加上该时间才作为整个超时时间。  
   
  WriteTotalTimeoutMultiplier，WriteTotalTimeoutConstant   意思类似。  
  如果设为0值表示这些参数不起作用。  
   
  这样我们很容易看出，如果我们想读写可靠，要根据  
  不同的速率和应用实际情况设置合适的值。和同步模式和异步模式、不同的通信协议无关。  
   
  重点在于理解:超时就意味着本次读写操作结束。  
   
  常见设置:  
  一般都会做以下设置：  
  TimeOuts.ReadIntervalTimeout=MAXDWORD;                        
  //   把间隔超时设为最大，把总超时设为0将导致ReadFile立即返回并完成操作  
   
  TimeOuts.ReadTotalTimeoutMultiplier=0;                        
  //读时间系数  
   
  TimeOuts.ReadTotalTimeoutConstant=0;                          
  //读时间常量      
   
  TimeOuts.WriteTotalTimeoutMultiplier=50;                
  //总超时=时间系数*要求读/写的字符数+时间常量  
   
  TimeOuts.WriteTotalTimeoutConstant=2000;                
  //设置写超时以指定WriteComm成员函数中的   
 
 */

	long nReadIntervalTimeout = 0 ;						// 接收时，两字符间最大的时延
	/*(妙)*/long nReadTotalTimeoutMultiplier = 0 ;		// 读取每字节的超时(秒)
	/*(微妙)*/long nReadTotalTimeoutConstant = 0 ;		// 读串口数据的固定超时

	long nWriteTotalTimeoutMultiplier = 0 ;				// 写每字节的超时
	long nWriteTotalTimeoutConstant = 0 ;				// 写串口数据的固定超时

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

	//设备串口设备超时时限
	RETURN_BOOL(SetCommTimeouts(pFileHandle->hFile,&comTimeOut))
}

ZEND_FUNCTION(pdr_com_setup_buffer)
{
	int nOutputBuffSize=1024, nInputBuffSize=1024 ;
	PDR_GetWin32FileHandleFromResrc("r|ll",__PDR_RESRC_MPARAM(&nInputBuffSize,&nOutputBuffSize))
		
	//设备串口设备读写缓冲区大小
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
