
typedef struct _pdr_proc_handle
{
	HANDLE hProcess ;
	HANDLE hThread ;

	DWORD dwProcessId ;
	DWORD dwThreadId ;

} pdr_proc_handle ;

typedef struct _pdr_pipe_handle
{
	HANDLE hRead ;
	HANDLE hWrite ;

} pdr_pipe_handle ;


// 取得已经打开的 process 句柄 资源
#define PDR_GetProcHandleFromResrc(type_spec,other_param) zval * zvalResrc ;\
	pdr_proc_handle * pProcess = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &zvalResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pProcess, pdr_proc_handle*, &zvalResrc, -1, resrc_name_pdr_proc, _pdr_get_resrc_proc()) ;\
	if(!pProcess || !pProcess->hProcess || !pProcess->hThread )\
	{\
		zend_error(E_WARNING, "PDR Process: you was given a avalid process handle." );\
		RETURN_FALSE\
	}


// 取得已经打开的 pipe 句柄 资源
#define PDR_GetPipeHandleFromResrc(type_spec,other_param) zval * zvalResrc ;\
	pdr_pipe_handle * pPipe = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &zvalResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pPipe, pdr_pipe_handle*, &zvalResrc, -1, resrc_name_pdr_pipe, _pdr_get_resrc_pipe()) ;\
	if(!pPipe || !pPipe->hRead || !pPipe->hWrite )\
	{\
		zend_error(E_WARNING, "PDR Process: you was given a avalid pipe handle." );\
		RETURN_FALSE\
	}


