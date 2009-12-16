// UIThread.cpp : 实现文件
//

#include "stdafx.h"
#include "CPDR.h"
#include "CUIThread.h"


// CUIThread

IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

CUIThread::CUIThread()
	: m_pInitHandle(NULL)
	, m_pExitHandle(NULL)
{
	m_pMainWnd = NULL ;
}

CUIThread::~CUIThread()
{}

BOOL CUIThread::InitInstance()
{
	TSRMLS_FETCH() ;

	// php 回调函数
	if( this->m_pInitHandle->pFunc )
	{
		zval *pRet ;
		int r = call_user_function_ex(
				this->m_pInitHandle->pFuncTable
				, NULL
				, this->m_pInitHandle->pFunc
				, &pRet
				, this->m_pInitHandle->nArgc, this->m_pInitHandle->pppArgs, 0, NULL 
				TSRMLS_CC
		) ;

		// 将 php 回调函数返回值作为 线程主窗口
		if( pRet && Z_TYPE_P(pRet)==IS_LONG)
		{
			HWND hWnd = (HWND)Z_LVAL_P(pRet) ;
			if( ::IsWindow(hWnd) )
			{
				if( m_pMainWnd )
				{
					delete m_pMainWnd ;
				}

				m_pMainWnd = CWnd::FromHandle(hWnd) ;
			}
		}
	}

	return TRUE;
}

int CUIThread::ExitInstance()
{
	TSRMLS_FETCH() ;

	int nRetCode = 0 ;

	// php 回调函数
	if( this->m_pExitHandle->pFunc )
	{
		zval *pRet ;
		int r = call_user_function_ex(
			this->m_pExitHandle->pFuncTable
			, NULL
			, this->m_pExitHandle->pFunc
			, &pRet
			, this->m_pExitHandle->nArgc, this->m_pExitHandle->pppArgs, 0, NULL 
			TSRMLS_CC
		) ;

		// 将 php 回调函数返回值作为 线程主窗口
		if( pRet && Z_TYPE_P(pRet)==IS_LONG)
		{
			nRetCode = (int)Z_LVAL_P(pRet) ;
		}
	}

	return nRetCode ;
}

BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
END_MESSAGE_MAP()


// CUIThread 消息处理程序
