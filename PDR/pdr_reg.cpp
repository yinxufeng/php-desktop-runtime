#include "stdafx.h"

#define pdr_reg_key(value) (( HKEY ) (ULONG_PTR)((LONG)value) )


ZEND_FUNCTION(pdr_reg_create)
{
	long nKey ;
	char * psSubKey ;
	long nSubKeyLen = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nKey,&psSubKey,&nSubKeyLen )==FAILURE )
	{
		RETURN_FALSE
	}

	HKEY hKey ;
	if( ERROR_SUCCESS!=::RegCreateKey(pdr_reg_key(nKey),psSubKey,&hKey) )
	{
		RETURN_FALSE
	}

	RETURN_LONG((long)hKey) 
}
ZEND_FUNCTION(pdr_reg_open)
{
	long nKey ;
	char * psSubKey ;
	long nSubKeyLen = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nKey,&psSubKey,&nSubKeyLen )==FAILURE )
	{
		RETURN_FALSE
	}

	HKEY hKey ;
	if( ERROR_SUCCESS!=::RegOpenKey(pdr_reg_key(nKey),psSubKey,&hKey) )
	{
		RETURN_FALSE
	}

	RETURN_LONG((long)hKey) 
}
ZEND_FUNCTION(pdr_reg_set_string)
{
	long nKey ;
	char * psSubKey ;
	long nSubKeyLen = 0 ;
	char * psData ;
	long nDataLen = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lss", &nKey,&psSubKey,&nSubKeyLen,&psData,&nDataLen )==FAILURE )
	{
		RETURN_FALSE
	}
	RETURN_BOOL(
		ERROR_SUCCESS==RegSetValue(pdr_reg_key(nKey),psSubKey,REG_SZ,psData,nDataLen)
	) 
}
ZEND_FUNCTION(pdr_reg_set)
{
	long nKeyHandle ;
	char * psValueName ;
	long nValueNameLen = 0 ;
	char * psValue ;
	long nValueLen = 0 ;
	long nValueType = REG_SZ ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lss|l", &nKeyHandle,&psValueName,&nValueNameLen,&psValue,&nValueLen,&nValueType )==FAILURE )
	{
		RETURN_FALSE
	}

	// 32 整数
	if(	nValueType>=REG_DWORD && nValueType<=REG_DWORD_BIG_ENDIAN )
	{
		long nValue = atol(psValue) ;
		BYTE pData[4] ;
		pData[0] = (BYTE)(nValue & 0x000000FF) ;
		pData[1] = (BYTE)((nValue & 0x0000FF00)>>8) ;
		pData[2] = (BYTE)((nValue & 0x00FF0000)>>16) ;
		pData[3] = (BYTE)((nValue & 0xFF000000)>>24) ;
		
		RETURN_BOOL(
			ERROR_SUCCESS==RegSetValueEx(pdr_reg_key(nKeyHandle),psValueName,0,nValueType,pData,4)
		) 
	}

	// 以字符串格式
	else
	{
		RETURN_BOOL(
			ERROR_SUCCESS==RegSetValueEx(pdr_reg_key(nKeyHandle),psValueName,0,nValueType,(const BYTE *)psValue,nValueLen)
		) 
	}
}
ZEND_FUNCTION(pdr_reg_get_string)
{
	long nKey ;
	char * psSubKey ;
	long nSubKeyLen = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nKey,&psSubKey,&nSubKeyLen )==FAILURE )
	{
		RETURN_FALSE
	}

	long nStringLen = 0 ;
	if( RegQueryValue(pdr_reg_key(nKey),psSubKey,NULL,&nStringLen)!=ERROR_SUCCESS )
	{
		RETURN_FALSE
	}

	char * psData = new char[nStringLen] ;
	if( ::RegQueryValue(pdr_reg_key(nKey),psSubKey,psData,&nStringLen)!=ERROR_SUCCESS )
	{
		delete [] psData ;
		RETURN_FALSE
	}

	ZVAL_STRING(return_value,psData,1)
	delete [] psData ;
}
ZEND_FUNCTION(pdr_reg_get)
{
	long nKeyHandle ;
	char * psValueName ;
	long nValueNameLen = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nKeyHandle,&psValueName,&nValueNameLen )==FAILURE )
	{
		RETURN_FALSE
	}

	DWORD nValueType = REG_SZ ;
	DWORD nDataLen = 0 ;
	long br = RegQueryValueEx(pdr_reg_key(nKeyHandle),psValueName,0,&nValueType,NULL,&nDataLen) ;
	if( br!=ERROR_SUCCESS )
	{
		RETURN_FALSE
	}


	// 32 整数
	if(	nValueType>=REG_DWORD && nValueType<=REG_DWORD_BIG_ENDIAN )
	{
		BYTE * psData = new BYTE[nDataLen] ;
		if( ::RegQueryValueEx(pdr_reg_key(nKeyHandle),psValueName,0,&nValueType,(BYTE*)psData,&nDataLen)!=ERROR_SUCCESS )
		{
			delete [] psData ;
			RETURN_FALSE
		}

		long nRet = (long) (psData[0] | (psData[1]<<8) | (psData[2]<<16) | (psData[3]<<24)) ;
		delete [] psData ;

		RETURN_LONG( nRet ) ;

	}

	// 以字符串格式返回
	else
	{
		char * psData = new char[nDataLen+1] ;
		if( ::RegQueryValueEx(pdr_reg_key(nKeyHandle),psValueName,0,&nValueType,(BYTE*)psData,&nDataLen)!=ERROR_SUCCESS )
		{
			delete [] psData ;
			RETURN_FALSE
		}

		psData[nDataLen] = '\0' ;
		ZVAL_STRING(return_value,psData,1)
		delete [] psData ;
	}


}
ZEND_FUNCTION(pdr_reg_get_type)
{
	long nKeyHandle ;
	char * psValueName ;
	long nValueNameLen = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nKeyHandle,&psValueName,&nValueNameLen )==FAILURE )
	{
		RETURN_FALSE
	}

	DWORD nValueType = REG_SZ ;
	DWORD nDataLen = 0 ;
	if( !::RegQueryValueEx(pdr_reg_key(nKeyHandle),psValueName,0,&nValueType,NULL,&nDataLen) )
	{
		RETURN_FALSE
	}
	RETURN_LONG(nValueType)
}

