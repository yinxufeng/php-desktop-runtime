// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
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

// TODO: 在此处引用程序需要的其他头文件
