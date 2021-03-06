// pdr_dhtml.cpp : 定义 DLL 应用程序的入口点。
//
#include "stdafx.h"
#include "CPDR.h"
#include "CDHtmlDlg.h"
#include "pdr_thread.h"
#include "pdr_file.h"
#include "pdr_smem.h"
#include "pdr_proc_pipe.h"
// #include "pdr_encode.h"			// 仅仅实现了 HOOK PHP
#include <Winuser.h>
#include "define_const.h"
#include "define_const_vkey.h"
#include "define_const_locale.h"
#include "define_const_file.h"
#include "define_const_window_msg.h"
#include "define_const_cb.h"
#include <afxdll_.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 初始化 mfc
// ----------------------------------
CWinApp theApp ;




int _pdr_get_resrc_dhtml_dlg()
{ return resrc_pdr_dhtml_dlg ; }

int _pdr_get_resrc_thread_window()
{ return resrc_pdr_thread_window ; }

int _pdr_get_resrc_menu_icon()
{ return resrc_pdr_menu_icon ; }

int _pdr_get_resrc_file()
{ return resrc_pdr_file ; }

int _pdr_get_resrc_proc()
{ return resrc_pdr_proc ; }

int _pdr_get_resrc_pipe()
{ return resrc_pdr_pipe ; }

int _pdr_get_resrc_smem()
{ return resrc_pdr_smem ; }


void _pdr_set_global_last_error(DWORD nError)
{ pdr_global_last_error = nError ; }
DWORD _pdr_get_global_last_error()
{ return pdr_global_last_error ; }






// 销毁资源
void _php_pdr_dhtml_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	CDHtmlDlg *pDlg= (CDHtmlDlg *) rsrc->ptr;
	delete pDlg ;
	// efree(pDlg) ;
}
void _php_pdr_thread_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{}
void _php_pdr_menu_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	CMenu *pMenu= (CMenu *) rsrc->ptr ;
	delete pMenu ;
}
void _php_pdr_file_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	pdr_file_handle * pFileHandle = (pdr_file_handle *) rsrc->ptr ;

	if( pFileHandle->hFile )
	{
		::CloseHandle(pFileHandle->hFile) ;
		pFileHandle->hFile = NULL ;
	}

	delete pFileHandle ;
}
void _php_pdr_proc_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	pdr_proc_handle * pProc = (pdr_proc_handle *) rsrc->ptr ;

	if( pProc->hProcess )
	{
		::CloseHandle(pProc->hProcess) ;
		pProc->hProcess = NULL ;
	}
	if( pProc->hThread )
	{
		::CloseHandle(pProc->hThread) ;
		pProc->hThread = NULL ;
	}

	pProc->dwProcessId = 0 ;
	pProc->dwThreadId = 0 ;
	delete pProc ;
}
void _php_pdr_pipe_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	pdr_pipe_handle * pPipe = (pdr_pipe_handle *) rsrc->ptr ;

	if( pPipe->hRead )
	{
		::CloseHandle(pPipe->hRead) ;
		pPipe->hRead = NULL ;
	}
	if( pPipe->hWrite )
	{
		::CloseHandle(pPipe->hWrite) ;
		pPipe->hWrite = NULL ;
	}

	delete pPipe ;
}
void _php_pdr_smem_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	pdr_smem_handle *pSMem= (pdr_smem_handle *) rsrc->ptr;

	pSMem->Release(false) ;
	delete pSMem ;
}


static AFX_EXTENSION_MODULE afx_ext_dll_pdr = { NULL, NULL } ;

