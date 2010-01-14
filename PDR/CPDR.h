// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PDR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// JPDR_DHTML_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef PDR_EXPORTS
#define PDR_API __declspec(dllexport)
#else
#define PDR_API __declspec(dllimport)
#endif

// �����Ǵ� JPDR_DHtml.dll ������
class PDR_API CPDR {
public:
	CPDR(void);
	// TODO: �ڴ�������ķ�����
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



static DWORD pdr_global_last_error ;
DWORD _pdr_get_global_last_error() ;
DWORD * _pdr_get_global_last_error_ptr() ;

#define set_last_error	DWORD * __pLastError = _pdr_get_global_last_error_ptr() ; \
		*__pLastError = ::GetLastError() ;\


typedef struct _pdr_callback_handle
{
	HashTable * pFuncTable ;
	zval * pFuncName ;
	zval ** ppArgs ;
	int nArgc ;
	//void *** tsrm_ls ;

	~_pdr_callback_handle()
	{
		// ���ٱ��� �ص����� �ı���������
		pFuncName->__refcount -- ;

		// ���ٱ�������
		for(int i=0;i<nArgc;i++)
		{
			ppArgs[i]->__refcount -- ;
			// ZVAL_DELREF(ppArgs[i]) ;
		}

		// ɾ��
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


#define _make_msg_from_object(pzvObject) \
	zval * pZvalHwnd = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"hwnd",4,0 TSRMLS_CC) ;\
	zval * pZvalMessage = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"message",7,0 TSRMLS_CC) ;\
	zval * pZvalWParam = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"wParam",6,0 TSRMLS_CC) ;\
	zval * pZvalLParam = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"lParam",6,0 TSRMLS_CC) ;\
	zval * pZvalTime = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"time",4,0 TSRMLS_CC) ;\
	zval * pZvalPT = zend_read_property(Z_OBJCE_P(pzvObject), pzvObject,"pt",2,0 TSRMLS_CC) ;\
	zval ** ppZvalPTX ;\
	zval ** ppZvalPTY ;\
	if( pZvalPT && Z_ARRVAL_P(pZvalPT) )\
	{\
		zend_hash_index_find(Z_ARRVAL_P(pZvalPT), 0, (void**)&ppZvalPTX) ;\
		zend_hash_index_find(Z_ARRVAL_P(pZvalPT), 1, (void**)&ppZvalPTY) ;\
	}\
	else\
	{\
		ppZvalPTX = NULL ;\
		ppZvalPTY = NULL ;\
	}\
\
	MSG msg ;\
	msg.hwnd = (HWND)Z_LVAL_P(pZvalHwnd) ;\
	msg.message = (UINT)Z_LVAL_P(pZvalMessage) ;\
	msg.wParam = (WPARAM)Z_LVAL_P(pZvalWParam) ;\
	msg.lParam = (LPARAM)Z_LVAL_P(pZvalLParam) ;\
	msg.time = (DWORD)Z_LVAL_P(pZvalTime) ;\
	msg.pt.x = ppZvalPTX? Z_LVAL_PP(ppZvalPTX): 0 ;\
	msg.pt.y = ppZvalPTY? Z_LVAL_PP(ppZvalPTY): 0 ;\


