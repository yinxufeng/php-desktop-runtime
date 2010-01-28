

typedef struct _pdr_file_handle
{
	HANDLE hFile ;

} pdr_file_handle ;


// 取得已经打开的 file 句柄 资源
#define PDR_GetWin32FileHandleFromResrc(type_spec,other_param) zval * zvalResrc ;\
	pdr_file_handle * pFileHandle = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &zvalResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pFileHandle, pdr_file_handle*, &zvalResrc, -1, resrc_name_pdr_file, _pdr_get_resrc_file()) ;\
	if(!pFileHandle || !pFileHandle->hFile )\
	{\
		zend_error(E_WARNING, "PDR Wnd32 File: you was given a avalid Win32 File handle." );\
		RETURN_FALSE\
	}



