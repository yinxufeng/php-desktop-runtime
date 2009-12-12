#include "stdafx.h"

#ifndef PDR_THREAD_HEADER
#define PDR_THREAD_HEADER 1


typedef struct _pdr_window_thread_handle
{
	zval * pFunc ;
	zval *** pppArgs ;
	int nArgc ;
	HashTable * pFuncTable ;
	void ***tsrm_ls ;

} pdr_window_thread_handle ;


typedef struct _pdr_window_thread
{
	CWinThread * pThread ;
	pdr_window_thread_handle * pInitHandle ;
	pdr_window_thread_handle * pExitHandle ;

} pdr_window_thread ;


#endif