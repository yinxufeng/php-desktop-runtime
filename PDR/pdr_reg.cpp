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
	if( !::RegCreateKey(pdr_reg_key(nKey),psSubKey,&hKey) )
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
	if( !::RegOpenKey(pdr_reg_key(nKey),psSubKey,&hKey) )
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
	RETURN_BOOL(RegSetValue(pdr_reg_key(nKey),psSubKey,REG_SZ,psData,nDataLen)) 
}
ZEND_FUNCTION(pdr_reg_set)
{
	long nKeyHandle ;
	char * psValueName ;
	long nValueNameLen = 0 ;
	char * psData ;
	long nDataLen = 0 ;
	long nValueType = REG_SZ ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lss|l", &nKeyHandle,&psValueName,&nValueNameLen,&psData,&nDataLen,&nValueType )==FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_BOOL(RegSetValueEx(pdr_reg_key(nKeyHandle),psValueName,0,nValueType,(const BYTE *)psData,nDataLen)) 
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
	if( !::RegQueryValue(pdr_reg_key(nKey),psSubKey,NULL,&nStringLen) )
	{
		RETURN_FALSE
	}

	char * psData = new char[nStringLen] ;
	if( !::RegQueryValue(pdr_reg_key(nKey),psSubKey,psData,&nStringLen) )
	{
		delete [] psData ;
		RETURN_FALSE
	}

	zval * pzvRet ;
	MAKE_STD_ZVAL(pzvRet)
	ZVAL_STRING(pzvRet,psData,1)
	delete [] psData ;

	RETURN_ZVAL(pzvRet,0,0)
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
	if( !::RegQueryValueEx(pdr_reg_key(nKeyHandle),psValueName,0,&nValueType,NULL,&nDataLen) )
	{
		RETURN_FALSE
	}

	char * psData = new char[nDataLen] ;
	if( !::RegQueryValueEx(pdr_reg_key(nKeyHandle),psValueName,0,&nValueType,(BYTE*)psData,&nDataLen) )
	{
		delete [] psData ;
		RETURN_FALSE
	}

	zval * pzvRet ;
	MAKE_STD_ZVAL(pzvRet)
	ZVAL_STRING(pzvRet,psData,1)
	delete [] psData ;

	RETURN_ZVAL(pzvRet,0,0)
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

ZEND_FUNCTION(pdr_reg_delete)
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
