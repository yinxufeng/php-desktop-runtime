// pdr_dhtml.cpp : 定义 DLL 应用程序的入口点。
//
#include "stdafx.h"
#include "CPDR.h"
#include "CDHtmlDlg.h"
#include "CUIThread.h"
#include "pdr_thread.h"
#include <Winuser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


int _pdr_get_resrc_dhtml_dlg()
{ return resrc_pdr_dhtml_dlg ; }

int _pdr_get_resrc_thread_window()
{ return resrc_pdr_thread_window ; }

int _pdr_get_resrc_menu_icon()
{ return resrc_pdr_menu_icon ; }


// 销毁资源
void _php_pdr_dhtml_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	CDHtmlDlg *pDlg= (CDHtmlDlg *) rsrc->ptr;
	delete pDlg ;
	//efree(pDlg);
}
void _php_pdr_thread_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	pdr_window_thread *pThreadRc = (pdr_window_thread *) rsrc->ptr ;
	
	if( pThreadRc->pThread && pThreadRc->pThread->m_hThread )
	{
		::TerminateThread(pThreadRc->pThread->m_hThread,0) ;
	}

	delete pThreadRc->pThread ;
	pThreadRc->pThread = NULL ;

	delete pThreadRc->pInitHandle ;
	pThreadRc->pInitHandle = NULL ;
	delete pThreadRc->pExitHandle ;
	pThreadRc->pInitHandle = NULL ;

	efree(pThreadRc);
}
void _php_pdr_menu_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	CMenu *pMenu= (CMenu *) rsrc->ptr ;
	delete pMenu ;
}

