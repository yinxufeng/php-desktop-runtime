

typedef struct _pdr_file_handle
{
	HANDLE hFile ;

} pdr_file_handle ;


// 取得已经打开的 COM句柄 资源
#define PDR_GetComHandleFromResrc(type_spec,other_param) zval * zvalResrc ;\
	pdr_file_handle * pFileHandle = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &zvalResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pFileHandle, pdr_file_handle*, &zvalResrc, -1, resrc_name_pdr_file, _pdr_get_resrc_file()) ;\
	if(!pFileHandle || !pFileHandle->hFile )\
	{\
		zend_error(E_WARNING, "PDR : you was given a avalid COM handle." );\
		RETURN_FALSE\
	}



