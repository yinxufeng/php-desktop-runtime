// pdr_dhtml.cpp : 定义 DLL 应用程序的入口点。
//
#include "stdafx.h"
#include "CPDR.h"
#include "CDHtmlDlg.h"
#include "CUIThread.h"
#include "pdr_thread.h"
#include <Winuser.h>
#include "define_const.h"
#include "define_const_vkey.h"


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
	// ----------------------------------
	_pdr_define_const
	_pdr_define_const_vkey


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
ZEND_FUNCTION(pdr_register_hot_key) ;
ZEND_FUNCTION(pdr_unregister_hot_key) ;
ZEND_FUNCTION(pdr_get_system_path) ;


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
ZEND_FUNCTION(pdr_tray_create);
ZEND_FUNCTION(pdr_tray_modify);
ZEND_FUNCTION(pdr_tray_delete);

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
    ZEND_FE(pdr_register_hot_key, NULL)
    ZEND_FE(pdr_unregister_hot_key, NULL)
    ZEND_FE(pdr_get_system_path, NULL)


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
    ZEND_FE(pdr_tray_create, NULL)
    ZEND_FE(pdr_tray_modify, NULL)
    ZEND_FE(pdr_tray_delete, NULL)


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