// 初始化函数
ZEND_MINIT_FUNCTION(pdr_init)
{
	// 注册资源类型
	resrc_pdr_dhtml_dlg = zend_register_list_destructors_ex(_php_pdr_dhtml_destruction_handler, NULL, resrc_name_pdr_dhtml_dlg, module_number);
	resrc_pdr_thread_window = zend_register_list_destructors_ex(_php_pdr_thread_destruction_handler, NULL, resrc_name_pdr_thread_window, module_number);
	resrc_pdr_menu_icon = zend_register_list_destructors_ex(_php_pdr_menu_destruction_handler, NULL, resrc_name_pdr_menu_icon, module_number);

	// 定义常量
	// ============================
	//	窗口属性
	REGISTER_LONG_CONSTANT("GWL_EXSTYLE", GWL_EXSTYLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GWL_STYLE", GWL_STYLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GWL_HINSTANCE", GWL_HINSTANCE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GWL_HWNDPARENT", GWL_HWNDPARENT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GWL_USERDATA", GWL_USERDATA, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GWL_ID", GWL_ID, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DWL_MSGRESULT", DWL_MSGRESULT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DWL_USER", DWL_USER, CONST_CS | CONST_PERSISTENT);

	// 窗口样式
	REGISTER_LONG_CONSTANT("WS_OVERLAPPED", WS_OVERLAPPED, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_POPUP", WS_POPUP, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_CHILD", WS_CHILD, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_MINIMIZE", WS_MINIMIZE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_VISIBLE", WS_VISIBLE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_DISABLED", WS_DISABLED, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_CLIPSIBLINGS", WS_CLIPSIBLINGS, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_CLIPCHILDREN", WS_CLIPCHILDREN, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_MAXIMIZE", WS_MAXIMIZE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_BORDER", WS_BORDER, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_DLGFRAME", WS_DLGFRAME, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_VSCROLL", WS_VSCROLL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_HSCROLL", WS_HSCROLL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_SYSMENU", WS_SYSMENU, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_THICKFRAME", WS_THICKFRAME, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_GROUP", WS_GROUP, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_TABSTOP", WS_TABSTOP, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_MINIMIZEBOX", WS_MINIMIZEBOX, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_MAXIMIZEBOX", WS_MAXIMIZEBOX, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_CAPTION", WS_CAPTION, CONST_CS|CONST_PERSISTENT);

	// 扩展窗口样式
	REGISTER_LONG_CONSTANT("WS_EX_DLGMODALFRAME", WS_EX_DLGMODALFRAME, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_NOPARENTNOTIFY", WS_EX_NOPARENTNOTIFY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_ACCEPTFILES", WS_EX_ACCEPTFILES, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_TRANSPARENT", WS_EX_TRANSPARENT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_MDICHILD", WS_EX_MDICHILD, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_TOOLWINDOW", WS_EX_TOOLWINDOW, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_WINDOWEDGE", WS_EX_WINDOWEDGE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_CONTEXTHELP", WS_EX_CONTEXTHELP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_RIGHT", WS_EX_RIGHT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_LEFT", WS_EX_LEFT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_RTLREADING", WS_EX_RTLREADING, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_LTRREADING", WS_EX_LTRREADING, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_LEFTSCROLLBAR", WS_EX_LEFTSCROLLBAR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_RIGHTSCROLLBAR", WS_EX_RIGHTSCROLLBAR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_CONTROLPARENT", WS_EX_CONTROLPARENT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_STATICEDGE", WS_EX_STATICEDGE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_APPWINDOW", WS_EX_APPWINDOW, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_OVERLAPPEDWINDOW", WS_EX_OVERLAPPEDWINDOW, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_LAYERED", WS_EX_LAYERED, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_NOACTIVATE", WS_EX_NOACTIVATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_CLIENTEDGE", WS_EX_CLIENTEDGE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("WS_EX_TOPMOST", WS_EX_TOPMOST, CONST_CS | CONST_PERSISTENT);

	//	对话框样式
	REGISTER_LONG_CONSTANT("DS_ABSALIGN", DS_ABSALIGN, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_SYSMODAL", DS_SYSMODAL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_LOCALEDIT", DS_LOCALEDIT, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_SETFONT", DS_SETFONT, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_MODALFRAME", DS_MODALFRAME, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_NOIDLEMSG", DS_NOIDLEMSG, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_SETFOREGROUND", DS_SETFOREGROUND, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_3DLOOK", DS_3DLOOK, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_FIXEDSYS", DS_FIXEDSYS, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_NOFAILCREATE", DS_NOFAILCREATE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_CONTROL", DS_CONTROL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_CENTER", DS_CENTER, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_CENTERMOUSE", DS_CENTERMOUSE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_CONTEXTHELP", DS_CONTEXTHELP, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_SHELLFONT", DS_SHELLFONT, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DS_USEPIXELS", 0x8000L, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DM_GETDEFID", DM_GETDEFID, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DM_SETDEFID", DM_SETDEFID, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DM_REPOSITION", DM_REPOSITION, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DC_HASDEFID", DC_HASDEFID, CONST_CS|CONST_PERSISTENT);

	// 对话框按钮
	REGISTER_LONG_CONSTANT("IDOK", IDOK, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IDCANCEL", IDCANCEL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IDABORT", IDCANCEL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IDIGNORE", IDCANCEL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IDNO", IDCANCEL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IDYES", IDCANCEL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IDRETRY", IDCANCEL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("MB_ABORTRETRYIGNORE", MB_ABORTRETRYIGNORE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("MB_OK", MB_OK, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("MB_OKCANCEL", MB_OKCANCEL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("MB_RETRYCANCEL", MB_RETRYCANCEL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("MB_YESNO", MB_YESNO, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("MB_YESNOCANCEL", MB_YESNOCANCEL, CONST_CS|CONST_PERSISTENT);

	//	窗口状态
	REGISTER_LONG_CONSTANT("SW_HIDE", SW_HIDE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_SHOWNORMAL", SW_SHOWNORMAL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_NORMAL", SW_NORMAL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_SHOWMINIMIZED", SW_SHOWMINIMIZED, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_SHOWMAXIMIZED", SW_SHOWMAXIMIZED, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_MAXIMIZE", SW_MAXIMIZE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_SHOWNOACTIVATE", SW_SHOWNOACTIVATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_SHOW", SW_SHOW, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_MINIMIZE", SW_MINIMIZE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_SHOWMINNOACTIVE", SW_SHOWMINNOACTIVE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_SHOWNA", SW_SHOWNA, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_RESTORE", SW_RESTORE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_SHOWDEFAULT", SW_SHOWDEFAULT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_FORCEMINIMIZE", SW_FORCEMINIMIZE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SW_MAX", SW_MAX, CONST_CS | CONST_PERSISTENT);

	//	窗口位置
	REGISTER_LONG_CONSTANT("SWP_ASYNCWINDOWPOS", SWP_ASYNCWINDOWPOS, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_DEFERERASE", SWP_DEFERERASE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_DRAWFRAME", SWP_DRAWFRAME, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_FRAMECHANGED", SWP_FRAMECHANGED, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_HIDEWINDOW", SWP_HIDEWINDOW, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_NOACTIVATE", SWP_NOACTIVATE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_NOCOPYBITS", SWP_NOCOPYBITS, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_NOMOVE", SWP_NOMOVE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_NOOWNERZORDER", SWP_NOOWNERZORDER, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_NOREDRAW", SWP_NOREDRAW, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_NOREPOSITION", SWP_NOREPOSITION, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_NOSENDCHANGING", SWP_NOSENDCHANGING, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_NOSIZE", SWP_NOSIZE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_NOZORDER", SWP_NOZORDER, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWP_SHOWWINDOW", SWP_SHOWWINDOW, CONST_CS|CONST_PERSISTENT);
	
	REGISTER_LONG_CONSTANT("HWND_BOTTOM", (long)HWND_BOTTOM, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("HWND_NOTOPMOST", (long)HWND_NOTOPMOST, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("HWND_TOP", (long)HWND_TOP, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("HWND_TOPMOST", (long)HWND_TOPMOST, CONST_CS|CONST_PERSISTENT);

	// HRESULT
	REGISTER_LONG_CONSTANT("S_OK", (long)S_OK, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("S_FALSE", (long)S_FALSE, CONST_CS|CONST_PERSISTENT);


	//	DHTML 事件
	REGISTER_LONG_CONSTANT("DHTML_ONACTIVATE", DISPID_HTMLELEMENTEVENTS_ONACTIVATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONAFTERUPDATE", DISPID_HTMLELEMENTEVENTS_ONAFTERUPDATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREACTIVATE", DISPID_HTMLELEMENTEVENTS_ONBEFOREACTIVATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONBEFORECOPY", DISPID_HTMLELEMENTEVENTS_ONBEFORECOPY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONBEFORECUT", DISPID_HTMLELEMENTEVENTS_ONBEFORECUT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREDEACTIVATE", DISPID_HTMLELEMENTEVENTS_ONBEFOREDEACTIVATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREEDITFOCUS", DISPID_HTMLELEMENTEVENTS_ONBEFOREEDITFOCUS, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREPASTE", DISPID_HTMLELEMENTEVENTS_ONBEFOREPASTE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONBEFOREUPDATE", DISPID_HTMLELEMENTEVENTS_ONBEFOREUPDATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONBLUR", DISPID_HTMLELEMENTEVENTS_ONBLUR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONCELLCHANGE", DISPID_HTMLELEMENTEVENTS_ONCELLCHANGE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONCLICK", DISPID_HTMLELEMENTEVENTS_ONCLICK, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONCONTEXTMENU", DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONCONTROLSELECT", DISPID_HTMLELEMENTEVENTS_ONCONTROLSELECT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONCOPY", DISPID_HTMLELEMENTEVENTS_ONCOPY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONCUT", DISPID_HTMLELEMENTEVENTS_ONCUT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDATAAVAILABLE", DISPID_HTMLELEMENTEVENTS_ONDATAAVAILABLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDATASETCHANGED", DISPID_HTMLELEMENTEVENTS_ONDATASETCHANGED, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDATASETCOMPLETE", DISPID_HTMLELEMENTEVENTS_ONDATASETCOMPLETE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDBLCLICK", DISPID_HTMLELEMENTEVENTS_ONDBLCLICK, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDEACTIVATE", DISPID_HTMLELEMENTEVENTS_ONDEACTIVATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDRAG", DISPID_HTMLELEMENTEVENTS_ONDRAG, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGEND", DISPID_HTMLELEMENTEVENTS_ONDRAGEND, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGENTER", DISPID_HTMLELEMENTEVENTS_ONDRAGENTER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGLEAVE", DISPID_HTMLELEMENTEVENTS_ONDRAGLEAVE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGOVER", DISPID_HTMLELEMENTEVENTS_ONDRAGOVER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDRAGSTART", DISPID_HTMLELEMENTEVENTS_ONDRAGSTART, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONDROP", DISPID_HTMLELEMENTEVENTS_ONDROP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONERRORUPDATE", DISPID_HTMLELEMENTEVENTS_ONERRORUPDATE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONFILTERCHANGE", DISPID_HTMLELEMENTEVENTS_ONFILTERCHANGE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONFOCUS", DISPID_HTMLELEMENTEVENTS_ONFOCUS, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONFOCUSIN", DISPID_HTMLELEMENTEVENTS_ONFOCUSIN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONFOCUSOUT", DISPID_HTMLELEMENTEVENTS_ONFOCUSOUT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONHELP", DISPID_HTMLELEMENTEVENTS_ONHELP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONKEYDOWN", DISPID_HTMLELEMENTEVENTS_ONKEYDOWN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONKEYPRESS", DISPID_HTMLELEMENTEVENTS_ONKEYPRESS, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONKEYUP", DISPID_HTMLELEMENTEVENTS_ONKEYUP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONLAYOUTCOMPLETE", DISPID_HTMLELEMENTEVENTS_ONLAYOUTCOMPLETE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONLOSECAPTURE", DISPID_HTMLELEMENTEVENTS_ONLOSECAPTURE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEDOWN", DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEENTER", DISPID_HTMLELEMENTEVENTS_ONMOUSEENTER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSELEAVE", DISPID_HTMLELEMENTEVENTS_ONMOUSELEAVE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEMOVE", DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEOUT", DISPID_HTMLELEMENTEVENTS_ONMOUSEOUT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEOVER", DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEUP", DISPID_HTMLELEMENTEVENTS_ONMOUSEUP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOUSEWHEEL", DISPID_HTMLELEMENTEVENTS_ONMOUSEWHEEL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOVE", DISPID_HTMLELEMENTEVENTS_ONMOVE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOVEEND", DISPID_HTMLELEMENTEVENTS_ONMOVEEND, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONMOVESTART", DISPID_HTMLELEMENTEVENTS_ONMOVESTART, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONPAGE", DISPID_HTMLELEMENTEVENTS_ONPAGE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONPASTE", DISPID_HTMLELEMENTEVENTS_ONPASTE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONPROPERTYCHANGE", DISPID_HTMLELEMENTEVENTS_ONPROPERTYCHANGE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONREADYSTATECHANGE", DISPID_HTMLELEMENTEVENTS_ONREADYSTATECHANGE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONRESIZE", DISPID_HTMLELEMENTEVENTS_ONRESIZE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONRESIZEEND", DISPID_HTMLELEMENTEVENTS_ONRESIZEEND, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONRESIZESTART", DISPID_HTMLELEMENTEVENTS_ONRESIZESTART, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONROWENTER", DISPID_HTMLELEMENTEVENTS_ONROWENTER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONROWEXIT", DISPID_HTMLELEMENTEVENTS_ONROWEXIT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONROWSDELETE", DISPID_HTMLELEMENTEVENTS_ONROWSDELETE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONROWSINSERTED", DISPID_HTMLELEMENTEVENTS_ONROWSINSERTED, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONSCROLL", DISPID_HTMLELEMENTEVENTS_ONSCROLL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DHTML_ONSELECTSTART", DISPID_HTMLELEMENTEVENTS_ONSELECTSTART, CONST_CS | CONST_PERSISTENT);

	
	// DHTML Dialog 事件
	REGISTER_LONG_CONSTANT("DHTML_ONINITDIALOG", DLG_EVENT_ONINITDIALOG, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("DHTML_ONBEFORENAVIGATE", DLG_EVENT_ONBEFORENAVIGATE, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("DHTML_ONNAVIGATECOMPLETE", DLG_EVENT_ONNAVIGATECOMPLETE, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("DHTML_ONDOCUMENTCOMPLETE", DLG_EVENT_ONDOCUMENTCOMPLETE, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("DHTML_ONCOMMAND", DLG_EVENT_ONCOMMAND, CONST_CS|CONST_PERSISTENT) ;
	
	// 托盘图标事件
	REGISTER_LONG_CONSTANT("TRAY_ONMOUSEMOVE", TRAY_EVENT_ONMOUSEMOVE, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TRAY_ONLBUTTONDOWN", TRAY_EVENT_ONLBUTTONDOWN, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TRAY_ONLBUTTONUP", TRAY_EVENT_ONLBUTTONUP, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TRAY_ONLBUTTONDBLCLK", TRAY_EVENT_ONLBUTTONDBLCLK, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TRAY_ONRBUTTONDOWN", TRAY_EVENT_ONRBUTTONDOWN, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TRAY_ONRBUTTONUP", TRAY_EVENT_ONRBUTTONUP, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TRAY_ONRBUTTONDBLCLK", TRAY_EVENT_ONRBUTTONDBLCLK, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TRAY_ONMBUTTONDOWN", TRAY_EVENT_ONMBUTTONDOWN, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TRAY_ONMBUTTONUP", TRAY_EVENT_ONMBUTTONUP, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TRAY_ONMBUTTONDBLCLK", TRAY_EVENT_ONMBUTTONDBLCLK, CONST_CS|CONST_PERSISTENT) ;

	// 菜单 FLAG
	REGISTER_LONG_CONSTANT("MF_CHECKED", MF_CHECKED, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_UNCHECKED", MF_UNCHECKED, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_DISABLED", MF_DISABLED, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_ENABLED", MF_ENABLED, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_GRAYED", MF_GRAYED, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_MENUBARBREAK", MF_MENUBARBREAK, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_MENUBREAK", MF_MENUBREAK, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_OWNERDRAW", MF_OWNERDRAW, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_POPUP", MF_POPUP, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_SEPARATOR", MF_SEPARATOR, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_STRING", MF_STRING, CONST_CS|CONST_PERSISTENT) ;

	REGISTER_LONG_CONSTANT("MF_BYCOMMAND", MF_BYCOMMAND, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("MF_BYPOSITION", MF_BYPOSITION, CONST_CS|CONST_PERSISTENT) ;


	// 菜单 弹出
	REGISTER_LONG_CONSTANT("TPM_CENTERALIGN", TPM_CENTERALIGN, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_LEFTALIGN", TPM_LEFTALIGN, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_RIGHTALIGN", TPM_RIGHTALIGN, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_BOTTOMALIGN", TPM_BOTTOMALIGN, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_TOPALIGN", TPM_TOPALIGN, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_VCENTERALIGN", TPM_VCENTERALIGN, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_NONOTIFY", TPM_NONOTIFY, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_RETURNCMD", TPM_RETURNCMD, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_LEFTBUTTON", TPM_LEFTBUTTON, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_RIGHTBUTTON", TPM_RIGHTBUTTON, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_HORNEGANIMATION", TPM_HORNEGANIMATION, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_HORPOSANIMATION", TPM_HORPOSANIMATION, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_NOANIMATION", TPM_NOANIMATION, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_VERNEGANIMATION", TPM_VERNEGANIMATION, CONST_CS|CONST_PERSISTENT) ;
	REGISTER_LONG_CONSTANT("TPM_VERPOSANIMATION", TPM_VERPOSANIMATION, CONST_CS|CONST_PERSISTENT) ;
	


	// 初始化 mfc
	// ----------------------------------
	AfxEnableControlContainer();
	CoInitialize(NULL);


	return SUCCESS;
}

// 析构函数
ZEND_MSHUTDOWN_FUNCTION(pdr_shutdown)
{
	/*	UNREGISTER_INI_ENTRIES(); */
	return SUCCESS;
}


// 系统函数
// ------------------------
ZEND_FUNCTION(pdr_get_last_error);
ZEND_FUNCTION(pdr_execute);
ZEND_FUNCTION(pdr_get_process_filename);
ZEND_FUNCTION(pdr_pump_msg);
ZEND_FUNCTION(pdr_peek_msg);
ZEND_FUNCTION(pdr_get_msg);
ZEND_FUNCTION(pdr_dispatch_msg);
ZEND_FUNCTION(pdr_close_handle) ;
ZEND_FUNCTION(pdr_create_mutex) ;
ZEND_FUNCTION(pdr_open_mutex) ;
ZEND_FUNCTION(pdr_release_mutex) ;


// 线程函数
// ------------------------
/*ZEND_FUNCTION(pdr_thread_create_work) ;
ZEND_FUNCTION(pdr_thread_create_window) ;
ZEND_FUNCTION(pdr_thread_set_hwnd) ;
ZEND_FUNCTION(pdr_thread_get_hwnd) ;
ZEND_FUNCTION(pdr_thread_set_init_handle) ;
ZEND_FUNCTION(pdr_thread_set_exit_handle) ;
ZEND_FUNCTION(pdr_thread_resume) ;
ZEND_FUNCTION(pdr_thread_suspend) ;*/

// Window 函数
// ------------------------
ZEND_FUNCTION(pdr_window_get_long);
ZEND_FUNCTION(pdr_window_set_long);
ZEND_FUNCTION(pdr_window_get_pixel);
ZEND_FUNCTION(pdr_window_show);
ZEND_FUNCTION(pdr_window_get_rect);
ZEND_FUNCTION(pdr_window_get_client_rect);
ZEND_FUNCTION(pdr_window_find);
ZEND_FUNCTION(pdr_window_find_ex);
ZEND_FUNCTION(pdr_window_set_pos);
ZEND_FUNCTION(pdr_window_get_thread);
ZEND_FUNCTION(pdr_window_get_process);
ZEND_FUNCTION(pdr_window_foreground);
ZEND_FUNCTION(pdr_window_msg) ;
ZEND_FUNCTION(pdr_window_set_parent) ;
ZEND_FUNCTION(pdr_window_get_parent) ;
ZEND_FUNCTION(pdr_window_get_desktop) ;
ZEND_FUNCTION(pdr_window_is_enabled) ;
ZEND_FUNCTION(pdr_window_enable) ;
ZEND_FUNCTION(pdr_window_exists);
ZEND_FUNCTION(pdr_window_destroy) ;
ZEND_FUNCTION(pdr_window_send_msg) ;
ZEND_FUNCTION(pdr_window_post_msg) ;
ZEND_FUNCTION(pdr_window_set_transparency) ;


// DHTML 函数
// ------------------------
ZEND_FUNCTION(pdr_dhtml_new);
ZEND_FUNCTION(pdr_dhtml_create);
ZEND_FUNCTION(pdr_dhtml_show);
ZEND_FUNCTION(pdr_dhtml_domodal);
ZEND_FUNCTION(pdr_dhtml_get_hwnd);
ZEND_FUNCTION(pdr_dhtml_ok);
ZEND_FUNCTION(pdr_dhtml_cancel);
ZEND_FUNCTION(pdr_dhtml_navigate);

ZEND_FUNCTION(pdr_dhtml_set_html);
ZEND_FUNCTION(pdr_dhtml_set_text);
ZEND_FUNCTION(pdr_dhtml_set_value);
ZEND_FUNCTION(pdr_dhtml_get_html);
ZEND_FUNCTION(pdr_dhtml_get_text);
ZEND_FUNCTION(pdr_dhtml_get_value);
ZEND_FUNCTION(pdr_dhtml_call) ;

ZEND_FUNCTION(pdr_dhtml_set_handle);
ZEND_FUNCTION(pdr_dhtml_exec_script);
ZEND_FUNCTION(pdr_dhtml_get_tags);
ZEND_FUNCTION(pdr_dhtml_el_collection_len);
ZEND_FUNCTION(pdr_dhtml_el_collection_item);


// Tray Icon 函数
// ------------------------
ZEND_FUNCTION(dhtml_tray_create);
ZEND_FUNCTION(dhtml_tray_modify);
ZEND_FUNCTION(dhtml_tray_delete);

// 菜单 函数
ZEND_FUNCTION(dhtml_menu_create) ;
ZEND_FUNCTION(dhtml_menu_append) ;
ZEND_FUNCTION(dhtml_menu_insert) ;
ZEND_FUNCTION(dhtml_menu_modify) ;
ZEND_FUNCTION(dhtml_menu_remove) ;
ZEND_FUNCTION(dhtml_menu_popup) ;
ZEND_FUNCTION(dhtml_menu_get_hmenu) ;
ZEND_FUNCTION(dhtml_menu_enable) ;
ZEND_FUNCTION(dhtml_menu_check) ;
ZEND_FUNCTION(dhtml_menu_radio_check) ;
ZEND_FUNCTION(dhtml_menu_item_count) ;
ZEND_FUNCTION(dhtml_menu_item_text) ;
ZEND_FUNCTION(dhtml_menu_item_id) ;


/* compiled function list so Zend knows what's in this module */
zend_function_entry pdr_dhtml_functions[] = {

	// 系统函数
	// ------------------------
    ZEND_FE(pdr_get_last_error, NULL)
    ZEND_FE(pdr_execute, NULL)
    ZEND_FE(pdr_get_process_filename, NULL)
    ZEND_FE(pdr_pump_msg, NULL)
    ZEND_FE(pdr_peek_msg, NULL)
    ZEND_FE(pdr_get_msg, NULL)
    ZEND_FE(pdr_dispatch_msg, NULL)
    ZEND_FE(pdr_close_handle, NULL)
    ZEND_FE(pdr_create_mutex, NULL)
    ZEND_FE(pdr_open_mutex, NULL)
    ZEND_FE(pdr_release_mutex, NULL)


	// 线程函数
	// ------------------------
    /*ZEND_FE(pdr_thread_create_work, NULL)
    ZEND_FE(pdr_thread_create_window, NULL)
    ZEND_FE(pdr_thread_set_hwnd, NULL)
    ZEND_FE(pdr_thread_get_hwnd, NULL)
    ZEND_FE(pdr_thread_set_init_handle, NULL)
    ZEND_FE(pdr_thread_set_exit_handle, NULL)
    ZEND_FE(pdr_thread_resume, NULL)
	ZEND_FE(pdr_thread_suspend, NULL)*/

	// Window 函数
	// ------------------------
    ZEND_FE(pdr_window_get_long, NULL)
    ZEND_FE(pdr_window_set_long, NULL)
    ZEND_FE(pdr_window_get_pixel, NULL)
    ZEND_FE(pdr_window_show, NULL)
    ZEND_FE(pdr_window_get_rect, NULL)
    ZEND_FE(pdr_window_get_client_rect, NULL)
    ZEND_FE(pdr_window_find, NULL)
    ZEND_FE(pdr_window_find_ex, NULL)
    ZEND_FE(pdr_window_set_pos, NULL)
    ZEND_FE(pdr_window_get_thread, NULL)
    ZEND_FE(pdr_window_get_process, NULL)
    ZEND_FE(pdr_window_foreground, NULL)
    ZEND_FE(pdr_window_msg, NULL)
    ZEND_FE(pdr_window_set_parent, NULL)
    ZEND_FE(pdr_window_get_parent, NULL)
    ZEND_FE(pdr_window_get_desktop, NULL)
    ZEND_FE(pdr_window_enable, NULL)
    ZEND_FE(pdr_window_is_enabled, NULL)
    ZEND_FE(pdr_window_exists, NULL)
    ZEND_FE(pdr_window_destroy, NULL)
    ZEND_FE(pdr_window_send_msg, NULL)
    ZEND_FE(pdr_window_post_msg, NULL)
    ZEND_FE(pdr_window_set_transparency, NULL)

	// DHTML 函数
	// ------------------------
    ZEND_FE(pdr_dhtml_new, NULL)
    ZEND_FE(pdr_dhtml_create, NULL)
    ZEND_FE(pdr_dhtml_show, NULL)
    ZEND_FE(pdr_dhtml_domodal, NULL)
    ZEND_FE(pdr_dhtml_get_hwnd, NULL)
    ZEND_FE(pdr_dhtml_ok, NULL)
    ZEND_FE(pdr_dhtml_cancel, NULL)
    ZEND_FE(pdr_dhtml_navigate, NULL)

    ZEND_FE(pdr_dhtml_set_html, NULL)
    ZEND_FE(pdr_dhtml_set_text, NULL)
    ZEND_FE(pdr_dhtml_set_value, NULL)
    ZEND_FE(pdr_dhtml_get_html, NULL)
    ZEND_FE(pdr_dhtml_get_text, NULL)
    ZEND_FE(pdr_dhtml_get_value, NULL)
    ZEND_FE(pdr_dhtml_call, NULL)

    ZEND_FE(pdr_dhtml_set_handle, NULL)	
    ZEND_FE(pdr_dhtml_exec_script, NULL)
    ZEND_FE(pdr_dhtml_get_tags, NULL)
    ZEND_FE(pdr_dhtml_el_collection_len, NULL)
    ZEND_FE(pdr_dhtml_el_collection_item, NULL)


	// Tray Icon 函数
	// ------------------------
    ZEND_FE(dhtml_tray_create, NULL)
    ZEND_FE(dhtml_tray_modify, NULL)
    ZEND_FE(dhtml_tray_delete, NULL)


	// 菜单 函数
    ZEND_FE(dhtml_menu_create, NULL)
    ZEND_FE(dhtml_menu_append, NULL)
    ZEND_FE(dhtml_menu_insert, NULL)
    ZEND_FE(dhtml_menu_modify, NULL)
    ZEND_FE(dhtml_menu_remove, NULL)
    ZEND_FE(dhtml_menu_popup, NULL)
    ZEND_FE(dhtml_menu_get_hmenu, NULL)
    ZEND_FE(dhtml_menu_enable, NULL)
    ZEND_FE(dhtml_menu_check, NULL)
    ZEND_FE(dhtml_menu_radio_check, NULL)
    ZEND_FE(dhtml_menu_item_count, NULL)
    ZEND_FE(dhtml_menu_item_text, NULL)


    {NULL, NULL, NULL}
};

ZEND_MINFO_FUNCTION(pdr_info)
{
	/*php_info_print_table_start();
	php_info_print_table_header(1, "PHP Desktop Runtime extension");
	php_info_print_table_row(1, "http://pdr.jecat.cn");
	php_info_print_table_row(2, "MFC App embeded", "Not in PDR.dll");
	php_info_print_table_end();*/
}


/* compiled module information */
zend_module_entry pdr_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"PDR",

	pdr_dhtml_functions,
	ZEND_MINIT(pdr_init),				// 初始化函数
	ZEND_MSHUTDOWN(pdr_shutdown)		// 析构函数
	, NULL, NULL, 
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	//ZEND_MINFO(pdr_info),
#endif
    "0.1", STANDARD_MODULE_PROPERTIES
};

/* implement standard "stub" routine to introduce ourselves to Zend */
ZEND_GET_MODULE(pdr)

