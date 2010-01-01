#include "stdafx.h"


typedef zend_op_array *(*ORI_ZEND_COMPILE_FILE)(zend_file_handle *file_handle, int type TSRMLS_DC) ;
ORI_ZEND_COMPILE_FILE * pdr_get_ori_zend_compile_file() ;

zend_op_array * pdr_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC) ;

#define pdr_encode 1

