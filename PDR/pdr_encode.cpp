#include "stdafx.h"
#include "pdr_encode.h"


ORI_ZEND_COMPILE_FILE pdr_ori_zend_compile_file = NULL ;
ORI_ZEND_COMPILE_FILE * pdr_get_ori_zend_compile_file() 
{
	return &pdr_ori_zend_compile_file ;
}

zend_op_array * pdr_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{
	if( strstr(file_handle->filename, ".opcode") != NULL)
	{
		// 读取 op_array
		// ... ...
	}
	
	// 调用 PHP原生 函数
	ORI_ZEND_COMPILE_FILE * _ori_zend_compile_file_ptr = pdr_get_ori_zend_compile_file() ;
	zend_op_array *op_array = (*_ori_zend_compile_file_ptr) (file_handle, type TSRMLS_CC) ;


	// 保存 op_array;
	// ... ...

	return op_array ;
}
