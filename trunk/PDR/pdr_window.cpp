#include "stdafx.h"
#include "CPDR.h"

ZEND_FUNCTION(pdr_window_set_text)
{
	char * psText ;
	int nWnd=0, nTextLen=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &nWnd, &psText, &nTextLen ) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		RETURN_FALSE 
	}

	RETURN_BOOL(::SetWindowText(hWnd,psText)) ;
}

ZEND_FUNCTION(pdr_window_get_text)
{
	int nWnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nWnd ) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		RETURN_FALSE 
	}

	
	zval * pzvRet ;
	MAKE_STD_ZVAL(pzvRet)

	int nTextLen = GetWindowTextLength(hWnd) ;
	if( !nTextLen )
	{
		ZVAL_STRING(pzvRet,"",1) ;
	}

	else
	{
		char * psText = new char[nTextLen+1] ;
		int nTextLen = ::GetWindowText(hWnd,psText,nTextLen) ;
		psText[nTextLen] = '\0' ;

		ZVAL_STRING(pzvRet,psText,1) ;
		delete [] psText ;
	}

	RETURN_ZVAL(pzvRet,0,0)
}

ZEND_FUNCTION(pdr_window_get_long)
{
	int nWnd=0, nIdx=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nWnd, &nIdx ) == FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_LONG(::GetWindowLong((HWND)nWnd,nIdx))
}

ZEND_FUNCTION(pdr_window_set_long)
{
	int nWnd=0, nIdx=0, nNewVal=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &nWnd, &nIdx, &nNewVal ) == FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_LONG(::SetWindowLong((HWND)nWnd,nIdx,nNewVal))
}

ZEND_FUNCTION(pdr_window_get_pixel)
{
	int nWnd=0, nX=0, nY=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|l", &nX, &nY, &nWnd ) == FAILURE )
	{
		RETURN_FALSE
	}

	CString ClrText(_T("")) ;

	//获取 DC
	HDC hDC = ::GetDC((HWND)nWnd);	
	if(hDC)
	{

		//获取当前鼠标点像素值
		COLORREF clr = ::GetPixel(hDC, (int)nX,(int)nY);
		
		ClrText.Format(_T("#%02x%02x%02x"),GetRValue(clr),GetGValue(clr),GetBValue(clr)) ;
		ClrText.MakeUpper() ;

		 //释放 DC
		::ReleaseDC(NULL, hDC);
	
		RETURN_STRING((char *)(LPCTSTR)ClrText,1)
	}

	else
	{
		RETURN_FALSE
	}
}

ZEND_FUNCTION(pdr_window_show)
{
	int nWnd=0, nCmdShow=SW_SHOW ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &nWnd, &nCmdShow ) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if( ::IsWindow(hWnd) )
	{
		RETURN_BOOL( ::ShowWindow(hWnd,nCmdShow) )
	}
	else
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}
}

ZEND_FUNCTION(pdr_window_get_rect)
{
	int nWnd=0, nLeft=0, nTop=0, nRight=0, nBottom=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nWnd ) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if(::IsWindow(hWnd))
	{
		CRect rect(0,0,0,0) ;
		GetWindowRect(hWnd,&rect) ;

		zval * pRetRect ;
		MAKE_STD_ZVAL(pRetRect) ;
		array_init(pRetRect) ;

		add_index_long(pRetRect,0,rect.left) ;
		add_index_long(pRetRect,1,rect.top) ;
		add_index_long(pRetRect,2,rect.right) ;
		add_index_long(pRetRect,3,rect.bottom) ;


		RETURN_ZVAL(pRetRect,1,0)
	}

	else
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}
}

ZEND_FUNCTION(pdr_window_get_client_rect)
{
	bool bRetScreenCoord = true ;
	int nWnd=0, nLeft=0, nTop=0, nRight=0, nBottom=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|b", &nWnd, &bRetScreenCoord ) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if(::IsWindow(hWnd))
	{
		RECT rect ;
		::GetClientRect(hWnd,&rect) ;

		if(bRetScreenCoord)
		{
			CPoint aPoint(rect.left,rect.top) ;
			::ClientToScreen(hWnd,&aPoint) ;
			rect.left = aPoint.x ;
			rect.top = aPoint.y ;

			aPoint.x = rect.right ;
			aPoint.y = rect.bottom ;
			::ClientToScreen(hWnd,&aPoint) ;
			rect.right = aPoint.x ;
			rect.bottom = aPoint.y ;
		}

		zval * pRetRect ;
		MAKE_STD_ZVAL(pRetRect) ;
		array_init(pRetRect) ;

		add_index_long(pRetRect,0,rect.left) ;
		add_index_long(pRetRect,1,rect.top) ;
		add_index_long(pRetRect,2,rect.right) ;
		add_index_long(pRetRect,3,rect.bottom) ;

		RETURN_ZVAL(pRetRect,1,0)
	}

	else
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}
}


ZEND_FUNCTION(pdr_window_find)
{
	char * psClassName=NULL, * psTitleText=NULL ;
	int nClassNameLen=0, nTitleTextLen=0 ;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &psClassName, &nClassNameLen, &psTitleText, &nTitleTextLen ) == FAILURE )
	{
		RETURN_FALSE
	}

	if( nClassNameLen==0 && nTitleTextLen==0 )
	{
		zend_error(E_WARNING, "PDR: must input one of class name and title text at least." ) ;
		RETURN_FALSE
	}

	long nRetHwnd = (long)::FindWindow(
		nClassNameLen? (LPCSTR)psClassName: NULL
		, nTitleTextLen? (LPCSTR)psTitleText: NULL
	) ;

	RETURN_LONG(nRetHwnd)
}

