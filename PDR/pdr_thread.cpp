#include "stdafx.h"
#include "pdr_thread.h"
#include "php_main.h"
#include "php_variables.h"
#include "CPDR.h"



static const zend_function_entry additional_functions[] = {
	{NULL, NULL, NULL}
};


const char HARDCODED_INI[] =
	"html_errors=0\n"
	"register_argc_argv=0\n"
	"implicit_flush=1\n"
	"output_buffering=0\n"
	"max_execution_time=0\n"
	"max_input_time=-1\n\0";


static char* php_embed_read_cookies(TSRMLS_D)
{
	return NULL;
}

static int php_embed_deactivate(TSRMLS_D)
{
	fflush(stdout);
	return SUCCESS;
}

static inline size_t php_embed_single_write(const char *str, uint str_length)
{
#ifdef PHP_WRITE_STDOUT
	long ret;

	ret = write(STDOUT_FILENO, str, str_length);
	if (ret <= 0) return 0;
	return ret;
#else
	size_t ret;

	ret = fwrite(str, 1, MIN(str_length, 16384), stdout);
	return ret;
#endif
}


static int php_embed_ub_write(const char *str, uint str_length TSRMLS_DC)
{
	const char *ptr = str;
	uint remaining = str_length;
	size_t ret;

	while (remaining > 0) {
		ret = php_embed_single_write(ptr, remaining);
		if (!ret) {
			php_handle_aborted_connection();
		}
		ptr += ret;
		remaining -= ret;
	}

	return str_length;
}

static void php_embed_flush(void *server_context)
{
	if (fflush(stdout)==EOF) {
		php_handle_aborted_connection();
	}
}

static void php_embed_send_header(sapi_header_struct *sapi_header, void *server_context TSRMLS_DC)
{
}

static void php_embed_log_message(char *message)
{
	fprintf (stderr, "%s\n", message);
}

static void php_embed_register_variables(zval *track_vars_array TSRMLS_DC)
{
	php_import_environment_variables(track_vars_array TSRMLS_CC);
}

static int php_embed_startup(sapi_module_struct *sapi_module)
{
	if (php_module_startup(sapi_module, NULL, 0)==FAILURE) {
		return FAILURE;
	}
	return SUCCESS;
}

pdr_thread_resrc::pdr_thread_resrc()
{
	psEntranceFunctionName = NULL ;
	nThreadId = 0 ;
	nThreadHandle = NULL ;


	////////////////////////////////////////
	php_module.name = "embed" ;
	php_module.pretty_name = "PHP Embedded Library" ;
	php_module.startup = php_embed_startup ;
	php_module.shutdown = php_module_shutdown_wrapper ;

	php_module.activate = NULL ;
	php_module.deactivate = php_embed_deactivate ;
	
	php_module.ub_write = php_embed_ub_write ;
	php_module.flush = php_embed_flush ;
	php_module.get_stat = NULL ;
	php_module.getenv = NULL ;

	php_module.sapi_error = php_error ;

	php_module.header_handler = NULL ;
	php_module.send_headers = NULL ;
	php_module.send_header = php_embed_send_header ;

	php_module.read_post = NULL ;
	php_module.read_cookies = php_embed_read_cookies ;
	
	php_module.register_server_variables = php_embed_register_variables ;
	php_module.log_message = php_embed_log_message ;
	php_module.get_request_time = NULL ;
	
#ifdef PHP53
	php_module.terminate_process = NULL ;
#endif

	//////////////////////////
	// tsrm_ls = NULL ;

}
pdr_thread_resrc::~ pdr_thread_resrc()
{
	if(psEntranceFunctionName)
	{
		delete psEntranceFunctionName ;
	}
}


