// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#include <stdio.h>
#include <tchar.h>


#define FOR_PHP 1


#ifdef PHP53

#define __refcount refcount__gc
#define __zend_is_callable(callable,callable_name) zend_is_callable(callable,IS_CALLABLE_CHECK_SYNTAX_ONLY,callable_name TSRMLS_CC)

#else

typedef int socklen_t;
#define __refcount refcount
#define __zend_is_callable(callable,callable_name) zend_is_callable(callable,IS_CALLABLE_CHECK_SYNTAX_ONLY,callable_name)

#endif


//extern "C" {
	#include "zend_config.w32.h"
	/* include standard header */
	#include "php.h"
//}

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
