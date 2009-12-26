// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 PDR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// JPDR_DHTML_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef PDR_EXPORTS
#define PDR_API __declspec(dllexport)
#else
#define PDR_API __declspec(dllimport)
#endif

// 此类是从 JPDR_DHtml.dll 导出的
class PDR_API CPDR {
public:
	CPDR(void);
	// TODO: 在此添加您的方法。
};

extern PDR_API int nCPDR;

PDR_API int fnCPDR(void);

#define WM_TRAY_ICON WM_USER+132


#define breakoption CString * p = new CString() ; p=NULL; p->GetLength() ;
#define alert(format,...) CString ss__ ;ss__.Format(format,__VA_ARGS__) ; ::AfxMessageBox(ss__) ;


#define __PDR_RESRC_MPARAM(other_param,...) ,other_param ,__VA_ARGS__


#define resrc_name_pdr_dhtml_dlg "PDR DHTML Dialog"
static int resrc_pdr_dhtml_dlg ;
int _pdr_get_resrc_dhtml_dlg() ;

#define resrc_name_pdr_thread_window "PDR Window Thread"
static int resrc_pdr_thread_window ;
int _pdr_get_resrc_thread_window() ;

#define resrc_name_pdr_menu_icon "PDR Popup Menu"
static int resrc_pdr_menu_icon ;
int _pdr_get_resrc_menu_icon() ;

#define resrc_name_pdr_file "PDR Win32 File"
static int resrc_pdr_file ;
int _pdr_get_resrc_file() ;

#define resrc_name_pdr_proc "PDR Process"
static int resrc_pdr_proc ;
int _pdr_get_resrc_proc() ;

#define resrc_name_pdr_pipe "PDR Pipe"
static int resrc_pdr_pipe ;
int _pdr_get_resrc_pipe() ;



typedef struct _pdr_callback_handle
{
	HashTable * pFuncTable ;
	zval * pFuncName ;
	zval ** ppArgs ;
	int nArgc ;
	//void *** tsrm_ls ;

	~_pdr_callback_handle()
	{
		// 减少变量引用
		for(int i=0;i<nArgc;i++)
		{
			ppArgs[i]->__refcount -- ;
			// ZVAL_DELREF(ppArgs[i]) ;
		}

		// 删除
		delete [] ppArgs ;
	}

} pdr_callback_handle ;


#define _make_msg_object(msg) zval * pZvalMsgPoint = NULL ; \
	MAKE_STD_ZVAL(pZvalMsgPoint) ;\
	array_init(pZvalMsgPoint);\
	add_index_long(pZvalMsgPoint,0,msg.pt.x) ;\
	add_index_long(pZvalMsgPoint,1,msg.pt.y) ;\
\
	zval * pZvalMsg = NULL ;\
	MAKE_STD_ZVAL(pZvalMsg) ;\
	object_init(pZvalMsg);\
	add_property_long(pZvalMsg, "hwnd", (long)msg.hwnd) ;\
	add_property_long(pZvalMsg, "message", (long)msg.message) ;\
	add_property_long(pZvalMsg, "wParam", (long)msg.wParam) ;\
	add_property_long(pZvalMsg, "lParam", (long)msg.lParam) ;\
	add_property_long(pZvalMsg, "time", (long)msg.time) ;\
	add_property_zval(pZvalMsg, "pt", pZvalMsgPoint) ;\

#define _make_msg_object_p(pMsg)	_make_msg_object((*pMsg))