DWORD WINAPI pdr_thread_callback(LPVOID pParam)
{
	::AfxMessageBox("in pdr_thread_callback()") ;

	//int argc = 0 ;
	//char **argv = NULL ;

	pdr_thread_resrc * pThreadResrc = (pdr_thread_resrc*) pParam ;

	
//#ifdef ignore
	//////////////////////////////////////////////////////////////
	zend_llist global_vars;
	#ifdef ZTS
		void ***tsrm_ls = NULL;
	#endif

	#ifdef HAVE_SIGNAL_H
	#if defined(SIGPIPE) && defined(SIG_IGN)
		signal(SIGPIPE, SIG_IGN); /* ignore SIGPIPE in standalone mode so
									 that sockets created via fsockopen()
									 don't kill PHP if the remote site
									 closes it.  in apache|apxs mode apache
									 does that for us!  thies@thieso.net
									 20000419 */
	#endif
	#endif

	#ifdef ZTS
	tsrm_startup(1, 1, 0, NULL);
	tsrm_ls = (void ***)ts_resource(0);
  //*ptsrm_ls = tsrm_ls;
	#endif

  sapi_startup(&pThreadResrc->php_module);

#ifdef PHP_WIN32
 // _fmode = _O_BINARY;			/*sets default for file streams to binary */
 // setmode(_fileno(stdin), O_BINARY);		/* make the stdio mode be binary */
 // setmode(_fileno(stdout), O_BINARY);		/* make the stdio mode be binary */
 // setmode(_fileno(stderr), O_BINARY);		/* make the stdio mode be binary */
#endif

pThreadResrc->php_module.ini_entries = (char *)malloc(sizeof(HARDCODED_INI));
memcpy(pThreadResrc->php_module.ini_entries, HARDCODED_INI, sizeof(HARDCODED_INI));

#ifdef PHP53
pThreadResrc->php_module.additional_functions = additional_functions ;
#endif


pThreadResrc->php_module.executable_location = NULL ;



  if (pThreadResrc->php_module.startup(&pThreadResrc->php_module)==FAILURE) {
	  return FAILURE;
  }
 
  zend_llist_init(&global_vars, sizeof(char *), NULL, 0);  

  /* Set some Embedded PHP defaults */
  SG(options) |= SAPI_OPTION_NO_CHDIR;
  SG(request_info).argc=0;
  SG(request_info).argv=NULL;

 if (php_request_startup(TSRMLS_C)==FAILURE) {
	  php_module_shutdown(TSRMLS_C);
	  return FAILURE;
  }
  
  SG(headers_sent) = 1;
  SG(request_info).no_headers = 1;
  php_register_variable("PHP_SELF", "-", NULL TSRMLS_CC);


	// TSRMLS_FETCH() ;
/*
	zval * pRet ;
	MAKE_STD_ZVAL(pRet) ;

	void *** tsrm_ls = (void ***)pParam ;

	//php_embed_init(0,NULL PTSRMLS_CC) ;
	zend_eval_string("fopen('c:\\xxxxxx','a')",pRet,"XXXXXXXX" TSRMLS_CC) ;
*/
	//#endif


	return 0 ;
}


ZEND_FUNCTION(pdr_thread_create)
{
	pdr_thread_resrc * pThreadResrc = new pdr_thread_resrc() ;
	
	int nResrc = _pdr_get_resrc_thread_window() ;
	ZEND_REGISTER_RESOURCE( return_value, pThreadResrc, nResrc )


	DWORD dwId = 0 ;
	pThreadResrc->nThreadHandle = ::CreateThread(NULL,0,pdr_thread_callback,(void *)pThreadResrc,0,&pThreadResrc->nThreadId) ;

	if( !pThreadResrc->nThreadHandle )
	{
		return_value->__refcount -- ;
		RETURN_FALSE ;
	}


}



#ifdef ignore

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
	/* 取得php线程资源 */
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
	/* 取得php线程资源 */
	__pdr_get_resource_window_thread("r",)

	if( !pThreadRc->pThread->m_pMainWnd )
	{
		RETURN_NULL()
	}

	RETURN_LONG((long)pThreadRc->pThread->m_pMainWnd->GetSafeHwnd())
}


#define __pdr_thread_set_handle(handle_type) 	\
	zval * pFunc = NULL ;\
	/* 取得php线程资源 */\
	__pdr_get_resource_window_thread("r|z",__PDR_RESRC_MPARAM(&pFunc))\
\
	/* 取得各项参数*/\
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
	/* 尝试转换类型到string*/\
	if (Z_TYPE_PP(params[1]) != IS_STRING && Z_TYPE_PP(params[1]) != IS_ARRAY)\
	{\
		SEPARATE_ZVAL(params[1]);\
		convert_to_string_ex(params[1]);\
	}\
\
	/* 检查参数类型*/\
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
	/* 增加 回调函数 和 回调参数 的引用计数 */\
	pThreadRc->handle_type->pFunc->__refcount ++ ; \
	for(int i=0;i<pThreadRc->handle_type->nArgc;i++)\
	{\
		(*pThreadRc->handle_type->pppArgs[i])->__refcount ++ ; \
	}\
\
	/* 返回 */\
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
	/* 取得php线程资源 */
	__pdr_get_resource_window_thread("r",)
	RETURN_LONG(pThreadRc->pThread->ResumeThread()) ;
}

ZEND_FUNCTION(pdr_thread_suspend)
{
	/* 取得php线程资源 */
	__pdr_get_resource_window_thread("r",)
	RETURN_LONG(pThreadRc->pThread->SuspendThread()) ;
}

#endif