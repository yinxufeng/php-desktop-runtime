// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define FOR_PHP 1


#if !defined(AFX_STDAFX_H__55937AE3_B9B9_4D38_BAA8_350400484E1E__INCLUDED_)
#define AFX_STDAFX_H__55937AE3_B9B9_4D38_BAA8_350400484E1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WINVER
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif 

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxhtml.h>
#include <afxdhtml.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__55937AE3_B9B9_4D38_BAA8_350400484E1E__INCLUDED_)



#ifdef PHP53

#define __refcount refcount__gc
#define __zend_is_callable(callable,callable_name) zend_is_callable(callable,IS_CALLABLE_CHECK_SYNTAX_ONLY,callable_name TSRMLS_CC)

#else

typedef int socklen_t;
#define __refcount refcount
#define __zend_is_callable(callable,callable_name) zend_is_callable(callable,IS_CALLABLE_CHECK_SYNTAX_ONLY,callable_name)

#endif

#ifdef PHP_COMPILER_VC8
#define PHP_COMPILER_ID "VC8"
#endif

extern "C" {
	#include "zend_config.w32.h"
	/* include standard header */
	#include "php.h"
}


#ifdef _DEBUG
/*typedef struct __BreakpointStruct{ int m ; } _BreakpointStruct ;

#define breakpoint \
	__BreakpointStruct * __pBreakPoint = new __BreakpointStruct() ;\
	__pBreakPoint = NULL ;\
	__pBreakPoint->m = 1 ;*/
#endif