// 初始化函数
ZEND_MINIT_FUNCTION(pdr_init)
{
	_pdr_set_global_last_error(0) ;

	// 注册资源类型
	resrc_pdr_dhtml_dlg = zend_register_list_destructors_ex(_php_pdr_dhtml_destruction_handler, NULL, resrc_name_pdr_dhtml_dlg, module_number);
	resrc_pdr_thread_window = zend_register_list_destructors_ex(_php_pdr_thread_destruction_handler, NULL, resrc_name_pdr_thread_window, module_number);
	resrc_pdr_menu_icon = zend_register_list_destructors_ex(_php_pdr_menu_destruction_handler, NULL, resrc_name_pdr_menu_icon, module_number);
	resrc_pdr_file = zend_register_list_destructors_ex(_php_pdr_file_destruction_handler, NULL, resrc_name_pdr_file, module_number);
	resrc_pdr_proc = zend_register_list_destructors_ex(_php_pdr_proc_destruction_handler, NULL, resrc_name_pdr_proc, module_number);
	resrc_pdr_pipe = zend_register_list_destructors_ex(_php_pdr_pipe_destruction_handler, NULL, resrc_name_pdr_pipe, module_number);
	resrc_pdr_smem = zend_register_list_destructors_ex(_php_pdr_smem_destruction_handler, NULL, resrc_name_pdr_smem, module_number);

	// 定义常量
	// ----------------------------------
	_pdr_define_const
	_pdr_define_const_vkey
	_pdr_define_const_locale
	_pdr_define_const_file
	_pdr_define_const_window_msg
	_pdr_define_const_cb


	// 初始化 mfc
	// ----------------------------------
	//BOOL b = AfxWinInit(::GetModuleHandle(NULL), NULL, NULL, 0) ;

	CoInitialize(NULL);
	AfxEnableControlContainer();


#ifdef pdr_encode
	// pdr encode
	// ----------------------------------
	//  保存 PHP 原始函数
	ORI_ZEND_COMPILE_FILE * _ori_zend_compile_file_ptr = pdr_get_ori_zend_compile_file() ;
	(*_ori_zend_compile_file_ptr) = zend_compile_file ;

	//  Hook 新函数
	zend_compile_file = pdr_compile_file ;
#endif

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
ZEND_FUNCTION(pdr_adjust_token_privileges) ;
ZEND_FUNCTION(pdr_get_process_filename);
ZEND_FUNCTION(pdr_pump_msg);
ZEND_FUNCTION(pdr_peek_msg);
ZEND_FUNCTION(pdr_get_msg);
ZEND_FUNCTION(pdr_translate_msg) ;
ZEND_FUNCTION(pdr_dispatch_msg);
ZEND_FUNCTION(pdr_create_mutex) ;
ZEND_FUNCTION(pdr_open_mutex) ;
ZEND_FUNCTION(pdr_release_mutex) ;
ZEND_FUNCTION(pdr_register_hot_key) ;
ZEND_FUNCTION(pdr_unregister_hot_key) ;
ZEND_FUNCTION(pdr_get_system_path) ;
ZEND_FUNCTION(pdr_browse_file) ;
ZEND_FUNCTION(pdr_browse_folder) ;
ZEND_FUNCTION(pdr_browse_folder2) ;
ZEND_FUNCTION(pdr_get_locale) ;
ZEND_FUNCTION(pdr_get_charset) ;
ZEND_FUNCTION(pdr_get_php_path) ;
ZEND_FUNCTION(pdr_get_cursor_pos) ;
ZEND_FUNCTION(pdr_msgbox) ;
ZEND_FUNCTION(pdr_appbar_get_rect) ;
ZEND_FUNCTION(pdr_var_ref_cnt);
ZEND_FUNCTION(pdr_get_module_handle) ;
ZEND_FUNCTION(pdr_load_library) ;
ZEND_FUNCTION(pdr_free_library) ;
ZEND_FUNCTION(pdr_exe_version_info) ;
ZEND_FUNCTION(pdr_get_os_version) ;
ZEND_FUNCTION(pdr_debug_) ;
ZEND_FUNCTION(pdr_get_static_property) ;
ZEND_FUNCTION(pdr_kill_var) ;
ZEND_FUNCTION(pdr_set_cursor_pos) ;
ZEND_FUNCTION(pdr_keybd_event) ;
ZEND_FUNCTION(pdr_mouse_event) ;
ZEND_FUNCTION(pdr_block_input) ;


ZEND_FUNCTION(pdr_thread_create) ;


// 剪切板 函数
// ------------------------
ZEND_FUNCTION(pdr_cb_open) ;
ZEND_FUNCTION(pdr_cb_close) ;
ZEND_FUNCTION(pdr_cb_empty) ;
ZEND_FUNCTION(pdr_cb_set) ;
ZEND_FUNCTION(pdr_cb_get) ;
// 存目：
//ZEND_FUNCTION(pdr_cb_owner) ;						GetClipboardOwner
//ZEND_FUNCTION(pdr_cb_register_format) ;			RegisterClipboardFormat
//ZEND_FUNCTION(pdr_cb_enum_formats) ;				EnumClipboardFormats
//ZEND_FUNCTION(pdr_cb_is_available_format) ;		IsClipBoardFormatAvailable
//ZEND_FUNCTION(pdr_cb_get_format_name) ;			GetClipboardFormatName


// Window 函数
// ------------------------
ZEND_FUNCTION(pdr_window_set_text) ;
ZEND_FUNCTION(pdr_window_get_text) ;
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
ZEND_FUNCTION(pdr_window_lock_update) ;
ZEND_FUNCTION(pdr_window_set_capture) ;

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
ZEND_FUNCTION(pdr_dhtml_set_icon) ;

ZEND_FUNCTION(pdr_dhtml_set_html);
ZEND_FUNCTION(pdr_dhtml_set_text);
ZEND_FUNCTION(pdr_dhtml_set_value);
ZEND_FUNCTION(pdr_dhtml_get_html);
ZEND_FUNCTION(pdr_dhtml_get_text);
ZEND_FUNCTION(pdr_dhtml_get_value);
ZEND_FUNCTION(pdr_dhtml_call) ;
ZEND_FUNCTION(pdr_dhtml_get_baseurl) ;

ZEND_FUNCTION(pdr_dhtml_set_handle);
ZEND_FUNCTION(pdr_dhtml_clear_handles);
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
// ------------------------
ZEND_FUNCTION(pdr_menu_create) ;
ZEND_FUNCTION(pdr_menu_append) ;
ZEND_FUNCTION(pdr_menu_insert) ;
ZEND_FUNCTION(pdr_menu_modify) ;
ZEND_FUNCTION(pdr_menu_remove) ;
ZEND_FUNCTION(pdr_menu_popup) ;
ZEND_FUNCTION(pdr_menu_get_hmenu) ;
ZEND_FUNCTION(pdr_menu_enable) ;
ZEND_FUNCTION(pdr_menu_check) ;
ZEND_FUNCTION(pdr_menu_radio_check) ;
ZEND_FUNCTION(pdr_menu_item_count) ;
ZEND_FUNCTION(pdr_menu_item_text) ;
ZEND_FUNCTION(pdr_menu_item_id) ;

// 文件操/串口 操作函数
// ------------------------
ZEND_FUNCTION(pdr_file_create) ;
ZEND_FUNCTION(pdr_file_get_handle) ;
ZEND_FUNCTION(pdr_file_write) ;
ZEND_FUNCTION(pdr_file_read) ;
ZEND_FUNCTION(pdr_file_close) ;
ZEND_FUNCTION(pdr_com_open) ;
ZEND_FUNCTION(pdr_com_stat) ;
ZEND_FUNCTION(pdr_com_set_timeouts) ;
ZEND_FUNCTION(pdr_com_setup_buffer) ;
ZEND_FUNCTION(pdr_handle_write) ;
ZEND_FUNCTION(pdr_handle_read) ;
ZEND_FUNCTION(pdr_handle_close) ;

// 进程/管道 函数
// ------------------------
ZEND_FUNCTION(pdr_pipe_create) ;
ZEND_FUNCTION(pdr_pipe_get_read_handle) ;
ZEND_FUNCTION(pdr_pipe_get_write_handle) ;
ZEND_FUNCTION(pdr_pipe_get_size) ;
ZEND_FUNCTION(pdr_pipe_peek) ;
ZEND_FUNCTION(pdr_pipe_get_std) ;
ZEND_FUNCTION(pdr_pipe_set_std) ;

ZEND_FUNCTION(pdr_proc_create) ;
ZEND_FUNCTION(pdr_proc_get_handle) ;
ZEND_FUNCTION(pdr_proc_get_tread_handle) ;
ZEND_FUNCTION(pdr_proc_get_id) ;
ZEND_FUNCTION(pdr_proc_get_tread_id) ;
ZEND_FUNCTION(pdr_proc_terminate) ;
ZEND_FUNCTION(pdr_proc_exit_code) ;

ZEND_FUNCTION(pdr_proc_enum) ;

// Windows注册表 函数
// ------------------------
ZEND_FUNCTION(pdr_reg_create) ;
ZEND_FUNCTION(pdr_reg_open) ;
ZEND_FUNCTION(pdr_reg_set) ;
ZEND_FUNCTION(pdr_reg_get) ;
ZEND_FUNCTION(pdr_reg_set_string) ;
ZEND_FUNCTION(pdr_reg_get_string) ;
ZEND_FUNCTION(pdr_reg_get_type) ;
ZEND_FUNCTION(pdr_reg_delete_value) ;
ZEND_FUNCTION(pdr_reg_delete_key) ;
ZEND_FUNCTION(pdr_reg_close) ;
ZEND_FUNCTION(pdr_reg_flush) ;

// Win32资源 函数
// ------------------------
ZEND_FUNCTION(pdr_rc_find) ;
ZEND_FUNCTION(pdr_rc_load) ;
ZEND_FUNCTION(pdr_rc_size) ;
ZEND_FUNCTION(pdr_rc_lock) ;
ZEND_FUNCTION(pdr_rc_read) ;

ZEND_FUNCTION(pdr_rc_filetype_icon) ;
ZEND_FUNCTION(pdr_rc_save_icon) ;

// 共享内存
// ------------------------
ZEND_FUNCTION(pdr_smem_create) ;
ZEND_FUNCTION(pdr_smem_open) ;
ZEND_FUNCTION(pdr_smem_get_size) ;
ZEND_FUNCTION(pdr_smem_release) ;
ZEND_FUNCTION(pdr_smem_write) ;
ZEND_FUNCTION(pdr_smem_read) ;
ZEND_FUNCTION(pdr_smem_clear) ;



/* compiled function list so Zend knows what's in this module */
zend_function_entry pdr_dhtml_functions[] = {

	// 系统函数
	// ------------------------
    ZEND_FE(pdr_get_last_error, NULL)
    ZEND_FE(pdr_execute, NULL)
    ZEND_FE(pdr_adjust_token_privileges, NULL)
    ZEND_FE(pdr_get_process_filename, NULL)
    ZEND_FE(pdr_pump_msg, NULL)
    ZEND_FE(pdr_peek_msg, NULL)
    ZEND_FE(pdr_get_msg, NULL)
    ZEND_FE(pdr_translate_msg, NULL)
    ZEND_FE(pdr_dispatch_msg, NULL)
    ZEND_FE(pdr_create_mutex, NULL)
    ZEND_FE(pdr_open_mutex, NULL)
    ZEND_FE(pdr_release_mutex, NULL)
    ZEND_FE(pdr_register_hot_key, NULL)
    ZEND_FE(pdr_unregister_hot_key, NULL)
    ZEND_FE(pdr_get_system_path, NULL)
    ZEND_FE(pdr_browse_file, NULL)
    ZEND_FE(pdr_browse_folder, NULL)
    ZEND_FE(pdr_browse_folder2, NULL)
    ZEND_FE(pdr_get_locale, NULL)
    ZEND_FE(pdr_get_charset, NULL)
    ZEND_FE(pdr_get_php_path, NULL)
    ZEND_FE(pdr_get_cursor_pos, NULL)
    ZEND_FE(pdr_msgbox, NULL)
	ZEND_FE(pdr_var_ref_cnt, NULL)
	ZEND_FE(pdr_appbar_get_rect, NULL)
	ZEND_FE(pdr_get_module_handle, NULL)
	ZEND_FE(pdr_load_library, NULL)
	ZEND_FE(pdr_free_library, NULL)
	ZEND_FE(pdr_exe_version_info, NULL)
	ZEND_FE(pdr_get_os_version, NULL)
	ZEND_FE(pdr_debug_, NULL)
	ZEND_FE(pdr_get_static_property, NULL)
	ZEND_FE(pdr_kill_var, NULL)
	ZEND_FE(pdr_set_cursor_pos, NULL)
	ZEND_FE(pdr_keybd_event, NULL)
	ZEND_FE(pdr_mouse_event, NULL)
	ZEND_FE(pdr_block_input, NULL)

    ZEND_FE(pdr_thread_create, NULL)


	// 剪切板 函数
	// ------------------------
	ZEND_FE(pdr_cb_open, NULL)
	ZEND_FE(pdr_cb_close, NULL)
	ZEND_FE(pdr_cb_empty, NULL)
	ZEND_FE(pdr_cb_set, NULL)
	ZEND_FE(pdr_cb_get, NULL)


	// Window 函数
	// ------------------------
    ZEND_FE(pdr_window_set_text, NULL)
    ZEND_FE(pdr_window_get_text, NULL)
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
    ZEND_FE(pdr_window_lock_update, NULL)
    ZEND_FE(pdr_window_set_capture, NULL)

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
    ZEND_FE(pdr_dhtml_set_icon, NULL)

    ZEND_FE(pdr_dhtml_set_html, NULL)
    ZEND_FE(pdr_dhtml_clear_handles, NULL)
    ZEND_FE(pdr_dhtml_set_text, NULL)
    ZEND_FE(pdr_dhtml_set_value, NULL)
    ZEND_FE(pdr_dhtml_get_html, NULL)
    ZEND_FE(pdr_dhtml_get_text, NULL)
    ZEND_FE(pdr_dhtml_get_value, NULL)
    ZEND_FE(pdr_dhtml_call, NULL)
    ZEND_FE(pdr_dhtml_get_baseurl, NULL)

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
	// ------------------------
    ZEND_FE(pdr_menu_create, NULL)
    ZEND_FE(pdr_menu_append, NULL)
    ZEND_FE(pdr_menu_insert, NULL)
    ZEND_FE(pdr_menu_modify, NULL)
    ZEND_FE(pdr_menu_remove, NULL)
    ZEND_FE(pdr_menu_popup, NULL)
    ZEND_FE(pdr_menu_get_hmenu, NULL)
    ZEND_FE(pdr_menu_enable, NULL)
    ZEND_FE(pdr_menu_check, NULL)
    ZEND_FE(pdr_menu_radio_check, NULL)
    ZEND_FE(pdr_menu_item_count, NULL)
    ZEND_FE(pdr_menu_item_text, NULL)

	// 文件操/串口 操作函数
	// ------------------------
    ZEND_FE(pdr_file_create, NULL)
    ZEND_FE(pdr_file_get_handle, NULL)
    ZEND_FE(pdr_file_read, NULL)
    ZEND_FE(pdr_file_write, NULL)
    ZEND_FE(pdr_file_close, NULL)
    ZEND_FE(pdr_com_open, NULL)
    ZEND_FE(pdr_com_stat, NULL)
    ZEND_FE(pdr_com_set_timeouts, NULL)
    ZEND_FE(pdr_com_setup_buffer, NULL)
    ZEND_FE(pdr_handle_write, NULL)
    ZEND_FE(pdr_handle_read, NULL)
    ZEND_FE(pdr_handle_close, NULL)

	// 进程/管道 函数
	// ------------------------
    ZEND_FE(pdr_pipe_create, NULL)
    ZEND_FE(pdr_pipe_get_read_handle, NULL)
    ZEND_FE(pdr_pipe_get_write_handle, NULL)
    ZEND_FE(pdr_pipe_get_size, NULL)
    ZEND_FE(pdr_pipe_peek, NULL)
    ZEND_FE(pdr_pipe_get_std, NULL)
    ZEND_FE(pdr_pipe_set_std, NULL)

    ZEND_FE(pdr_proc_create, NULL)
    ZEND_FE(pdr_proc_get_handle, NULL)
    ZEND_FE(pdr_proc_get_tread_handle, NULL)
    ZEND_FE(pdr_proc_get_id, NULL)
    ZEND_FE(pdr_proc_get_tread_id, NULL)
    ZEND_FE(pdr_proc_terminate, NULL)
    ZEND_FE(pdr_proc_exit_code, NULL)

    ZEND_FE(pdr_proc_enum, NULL)
	

	// Windows注册表 函数
	// ------------------------
    ZEND_FE(pdr_reg_create, NULL)
    ZEND_FE(pdr_reg_open, NULL)
    ZEND_FE(pdr_reg_set_string, NULL)
    ZEND_FE(pdr_reg_set, NULL)
    ZEND_FE(pdr_reg_get_string, NULL)
    ZEND_FE(pdr_reg_get, NULL)
    ZEND_FE(pdr_reg_get_type, NULL)
    ZEND_FE(pdr_reg_delete_value, NULL)
    ZEND_FE(pdr_reg_delete_key, NULL)
    ZEND_FE(pdr_reg_close, NULL)
    ZEND_FE(pdr_reg_flush, NULL)

	// Win32资源 函数
	// ------------------------
    ZEND_FE(pdr_rc_find, NULL)
    ZEND_FE(pdr_rc_load, NULL)
    ZEND_FE(pdr_rc_size, NULL)
    ZEND_FE(pdr_rc_lock, NULL)
    ZEND_FE(pdr_rc_read, NULL)

    ZEND_FE(pdr_rc_filetype_icon, NULL)
    ZEND_FE(pdr_rc_save_icon, NULL)

	// 共享内存 函数
	// ------------------------
    ZEND_FE(pdr_smem_create, NULL)
    ZEND_FE(pdr_smem_open, NULL)
    ZEND_FE(pdr_smem_get_size, NULL)
    ZEND_FE(pdr_smem_release, NULL)
    ZEND_FE(pdr_smem_write, NULL)
    ZEND_FE(pdr_smem_read, NULL)
    ZEND_FE(pdr_smem_clear, NULL)



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




zval * make_msg_object(MSG * pMsg)
{
	if( !pMsg || ((long)pMsg)==1 )
	{
		return NULL ;
	}

	TSRMLS_FETCH() ;
	zval * pZvalMsg = NULL ;
	MAKE_STD_ZVAL(pZvalMsg) ;
	object_init(pZvalMsg);
	pZvalMsg->refcount__gc = 1 ; 

	add_property_long(pZvalMsg, "hwnd", (long)pMsg->hwnd) ;
	add_property_long(pZvalMsg, "message", (long)pMsg->message) ;
	add_property_long(pZvalMsg, "wParam", (long)pMsg->wParam) ;
	add_property_long(pZvalMsg, "lParam", (long)pMsg->lParam) ;
	add_property_long(pZvalMsg, "time", (long)pMsg->time) ;
	

	zval * pZvalMsgPoint = NULL ; 
	MAKE_STD_ZVAL(pZvalMsgPoint) ;
	array_init(pZvalMsgPoint);
	pZvalMsgPoint->refcount__gc = 1 ;
	add_index_long(pZvalMsgPoint,0,pMsg->pt.x) ;
	add_index_long(pZvalMsgPoint,1,pMsg->pt.y) ;

	add_property_zval(pZvalMsg, "pt", pZvalMsgPoint) ;

	return pZvalMsg ;
}

zval * CreatePHPObject()
{
	TSRMLS_FETCH() ;
	zval * pPHPObject = NULL ;
	MAKE_STD_ZVAL(pPHPObject) ;
	object_init(pPHPObject);

	return pPHPObject ;
}

void make_msg_from_object(zval * pzvObject,MSG * pMsg)
{
	TSRMLS_FETCH() ;

	zval * pZvalHwnd = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"hwnd",4,0 TSRMLS_CC) ; 
	zval * pZvalMessage = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"message",7,0 TSRMLS_CC) ; 
	zval * pZvalWParam = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"wParam",6,0 TSRMLS_CC) ; 
	zval * pZvalLParam = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"lParam",6,0 TSRMLS_CC) ; 
	zval * pZvalTime = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"time",4,0 TSRMLS_CC) ; 
	zval * pZvalPT = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"pt",2,0 TSRMLS_CC) ; 
	zval ** ppZvalPTX ; 
	zval ** ppZvalPTY ; 
	if( pZvalPT && Z_ARRVAL_P(pZvalPT) ) 
	{ 
		zend_hash_index_find(Z_ARRVAL_P(pZvalPT), 0, (void**)&ppZvalPTX) ; 
		zend_hash_index_find(Z_ARRVAL_P(pZvalPT), 1, (void**)&ppZvalPTY) ; 
	} 
	else 
	{ 
		ppZvalPTX = NULL ; 
		ppZvalPTY = NULL ; 
	} 
 
	pMsg->hwnd = (HWND)Z_LVAL_P(pZvalHwnd) ; 
	pMsg->message = (UINT)Z_LVAL_P(pZvalMessage) ; 
	pMsg->wParam = (WPARAM)Z_LVAL_P(pZvalWParam) ; 
	pMsg->lParam = (LPARAM)Z_LVAL_P(pZvalLParam) ; 
	pMsg->time = (DWORD)Z_LVAL_P(pZvalTime) ; 
	pMsg->pt.x = ppZvalPTX? Z_LVAL_PP(ppZvalPTX): 0 ; 
	pMsg->pt.y = ppZvalPTY? Z_LVAL_PP(ppZvalPTY): 0 ; 
}