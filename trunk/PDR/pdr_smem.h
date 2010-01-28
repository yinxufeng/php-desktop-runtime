
typedef struct _pdr_smem_handle
{
	HANDLE hMappingFile ;
	//char * pAccessPtr ;
	bool bAutoRelease ;
	unsigned long nMemSize ;

	//bool Open(HANDLE hMemFile,unsigned long nSize) ;
	void Release(bool bForceRelease=true) ;
	bool Write(char * pData,unsigned long nDataLen,unsigned long nSeek=0) ;
	bool Read(char * pData,unsigned long nDataLen,unsigned long nSeek=0) ;
	void WriteMemSize(unsigned long nSize,char * pInAccessPtr=NULL) ;
	unsigned long ReadMemSize(char * pInAccessPtr=NULL) ;

} pdr_smem_handle ;



// 取得已经打开的 共享对象句柄 资源
#define PDR_GetShareMemoryHandleFromResrc(type_spec,other_param) zval * zvalResrc ;\
	pdr_smem_handle * pSMemResrc = NULL ;\
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, type_spec, &zvalResrc other_param ) == FAILURE )\
	{\
		RETURN_FALSE\
	}\
	ZEND_FETCH_RESOURCE(pSMemResrc, pdr_smem_handle*, &zvalResrc, -1, resrc_name_pdr_smem, _pdr_get_resrc_smem()) ;\
	if(!pSMemResrc || !pSMemResrc->hMappingFile )\
	{\
		zend_error(E_WARNING, "PDR Share Memory: you was given a avalid Share Memory resource." );\
		RETURN_FALSE\
	}

