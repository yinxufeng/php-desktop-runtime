#include "stdafx.h"
#include "pdr_thread.h"
#include "CUIThread.h"
#include "CPDR.h"
//#include "php_embed.h"


UINT _pdr_work_thread(LPVOID pParam)
{
	zval * pRet ;
	MAKE_STD_ZVAL(pRet) ;

	void *** tsrm_ls = (void ***)pParam ;

	//php_embed_init(0,NULL PTSRMLS_CC) ;
	zend_eval_string("fopen('c:\\xxxxxx','a')",pRet,"XXXXXXXX" TSRMLS_CC) ;

	return 0 ;
}


ZEND_FUNCTION(pdr_thread_create_work)
{
	//::AfxBeginThread(_pdr_work_thread,tsrm_ls) ;
}

ZEND_FUNCTION(pdr_thread_create_window)
{
	bool bStartAtOnce = false ;
	long nPriority = 0 ;
	long nStackSize = 0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|bll", &bStartAtOnce, &nPriority, &nStackSize ) == FAILURE )
	{
		RETURN_FALSE
	}

	pdr_window_thread * pThreadRc = new pdr_window_thread() ;

	pThreadRc->pInitHandle = new pdr_window_thread_handle() ;
	pThreadRc->pInitHandle->pFunc = NULL ;
	pThreadRc->pInitHandle->pppArgs = NULL ;
	pThreadRc->pInitHandle->nArgc = 0 ;
	pThreadRc->pInitHandle->pFuncTable = EG(function_table) ;

	pThreadRc->pExitHandle = new pdr_window_thread_handle() ;
	pThreadRc->pExitHandle->pFunc = NULL ;
	pThreadRc->pExitHandle->pppArgs = NULL ;
	pThreadRc->pExitHandle->nArgc = 0 ;
	pThreadRc->pExitHandle->pFuncTable = EG(function_table) ;

	pThreadRc->pThread = ::AfxBeginThread(
			RUNTIME_CLASS(CUIThread)
			, (int)nPriority
			, (UINT)nStackSize
			, (DWORD)(bStartAtOnce?0:CREATE_SUSPENDED)
			, (LPSECURITY_ATTRIBUTES)NULL ) ;

	((CUIThread *)pThreadRc->pThread)->SetEventHandle(pThreadRc->pInitHandle,pThreadRc->pExitHandle) ;

	int nResrc = _pdr_get_resrc_thread_window() ;
	ZEND_REGISTER_RESOURCE( return_value, pThreadRc, nResrc )	
}

#define __pdr_get_resource_window_thread(type_spec,other_param) zval * hResrc = NULL ;\
	_pdr_window_thread *pThreadRc = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &hResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pThreadRc, _pdr_window_thread*, &hResrc, -1, resrc_name_pdr_thread_window, _pdr_get_resrc_thread_window()) ;\
	if(!pThreadRc)\
	{\
		zend_error(E_WARNING, "PDR Thread: you was given a avalid window thread resource handle." );\
		RETURN_FALSE\
	}\

ZEND_FUNCTION(pdr_thread_set_hwnd)
{
	/* ȡ��php�߳���Դ */
	long nWnd = 0 ;
	__pdr_get_resource_window_thread("rl",__PDR_RESRC_MPARAM(&nWnd))

	HWND hWnd = (HWND) nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR Thread: the sec paramater is not a valid window handle." );
		RETURN_FALSE
	}

	if( pThreadRc->pThread->m_pMainWnd )
	{
		delete pThreadRc->pThread->m_pMainWnd ;
	}
	pThreadRc->pThread->m_pMainWnd = CWnd::FromHandle(hWnd) ;
	RETURN_TRUE
}

ZEND_FUNCTION(pdr_thread_get_hwnd)
{
	/* ȡ��php�߳���Դ */
	__pdr_get_resource_window_thread("r",)

	if( !pThreadRc->pThread->m_pMainWnd )
	{
		RETURN_NULL()
	}

	RETURN_LONG((long)pThreadRc->pThread->m_pMainWnd->GetSafeHwnd())
}


#define __pdr_thread_set_handle(handle_type) 	\
	zval * pFunc = NULL ;\
	/* ȡ��php�߳���Դ */\
	__pdr_get_resource_window_thread("r|z",__PDR_RESRC_MPARAM(&pFunc))\
\
	/* ȡ�ø������*/\
	zval ***params;\
	int argc = ZEND_NUM_ARGS();\
	if (argc<2) {\
		WRONG_PARAM_COUNT;\
	}\
\
	params = (zval ***)safe_emalloc(sizeof(zval **), argc, 0);\
	if (zend_get_parameters_array_ex(2, params) == FAILURE)\
	{\
		efree(params);\
		RETURN_FALSE;\
	}\
\
	/* ����ת�����͵�string*/\
	if (Z_TYPE_PP(params[1]) != IS_STRING && Z_TYPE_PP(params[1]) != IS_ARRAY)\
	{\
		SEPARATE_ZVAL(params[1]);\
		convert_to_string_ex(params[1]);\
	}\
\
	/* ����������*/\
	char *name;\
	if (!__zend_is_callable(*params[1], &name))\
	{\
		php_error_docref1(NULL TSRMLS_CC, name, E_WARNING, "First argument is expected to be a valid callback");\
		efree(name);\
		RETURN_FALSE\
	}\
\
\
	pThreadRc->handle_type->pFunc = *params[1] ;\
	pThreadRc->handle_type->pppArgs = argc>2? (params+2): NULL ;\
	pThreadRc->handle_type->nArgc = argc-2;\
	/*pThreadRc->handle_type->tsrm_ls = tsrm_ls;*/\
\
	/* ���� �ص����� �� �ص����� �����ü��� */\
	pThreadRc->handle_type->pFunc->__refcount ++ ; \
	for(int i=0;i<pThreadRc->handle_type->nArgc;i++)\
	{\
		(*pThreadRc->handle_type->pppArgs[i])->__refcount ++ ; \
	}\
\
	/* ���� */\
	RETURN_TRUE\
\
\

ZEND_FUNCTION(pdr_thread_set_init_handle)
{
	//__pdr_thread_set_handle(pInitHandle)
}

ZEND_FUNCTION(pdr_thread_set_exit_handle)
{
	//__pdr_thread_set_handle(pExitHandle)
}


ZEND_FUNCTION(pdr_thread_resume)
{
	/* ȡ��php�߳���Դ */
	__pdr_get_resource_window_thread("r",)
	RETURN_LONG(pThreadRc->pThread->ResumeThread()) ;
}

ZEND_FUNCTION(pdr_thread_suspend)
{
	/* ȡ��php�߳���Դ */
	__pdr_get_resource_window_thread("r",)
	RETURN_LONG(pThreadRc->pThread->SuspendThread()) ;
}