ZEND_FUNCTION(pdr_window_find_ex)
{
	char * psClassName=NULL, * psTitleText=NULL ;
	int nClassNameLen=0, nTitleTextLen=0 ;
	long nParent=0, nhAfter=0 ;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls|s", &nParent, &nhAfter, &psClassName, &nClassNameLen, &psTitleText, &nTitleTextLen ) == FAILURE )
	{
		RETURN_FALSE
	}

	if( nClassNameLen==0 && nTitleTextLen==0 )
	{
		zend_error(E_WARNING, "PDR: must input one of class name and title text at least." ) ;
		RETURN_FALSE
	}

	long nRetHwnd = (long)::FindWindowEx(
		(HWND)nParent, (HWND)nhAfter
		, nClassNameLen? (LPCSTR)psClassName: NULL
		, nTitleTextLen? (LPCSTR)psTitleText: NULL
	) ;

	RETURN_LONG(nRetHwnd)
}


ZEND_FUNCTION(pdr_window_set_pos)
{
	long nWnd=0, nWndInsertAfter=0 ;
	UINT nFlag=SWP_NOSIZE|SWP_NOZORDER ;
	int nX=0, nY=0, nCX=0, nCY=0 ;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|llllll", &nWnd, &nX, &nY, &nCX, &nCY, &nWndInsertAfter, &nFlag ) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}
	
	RETURN_BOOL( ::SetWindowPos( hWnd,(HWND)nWndInsertAfter, nX, nY, nCX, nCY, nFlag )!=0 ) ;
}


ZEND_FUNCTION(pdr_window_get_thread)
{
	long nWnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nWnd ) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}

	DWORD nProcessId = 0 ;
	long nThreadId = (long)::GetWindowThreadProcessId(hWnd,&nProcessId) ;

	RETURN_LONG( nThreadId ) ;
}

ZEND_FUNCTION(pdr_window_get_process)
{
	long nWnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nWnd ) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}

	DWORD nProcessId = 0 ;
	::GetWindowThreadProcessId(hWnd,&nProcessId) ;

	RETURN_LONG( nProcessId ) ;
}

ZEND_FUNCTION(pdr_window_foreground)
{
	long nWnd=0 ;
	bool bRestore = true ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|b", &nWnd ) == FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}

	::SwitchToThisWindow(hWnd,bRestore) ;
	RETURN_TRUE
}

ZEND_FUNCTION(pdr_window_set_parent)
{
	long nHwnd=0, nParentHwnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nHwnd, &nParentHwnd )==FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_LONG( (long)::SetParent((HWND)nHwnd,(HWND)nParentHwnd) ) ;
}

ZEND_FUNCTION(pdr_window_get_parent)
{
	long nHwnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nHwnd )==FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_LONG( (long)::GetParent((HWND)nHwnd ) ) ;
}

ZEND_FUNCTION(pdr_window_get_desktop)
{
	RETURN_LONG( (long)::GetDesktopWindow() )
}

ZEND_FUNCTION(pdr_window_enable)
{
	long nWnd=0 ;
	bool bEnable=true ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|b", &nWnd, &bEnable )==FAILURE )
	{
		RETURN_FALSE
	}
	
	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}

	RETURN_BOOL( ::EnableWindow(hWnd,bEnable) )
}

ZEND_FUNCTION(pdr_window_is_enabled)
{
	long nWnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nWnd )==FAILURE )
	{
		RETURN_FALSE
	}
	
	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}

	RETURN_BOOL( ::IsWindowEnabled(hWnd) )
}

ZEND_FUNCTION(pdr_window_exists)
{
	long nWnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nWnd )==FAILURE )
	{
		RETURN_FALSE
	}

	RETURN_BOOL( ::IsWindow((HWND)nWnd) )
}

ZEND_FUNCTION(pdr_window_destroy)
{
	long nWnd=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nWnd )==FAILURE )
	{
		RETURN_FALSE
	}
	
	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}

	RETURN_BOOL( ::DestroyWindow(hWnd) )
}

ZEND_FUNCTION(pdr_window_send_msg)
{
	long nWnd=0, nMsg=0, wParam=0, lParam=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|ll", &nWnd, &nMsg, &wParam, &lParam )==FAILURE )
	{
		RETURN_FALSE
	}
	
	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}

	RETURN_LONG( ::SendMessage(hWnd,(UINT)nMsg,(WPARAM)wParam,(LPARAM)lParam) ) ;
}

ZEND_FUNCTION(pdr_window_post_msg)
{
	long nWnd=0, nMsg=0, wParam=0, lParam=0 ;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|ll", &nWnd, &nMsg, &wParam, &lParam )==FAILURE )
	{
		RETURN_FALSE
	}
	
	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}

	RETURN_BOOL( ::PostMessage(hWnd,(UINT)nMsg,(WPARAM)wParam,(LPARAM)lParam) ) ;
}

ZEND_FUNCTION(pdr_window_set_transparency)
{
	char r=(char)255, g=(char)255, b=(char)255 ;
	int nAlpha = 255 ;
	int nFlags = LWA_ALPHA ;
	long nWnd = 0 ;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|llll", &nWnd,&nFlags,&nAlpha,&r,&g,&b )==FAILURE )
	{
		RETURN_FALSE
	}

	HWND hWnd = (HWND)nWnd ;
	if( !::IsWindow(hWnd) )
	{
		zend_error(E_WARNING, "PDR: paramter 1 is not a valid window handle." );
		RETURN_FALSE
	}
	
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{
		// 设置窗口的 GWL_EXSTYLE 属性
		SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|0x80000) ;

		//取得SetLayeredWindowAttributes函数指针 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");

		if(fun)fun(hWnd,RGB(r,g,b),nAlpha,nFlags) ;
		FreeLibrary(hInst); 

		RETURN_TRUE
	}

	else
	{
		RETURN_FALSE
	}	
}