ZEND_FUNCTION(pdr_reg_delete_key)
{
	long nKey ;
	char * psSubKey ;
	long nSubKeyLen = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nKey,&psSubKey,&nSubKeyLen )==FAILURE )
	{
		RETURN_FALSE
	}
	RETURN_BOOL(RegDeleteKey(pdr_reg_key(nKey),psSubKey)==ERROR_SUCCESS)
}

ZEND_FUNCTION(pdr_reg_delete_value)
{
	long nKeyHandle ;
	char * psValueName ;
	long nValueNameLen = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nKeyHandle,&psValueName,&nValueNameLen )==FAILURE )
	{
		RETURN_FALSE
	}
	RETURN_BOOL(RegDeleteValue(pdr_reg_key(nKeyHandle),psValueName)==ERROR_SUCCESS)
}

ZEND_FUNCTION(pdr_reg_close)
{
	long nKeyHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nKeyHandle )==FAILURE )
	{
		RETURN_FALSE
	}
	RETURN_BOOL(RegCloseKey(pdr_reg_key(nKeyHandle))==ERROR_SUCCESS)
}

ZEND_FUNCTION(pdr_reg_flush)
{
	long nKeyHandle ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nKeyHandle )==FAILURE )
	{
		RETURN_FALSE
	}
	RETURN_BOOL(RegFlushKey(pdr_reg_key(nKeyHandle))==ERROR_SUCCESS)
}
